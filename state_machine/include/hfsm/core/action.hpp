#pragma once

#include <functional>
#include <type_traits>
#include <utility>

namespace hfsm {

    /// Action base class
    template <typename Event, typename Machine> class Action
    {
    public:
        using event_type = Event;
        using machine_type = Machine;

        virtual ~Action() = default;
        virtual void execute(const Event& evt, Machine& sm) = 0;
    };

    /// Function action: wraps a callable as an action
    template <typename F, typename Event, typename Machine>
    class FunctionAction : public Action<Event, Machine>
    {
    public:
        explicit FunctionAction(F&& func) : func_(std::move(func)) {}

        void execute(const Event& evt, Machine& sm) override
        {
            func_(evt, sm);
        }

    private:
        F func_;
    };

    /// Helper to create actions
    template <typename Event, typename Machine, typename F>
    auto make_action(F&& func) -> std::unique_ptr<Action<Event, Machine>>
    {
        return std::make_unique<FunctionAction<F, Event, Machine>>(
            std::forward<F>(func));
    }

    /// Action queue: deferred action execution
    class ActionQueue
    {
    public:
        using ActionFunc = std::function<void()>;

        void push(ActionFunc func)
        {
            queue_.push_back(std::move(func));
        }

        void execute_all()
        {
            auto q = std::move(queue_);
            for (auto& f : q) {
                f();
            }
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
        std::vector<ActionFunc> queue_;
    };

} // namespace hfsm
