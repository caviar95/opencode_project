#pragma once

#include <deque>
#include <functional>
#include <type_traits>

#include "../core/event.hpp"
#include "../core/machine.hpp"

namespace hfsm {

    // ============================================================
    // Deferred Event Queue
    // ============================================================

    class DeferredEventQueue
    {
    public:
        using ProcessFn = std::function<EventResult(const EventEnvelope&)>;

        void defer(EventEnvelope evt)
        {
            queue_.push_back(std::move(evt));
        }

        void defer_front(EventEnvelope evt)
        {
            queue_.push_front(std::move(evt));
        }

        /// Process all deferred events, returns false if any unhandled
        bool process_all(ProcessFn handler)
        {
            bool all_handled = true;
            auto q = std::move(queue_);
            queue_.clear();

            for (auto& evt : q) {
                auto result = handler(evt);
                if (result == EventResult::Unhandled) {
                    all_handled = false;
                }
            }

            return all_handled;
        }

        /// Process one deferred event
        EventResult process_one(ProcessFn handler)
        {
            if (queue_.empty())
                return EventResult::Handled;
            auto evt = std::move(queue_.front());
            queue_.pop_front();
            return handler(evt);
        }

        std::size_t size() const noexcept
        {
            return queue_.size();
        }
        bool empty() const noexcept
        {
            return queue_.empty();
        }
        void clear()
        {
            queue_.clear();
        }

    private:
        std::deque<EventEnvelope> queue_;
    };

} // namespace hfsm
