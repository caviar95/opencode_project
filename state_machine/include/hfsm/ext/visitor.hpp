#pragma once

#include <functional>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

#include "../core/event.hpp"
#include "../core/machine.hpp"
#include "../core/state.hpp"

namespace hfsm {

    // ============================================================
    // State Visitor
    // ============================================================

    /// Visitor for inspecting state machine internals
    class StateVisitor
    {
    public:
        virtual ~StateVisitor() = default;

        virtual void visit_state(StateId id, const char* name, bool active) = 0;
        virtual void visit_transition(StateId src,
                                      StateId dst,
                                      const std::type_index& evt_type) = 0;
        virtual void visit_event(const std::type_index& evt_type,
                                 EventResult result) = 0;
    };

    /// Collects state machine statistics
    struct MachineStats
    {
        std::size_t total_transitions = 0;
        std::size_t handled_events = 0;
        std::size_t unhandled_events = 0;
        std::size_t rejected_guards = 0;
        std::size_t deferred_count = 0;
        std::size_t error_count = 0;

        double avg_transition_time_ms = 0.0;

        void reset()
        {
            total_transitions = 0;
            handled_events = 0;
            unhandled_events = 0;
            rejected_guards = 0;
            deferred_count = 0;
            error_count = 0;
            avg_transition_time_ms = 0.0;
        }
    };

    /// Statistics collector
    class StatsCollector : public StateVisitor
    {
    public:
        void visit_state(StateId, const char*, bool) override {}
        void visit_transition(StateId, StateId, const std::type_index&) override
        {
            ++stats_.total_transitions;
        }
        void visit_event(const std::type_index&, EventResult result) override
        {
            switch (result) {
            case EventResult::Handled:
                ++stats_.handled_events;
                break;
            case EventResult::Unhandled:
                ++stats_.unhandled_events;
                break;
            case EventResult::Rejected:
                ++stats_.rejected_guards;
                break;
            case EventResult::Deferred:
                ++stats_.deferred_count;
                break;
            case EventResult::Error:
                ++stats_.error_count;
                break;
            }
        }

        const MachineStats& stats() const
        {
            return stats_;
        }

    private:
        MachineStats stats_;
    };

} // namespace hfsm
