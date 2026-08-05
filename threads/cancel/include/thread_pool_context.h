#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

namespace opcua {

// ============================================================================
// WorkerContext
// ============================================================================
// 每个工作线程独立拥有的上下文对象。
// 该上下文由 mutex 保护, 可被任意线程 (包括主线程 / 其他 worker) 查询与修改,
// 用于让"外部"线程监控和调节某一条工作线程的运行参数。
//
// 设计约束:
//   - 所有公开方法线程安全 (内部加锁);
//   - 上下文的生命周期由线程池管理 (shared_ptr),
//     因此其他线程持有引用时不会悬垂;
//   - 工作线程通过 update* 方法报告自身运行状态;
//   - 外部线程通过 set* / get* 方法调节 / 查看运行参数。
// ============================================================================
class WorkerContext {
public:
    explicit WorkerContext(uint32_t id) : id_(id) {}

    WorkerContext(const WorkerContext&) = delete;
    WorkerContext& operator=(const WorkerContext&) = delete;

    // ---- 标识 ----
    uint32_t id() const { return id_; }

    // ---- 由 worker 自身报告的运行状态 (其他线程可查询) ----
    bool running() const { return running_.load(std::memory_order_acquire); }
    uint64_t pollCount() const {
        std::lock_guard<std::mutex> lk(mtx_);
        return poll_count_;
    }
    std::string lastValue() const {
        std::lock_guard<std::mutex> lk(mtx_);
        return last_value_;
    }
    uint64_t lastPollMs() const {
        std::lock_guard<std::mutex> lk(mtx_);
        return last_poll_ms_;
    }

    // ---- 外部线程可查询 / 修改的配置参数 ----
    std::string name() const {
        std::lock_guard<std::mutex> lk(mtx_);
        return name_;
    }
    void setName(const std::string& name) {
        std::lock_guard<std::mutex> lk(mtx_);
        name_ = name;
    }

    bool enabled() const {
        std::lock_guard<std::mutex> lk(mtx_);
        return enabled_;
    }
    void setEnabled(bool on) {
        std::lock_guard<std::mutex> lk(mtx_);
        enabled_ = on;
    }

    uint32_t pollIntervalMs() const {
        std::lock_guard<std::mutex> lk(mtx_);
        return poll_interval_ms_;
    }
    void setPollIntervalMs(uint32_t ms) {
        std::lock_guard<std::mutex> lk(mtx_);
        poll_interval_ms_ = ms;
    }

    std::vector<uint32_t> nodeIds() const {
        std::lock_guard<std::mutex> lk(mtx_);
        return node_ids_;
    }
    void setNodeIds(std::vector<uint32_t> ids) {
        std::lock_guard<std::mutex> lk(mtx_);
        node_ids_ = std::move(ids);
    }
    void addNodeId(uint32_t id) {
        std::lock_guard<std::mutex> lk(mtx_);
        node_ids_.push_back(id);
    }

    // 请求 worker 停止 (协同式取消)。worker 自身在循环中检查 shouldStop()。
    void requestStop() { stop_.store(true, std::memory_order_release); }
    bool shouldStop() const { return stop_.load(std::memory_order_acquire); }
    void clearStop() { stop_.store(false, std::memory_order_release); }

    // ---- 供 worker 自身调用的状态报告 ----
    void markRunning() { running_.store(true, std::memory_order_release); }
    void markStopped() { running_.store(false, std::memory_order_release); }
    void reportPoll(uint64_t poll_ms, const std::string& value) {
        std::lock_guard<std::mutex> lk(mtx_);
        ++poll_count_;
        last_poll_ms_ = poll_ms;
        last_value_ = value;
    }

private:
    uint32_t id_ = 0;

    mutable std::mutex mtx_;
    std::string name_;
    bool enabled_ = true;
    uint32_t poll_interval_ms_ = 1000;
    std::vector<uint32_t> node_ids_;

    // worker 自身写入, 外部线程只读:
    uint64_t poll_count_ = 0;
    uint64_t last_poll_ms_ = 0;
    std::string last_value_;

    std::atomic<bool> running_{false};
    std::atomic<bool> stop_{false};
};

// ============================================================================
// ThreadPool
// ============================================================================
// 固定线程数的工作池。每个 worker 线程:
//   - 拥有唯一的 WorkerContext (通过 context(id) 可获取);
//   - 持有一条独立的任务队列 (tasks[id]);
//   - 循环执行队列中最新提交的任务函数, 任务可运行任意时长;
//   - 任务函数接收本线程的 shared_ptr<WorkerContext>,
//     以便 (a) 查询/更新自身状态, (b) 需要时访问其他 worker 的上下文。
//
// 使用模式:
//   ThreadPool pool(4);
//   auto ctx = pool.context(2);            // 查询 / 修改 worker#2 的上下文
//   ctx->setEnabled(false);
//   pool.submit(2, [ctx](auto){ ... });    // 向 worker#2 提交任务
// ============================================================================
class ThreadPool {
public:
    using Task = std::function<void(std::shared_ptr<WorkerContext>)>;

    explicit ThreadPool(size_t num_workers) {
        for (size_t i = 0; i < num_workers; ++i) {
            auto ctx = std::make_shared<WorkerContext>(static_cast<uint32_t>(i));
            ctx->setName("worker-" + std::to_string(i));
            workers_.emplace_back(
                std::thread(&ThreadPool::run, this, i),
                ctx, std::queue<Task>{});
        }
    }

    ~ThreadPool() {
        shutdown();
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    size_t size() const { return workers_.size(); }

    // 获取某个 worker 的上下文。任意线程均可调用, 从而查询 / 修改该 worker。
    std::shared_ptr<WorkerContext> context(size_t id) const {
        return id < workers_.size() ? workers_[id].ctx : nullptr;
    }

    // 向指定 worker 提交一个任务。任务函数会在该 worker 线程上执行。
    bool submit(size_t id, Task task) {
        if (id >= workers_.size()) return false;
        {
            std::lock_guard<std::mutex> lk(workers_[id].mtx);
            workers_[id].tasks.push(std::move(task));
        }
        workers_[id].cv.notify_one();
        return true;
    }

    // 轮询分配: 将任务提交给"下一个"空闲 worker。返回被选中的 worker id, 失败返回 SIZE_MAX。
    size_t submitRoundRobin(Task task) {
        for (size_t i = 0; i < workers_.size(); ++i) {
            size_t id = (next_round_robin_.fetch_add(1)) % workers_.size();
            if (submit(id, std::move(task))) return id;
        }
        return SIZE_MAX;
    }

    // 请求全部 worker 停止当前任务并退出。阻塞直到所有线程结束。
    void shutdown() {
        for (auto& w : workers_) {
            {
                std::lock_guard<std::mutex> lk(w.mtx);
                w.ctx->requestStop();
                w.shutdown = true;
            }
            w.cv.notify_all();
        }
        for (auto& w : workers_) {
            if (w.thread.joinable()) w.thread.join();
        }
    }

    // 仅请求指定 worker 的当前任务协同退出 (不销毁线程)。
    void stopWorker(size_t id) {
        if (auto ctx = context(id)) ctx->requestStop();
    }

private:
    struct Worker {
        std::thread thread;
        std::shared_ptr<WorkerContext> ctx;
        std::queue<Task> tasks;
        std::mutex mtx;
        std::condition_variable cv;
        bool shutdown = false;
    };

    void run(size_t id) {
        auto ctx = workers_[id].ctx;
        ctx->markRunning();

        while (true) {
            Task task;
            {
                std::unique_lock<std::mutex> lk(workers_[id].mtx);
                workers_[id].cv.wait(lk, [&] {
                    return workers_[id].shutdown || !workers_[id].tasks.empty();
                });
                if (workers_[id].tasks.empty()) {
                    if (workers_[id].shutdown) break;
                    continue;
                }
                task = std::move(workers_[id].tasks.front());
                workers_[id].tasks.pop();
            }
            ctx->clearStop();
            try {
                task(ctx);
            } catch (const std::exception&) {
                // 任务异常不应终止 worker 线程, 交由上层自行处理
            }
        }

        ctx->markStopped();
    }

    std::vector<Worker> workers_;
    std::atomic<size_t> next_round_robin_{0};
};

}  // namespace opcua
