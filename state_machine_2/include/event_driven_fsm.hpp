#pragma once

#include <map>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>
#include <chrono>
#include <optional>

template<typename State, typename Event>
class EventDrivenStateMachine {
public:
    using Action = std::function<void()>;
    using Guard = std::function<bool()>;
    using StateChangeCallback = std::function<void(State from, State to, Event event)>;

    struct Transition {
        State target;
        Action action;
        Guard guard;
    };

    EventDrivenStateMachine() = default;

    ~EventDrivenStateMachine() {
        stop_async();
    }

    EventDrivenStateMachine(const EventDrivenStateMachine&) = delete;
    EventDrivenStateMachine& operator=(const EventDrivenStateMachine&) = delete;
    EventDrivenStateMachine(EventDrivenStateMachine&&) = delete;
    EventDrivenStateMachine& operator=(EventDrivenStateMachine&&) = delete;

    void add_transition(State from, Event event, State to,
                        Action action = nullptr, Guard guard = nullptr) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto key = std::make_pair(from, event);
        transitions_[key] = Transition{to, std::move(action), std::move(guard)};
    }

    void on_entry(State state, Action action) {
        std::lock_guard<std::mutex> lock(mutex_);
        entry_actions_[state] = std::move(action);
    }

    void on_exit(State state, Action action) {
        std::lock_guard<std::mutex> lock(mutex_);
        exit_actions_[state] = std::move(action);
    }

    void add_observer(StateChangeCallback cb) {
        std::lock_guard<std::mutex> lock(mutex_);
        observers_.push_back(std::move(cb));
    }

    bool process_event(Event event) {
        std::lock_guard<std::mutex> lock(mutex_);
        return process_internal_locked(event);
    }

    void post_event(Event event) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            event_queue_.push(event);
        }
        cv_.notify_one();
    }

    void start_async() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (running_) return;
        running_ = true;
        processing_thread_ = std::thread(&EventDrivenStateMachine::processing_loop, this);
    }

    void stop_async() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!running_) return;
            running_ = false;
        }
        cv_.notify_all();
        if (processing_thread_.joinable()) {
            processing_thread_.join();
        }
    }

    void defer_current_event() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (current_processing_event_.has_value()) {
            deferred_queue_.push(current_processing_event_.value());
        }
    }

    void process_deferred_events() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!deferred_queue_.empty()) {
            Event evt = deferred_queue_.front();
            deferred_queue_.pop();
            event_queue_.push(evt);
        }
    }

    State current_state() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return current_state_;
    }

    void reset(State initial) {
        std::lock_guard<std::mutex> lock(mutex_);
        current_state_ = initial;
        if (auto it = entry_actions_.find(initial); it != entry_actions_.end()) {
            it->second();
        }
    }

    size_t pending_events() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return event_queue_.size();
    }

    size_t deferred_events() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return deferred_queue_.size();
    }

    bool is_running() const {
        return running_.load();
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        transitions_.clear();
        entry_actions_.clear();
        exit_actions_.clear();
        while (!event_queue_.empty()) event_queue_.pop();
        while (!deferred_queue_.empty()) deferred_queue_.pop();
        observers_.clear();
    }

private:
    bool process_internal_locked(Event event) {
        auto key = std::make_pair(current_state_, event);
        auto it = transitions_.find(key);
        if (it == transitions_.end()) {
            return false;
        }

        Transition& tr = it->second;

        if (tr.guard && !tr.guard()) {
            return false;
        }

        State previous = current_state_;

        if (auto exit_it = exit_actions_.find(previous); exit_it != exit_actions_.end()) {
            exit_it->second();
        }

        current_state_ = tr.target;

        if (tr.action) {
            tr.action();
        }

        if (auto entry_it = entry_actions_.find(current_state_); entry_it != entry_actions_.end()) {
            entry_it->second();
        }

        notify_observers_locked(previous, current_state_, event);

        return true;
    }

    void notify_observers_locked(State from, State to, Event event) {
        for (auto& cb : observers_) {
            cb(from, to, event);
        }
    }

    void processing_loop() {
        while (true) {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [this] {
                return !running_ || !event_queue_.empty();
            });

            if (!running_ && event_queue_.empty()) {
                break;
            }

            if (event_queue_.empty()) {
                continue;
            }

            Event event = event_queue_.front();
            event_queue_.pop();
            current_processing_event_ = event;

            lock.unlock();

            {
                std::lock_guard<std::mutex> lock2(mutex_);
                process_internal_locked(event);
                current_processing_event_.reset();
            }
        }
    }

    State current_state_;
    std::optional<Event> current_processing_event_;
    std::map<std::pair<State, Event>, Transition> transitions_;
    std::map<State, Action> entry_actions_;
    std::map<State, Action> exit_actions_;
    std::vector<StateChangeCallback> observers_;

    std::queue<Event> event_queue_;
    std::queue<Event> deferred_queue_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::thread processing_thread_;
    std::atomic<bool> running_{false};
};
