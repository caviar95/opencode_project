#pragma once

#include <atomic>
#include <condition_variable>
#include <exception>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

namespace stage {

// Runs a sequence of stages on a dedicated worker thread.
//
// Cancellation is cooperative and only honored at stage boundaries:
//   - a stop request never force-kills the stage currently executing;
//     that stage always runs to completion;
//   - once the running stage finishes, all remaining stages are skipped;
//   - the cleanup stage always runs exactly once;
//   - stop() blocks until cleanup has finished, so an RPC caller can rely
//     on the pipeline being fully stopped when it returns.
//
// Cancellation granularity is the stage, not the thread.
class Pipeline {
public:
    using Step = std::function<void(Pipeline&)>;
    using Cleanup = std::function<void(Pipeline&, bool aborted)>;

    Pipeline() = default;
    ~Pipeline();

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    // --- configuration (call before start()) ---
    void set_steps(std::vector<Step> steps);
    void set_cleanup(Cleanup cleanup);

    // --- control ---
    // Spawns the worker thread and begins executing stages.
    // Throws std::logic_error if a run is already in progress.
    void start();

    // Graceful stop: requests cancellation, then blocks until the current
    // stage finishes, remaining stages are skipped, and cleanup has run.
    // Safe to call from any thread; no-op if not running.
    // NOTE: must not be called from inside a stage (would self-deadlock);
    // stages that want to cancel themselves use request_stop() instead.
    void stop();

    // Non-blocking stop request (e.g. from an RPC handler).
    void request_stop() noexcept;

    // Blocks until all work and cleanup has finished.
    // Rethrows the exception that aborted the pipeline, if any.
    void wait();

    // Rethrows the exception that aborted the pipeline, if any.
    void rethrow_if_failed();

    // --- queries ---
    // Stages may poll this to cooperatively shorten their own work.
    bool stop_requested() const noexcept;

    // Index of the stage currently executing; -1 while idle/cleaning/finished.
    int current_stage() const noexcept;

    bool is_running() const noexcept;

private:
    void run();

    std::vector<Step> steps_;
    Cleanup cleanup_;

    std::atomic<bool> stop_flag_{false};
    std::atomic<int> current_{-1};

    mutable std::mutex m_;
    mutable std::condition_variable done_cv_;
    bool done_{false};
    std::exception_ptr failure_;
    std::thread worker_;

    bool running_locked() const { return worker_.joinable() && !done_; }
};

inline void Pipeline::set_steps(std::vector<Step> steps) {
    std::lock_guard<std::mutex> lk(m_);
    if (running_locked())
        throw std::logic_error("Pipeline::set_steps: pipeline already running");
    steps_ = std::move(steps);
}

inline void Pipeline::set_cleanup(Cleanup cleanup) {
    std::lock_guard<std::mutex> lk(m_);
    if (running_locked())
        throw std::logic_error("Pipeline::set_cleanup: pipeline already running");
    cleanup_ = std::move(cleanup);
}

inline void Pipeline::start() {
    std::lock_guard<std::mutex> lk(m_);
    if (running_locked())
        throw std::logic_error("Pipeline::start: already running");
    if (worker_.joinable())
        worker_.join();  // reap a previous finished run before restarting
    stop_flag_.store(false, std::memory_order_relaxed);
    failure_ = nullptr;
    done_ = false;
    current_.store(-1, std::memory_order_relaxed);
    worker_ = std::thread(&Pipeline::run, this);
}

inline void Pipeline::request_stop() noexcept {
    stop_flag_.store(true, std::memory_order_release);
}

inline void Pipeline::stop() {
    request_stop();
    wait();
}

inline void Pipeline::wait() {
    std::unique_lock<std::mutex> lk(m_);
    done_cv_.wait(lk, [this] { return done_ || !worker_.joinable(); });
    lk.unlock();
    rethrow_if_failed();
}

inline void Pipeline::rethrow_if_failed() {
    std::lock_guard<std::mutex> lk(m_);
    if (failure_)
        std::rethrow_exception(failure_);
}

inline bool Pipeline::stop_requested() const noexcept {
    return stop_flag_.load(std::memory_order_acquire);
}

inline int Pipeline::current_stage() const noexcept {
    return current_.load(std::memory_order_relaxed);
}

inline bool Pipeline::is_running() const noexcept {
    std::lock_guard<std::mutex> lk(m_);
    return running_locked();
}

inline void Pipeline::run() {
    std::exception_ptr failure;
    bool aborted = false;
    try {
        for (std::size_t i = 0; i < steps_.size(); ++i) {
            current_.store(static_cast<int>(i), std::memory_order_relaxed);
            steps_[i](*this);  // runs to completion, never interrupted
            if (stop_flag_.load(std::memory_order_acquire)) {
                aborted = true;
                break;
            }
        }
    } catch (...) {
        failure = std::current_exception();
        aborted = true;
    }

    current_.store(-1, std::memory_order_relaxed);

    try {
        if (cleanup_)
            cleanup_(*this, aborted);
    } catch (...) {
        if (!failure)
            failure = std::current_exception();
    }

    {
        std::lock_guard<std::mutex> lk(m_);
        failure_ = failure;
        done_ = true;
    }
    done_cv_.notify_all();
}

inline Pipeline::~Pipeline() {
    request_stop();
    if (worker_.joinable())
        worker_.join();
}

}  // namespace stage
