#pragma once

#include <functional>
#include <type_traits>

namespace hfsm {

    /// Guard result
    struct GuardResult
    {
        bool allowed = true;
        const char* reason = nullptr;

        explicit operator bool() const noexcept
        {
            return allowed;
        }
    };

    /// Guard base class
    template <typename Event, typename Machine> class Guard
    {
    public:
        using event_type = Event;
        using machine_type = Machine;

        virtual ~Guard() = default;
        virtual GuardResult check(const Event& evt, const Machine& sm) = 0;
    };

    /// Function guard: wraps a callable as a guard
    template <typename F, typename Event, typename Machine>
    class FunctionGuard : public Guard<Event, Machine>
    {
    public:
        explicit FunctionGuard(F&& func) : func_(std::move(func)) {}

        GuardResult check(const Event& evt, const Machine& sm) override
        {
            if constexpr (std::is_invocable_v<F, const Event&, const Machine&>)
            {
                return func_(evt, sm) ? GuardResult{true}
                                      : GuardResult{false, "guard rejected"};
            }
            else if constexpr (std::is_invocable_v<F, const Event&>) {
                return func_(evt) ? GuardResult{true}
                                  : GuardResult{false, "guard rejected"};
            }
            else if constexpr (std::is_invocable_v<F>) {
                return func_() ? GuardResult{true}
                               : GuardResult{false, "guard rejected"};
            }
            else {
                return GuardResult{true};
            }
        }

    private:
        F func_;
    };

    /// Helper to create guards
    template <typename Event, typename Machine, typename F>
    auto make_guard(F&& func) -> std::unique_ptr<Guard<Event, Machine>>
    {
        return std::make_unique<FunctionGuard<F, Event, Machine>>(
            std::forward<F>(func));
    }

} // namespace hfsm
