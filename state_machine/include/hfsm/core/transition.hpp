#pragma once

#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <utility>
#include <vector>

#include "action.hpp"
#include "event.hpp"
#include "guard.hpp"
#include "state.hpp"

namespace hfsm {

    // Forward declarations
    template <typename Src, typename Evt> struct TransitionBuilder;

    template <typename Src, typename Evt, typename G> struct GuardedBuilder;

    template <typename Src, typename Evt, typename G, typename A>
    struct ActionBuilder;

    // ============================================================
    // Transition Kind
    // ============================================================

    enum class TransitionKind : uint8_t {
        External,
        Internal,
        Local,
    };

    // ============================================================
    // Transition Rule (compile-time)
    // ============================================================

    template <typename Src,
              typename Evt,
              typename Dst,
              typename GuardFn,
              typename ActionFn>
    struct Transition
    {
        using src_type = Src;
        using event_type = Evt;
        using dst_type = Dst;
        using guard_type = GuardFn;
        using action_type = ActionFn;

        TransitionKind kind = TransitionKind::External;
        bool is_initial = false;
        bool is_any = false;

        GuardFn guard_fn{};
        ActionFn action_fn{};

        constexpr Transition() = default;

        constexpr Transition(GuardFn g, ActionFn a, TransitionKind k)
            : kind(k), guard_fn(std::move(g)), action_fn(std::move(a))
        {
        }
    };

    // ============================================================
    // DSL: GuardedBuilder (guard selected, needs action and/or dst)
    // ============================================================

    template <typename Src, typename Evt, typename G> struct GuardedBuilder
    {
        using src_type = Src;
        using event_type = Evt;
        using guard_type = G;

        TransitionKind kind;
        bool is_any;
        G guard_fn;

        template <typename A> constexpr auto operator/(A&& action) const
        {
            return ActionBuilder<Src, Evt, G, A>{kind, is_any, guard_fn,
                                                 std::forward<A>(action)};
        }

        template <typename Dst> constexpr auto operator=(Dst* /*dst*/) const
        {
            return Transition<Src, Evt, Dst, G, std::false_type>{
                guard_fn, std::false_type{}, kind};
        }
    };

    // ============================================================
    // DSL: ActionBuilder (action selected, needs dst)
    // ============================================================

    template <typename Src, typename Evt, typename G, typename A>
    struct ActionBuilder
    {
        using src_type = Src;
        using event_type = Evt;
        using guard_type = G;
        using action_type = A;

        TransitionKind kind;
        bool is_any;
        G guard_fn;
        A action_fn;

        template <typename Dst> constexpr auto operator=(Dst* /*dst*/) const
        {
            return Transition<Src, Evt, Dst, G, A>{guard_fn, action_fn, kind};
        }
    };

    // ============================================================
    // DSL: TransitionBuilder (source + event, needs guard/action/dst)
    // ============================================================

    template <typename Src, typename Evt> struct TransitionBuilder
    {
        using src_type = Src;
        using event_type = Evt;

        TransitionKind kind = TransitionKind::External;
        bool is_any = false;

        template <typename G> constexpr auto operator[](G&& guard) const
        {
            return GuardedBuilder<Src, Evt, std::decay_t<G>>{
                kind, is_any, std::forward<G>(guard)};
        }

        template <typename A> constexpr auto operator/(A&& action) const
        {
            return ActionBuilder<Src, Evt, std::false_type, A>{
                kind, is_any, std::false_type{}, std::forward<A>(action)};
        }

        template <typename Dst> constexpr auto operator=(Dst* /*dst*/) const
        {
            return Transition<Src, Evt, Dst, std::true_type, std::false_type>{
                std::true_type{}, std::false_type{}, kind};
        }
    };

    // ============================================================
    // Initial State Marker
    // ============================================================

    template <typename S> struct InitialState
    {
        using type = S;
        S* state_ptr;
    };

    template <typename S> constexpr auto initial(S* s)
    {
        return InitialState<S>{s};
    }

    // ============================================================
    // Transition Table
    // ============================================================

    template <typename... TRules> class TransitionTable
    {
    public:
        static constexpr std::size_t size = sizeof...(TRules);
        using rule_list = std::tuple<TRules...>;

        std::tuple<TRules...> rules_;

        constexpr TransitionTable() = default;

        explicit constexpr TransitionTable(std::tuple<TRules...> rules)
            : rules_(std::move(rules))
        {
        }

        template <std::size_t I> constexpr const auto& get() const
        {
            return std::get<I>(rules_);
        }
    };

    template <typename... T> constexpr auto make_transition_table(T&&... rules)
    {
        return TransitionTable<std::decay_t<T>...>{
            std::make_tuple(std::forward<T>(rules)...)};
    }

} // namespace hfsm
