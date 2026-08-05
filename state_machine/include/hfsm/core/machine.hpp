#pragma once

#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include "event.hpp"
#include "state.hpp"
#include "transition.hpp"

namespace hfsm {

    /// Typed state ID provider (runtime unique ID per type)
    template <typename T> struct TypedStateId
    {
        static StateId id()
        {
            static StateId sid = next_id();
            return sid;
        }

    private:
        static StateId next_id()
        {
            static StateId counter = 1;
            return counter++;
        }
    };

    // ============================================================
    // Event Processing Result
    // ============================================================

    enum class EventResult {
        Handled,
        Unhandled,
        Deferred,
        Rejected,
        Error,
    };

    // ============================================================
    // Runtime Transition Entry
    // ============================================================

    struct RuntimeTransition
    {
        StateId src_id = INVALID_STATE;
        std::type_index event_type = typeid(void);
        StateId dst_id = INVALID_STATE;
        bool is_any = false;
        bool is_internal = false;
        std::function<bool(const EventEnvelope&)> guard;
        std::function<void(const EventEnvelope&)> action;
    };

    // ============================================================
    // Active State Tracking
    // ============================================================

    struct ActiveState
    {
        StateId id = INVALID_STATE;
        std::vector<StateId> active_children;
        StateId history = INVALID_STATE;
        std::vector<StateId> history_stack;
    };

    // ============================================================
    // Machine Configuration
    // ============================================================

    struct MachineConfig
    {
        bool enable_logging = false;
        bool throw_on_invalid_event = false;
        bool defer_unhandled_events = false;
    };

    // ============================================================
    // Logger Interface
    // ============================================================

    using LogFn = std::function<void(const std::string&)>;

    // ============================================================
    // State Machine Engine (Runtime)
    // ============================================================

    class StateMachineEngine
    {
    public:
        explicit StateMachineEngine(MachineConfig cfg = {})
            : config_(std::move(cfg))
        {
        }

        virtual ~StateMachineEngine() = default;

        void register_state(StateId id)
        {
            if (states_.find(id) == states_.end()) {
                states_[id] = ActiveState{id, {}, INVALID_STATE, {}};
            }
        }

        void set_initial(StateId id)
        {
            register_state(id);
            initial_state_ = id;
            if (active_state_.id == INVALID_STATE) {
                active_state_ = states_[id];
            }
        }

        void add_rule(const RuntimeTransition& rule)
        {
            rules_.push_back(rule);
        }

        template <typename E> EventResult handle(const E& raw_event)
        {
            EventEnvelope envelope(raw_event);
            return handle_envelope(envelope);
        }

        EventResult handle_envelope(const EventEnvelope& evt)
        {
            for (const auto& rule : rules_) {
                if (!matches(rule, evt))
                    continue;
                if (rule.guard && !rule.guard(evt)) {
                    log("guard rejected");
                    continue;
                }
                execute_transition(rule, evt);
                return EventResult::Handled;
            }

            if (config_.defer_unhandled_events) {
                deferred_events_.push_back(evt);
                return EventResult::Deferred;
            }

            return EventResult::Unhandled;
        }

        EventResult process_deferred()
        {
            auto events = std::move(deferred_events_);
            EventResult result = EventResult::Handled;
            for (const auto& evt : events) {
                auto r = handle_envelope(evt);
                if (r == EventResult::Unhandled) {
                    result = EventResult::Unhandled;
                }
            }
            return result;
        }

        bool is_in(StateId id) const noexcept
        {
            return active_state_.id == id;
        }

        StateId current_state() const noexcept
        {
            return active_state_.id;
        }

        const char* get_state_name(StateId id) const
        {
            auto it = state_names_.find(id);
            return it != state_names_.end() ? it->second : "unknown";
        }

        void set_state_name(StateId id, const char* name)
        {
            state_names_[id] = name;
        }

        void set_logger(LogFn logger)
        {
            logger_ = std::move(logger);
        }

        void on_entry(StateId id, std::function<void(const EventEnvelope&)> cb)
        {
            entry_actions_[id] = std::move(cb);
        }

        void on_exit(StateId id, std::function<void(const EventEnvelope&)> cb)
        {
            exit_actions_[id] = std::move(cb);
        }

        void reset()
        {
            if (active_state_.id != INVALID_STATE &&
                active_state_.id != initial_state_)
            {
                run_exit(active_state_.id);
            }
            active_state_ = states_[initial_state_];
            if (active_state_.id != INVALID_STATE) {
                run_entry(active_state_.id);
            }
            deferred_events_.clear();
        }

        const MachineConfig& config() const
        {
            return config_;
        }

    protected:
        bool matches(const RuntimeTransition& rule,
                     const EventEnvelope& evt) const
        {
            if (rule.event_type != evt.type_info())
                return false;
            if (rule.is_any)
                return true;
            return rule.src_id == active_state_.id;
        }

        void execute_transition(const RuntimeTransition& rule,
                                const EventEnvelope& evt)
        {
            if (rule.is_internal) {
                if (rule.action)
                    rule.action(evt);
                return;
            }

            StateId exit_id = active_state_.id;
            StateId enter_id = rule.dst_id;

            run_exit(exit_id);

            active_state_ = states_[enter_id];

            if (rule.action) {
                rule.action(evt);
            }

            run_entry(enter_id);

            log(get_state_name(exit_id) + std::string(" -> ") +
                get_state_name(enter_id));
        }

        void run_entry(StateId id)
        {
            auto it = entry_actions_.find(id);
            if (it != entry_actions_.end()) {
                EventEnvelope dummy(int{0});
                it->second(dummy);
            }
        }

        void run_exit(StateId id)
        {
            auto it = exit_actions_.find(id);
            if (it != exit_actions_.end()) {
                EventEnvelope dummy(int{0});
                it->second(dummy);
            }
        }

        void log(const std::string& msg)
        {
            if (logger_) {
                logger_("[hfsm] " + msg);
            }
        }

    protected:
        MachineConfig config_;
        StateId initial_state_ = INVALID_STATE;
        ActiveState active_state_;
        std::unordered_map<StateId, ActiveState> states_;
        std::vector<RuntimeTransition> rules_;
        std::unordered_map<StateId, const char*> state_names_;
        std::unordered_map<StateId, std::function<void(const EventEnvelope&)>>
            entry_actions_;
        std::unordered_map<StateId, std::function<void(const EventEnvelope&)>>
            exit_actions_;
        std::vector<EventEnvelope> deferred_events_;
        LogFn logger_;
    };

    // ============================================================
    // Typed State Machine
    // ============================================================

    template <typename MachineDef> class Machine : public StateMachineEngine
    {
    public:
        using definition = MachineDef;

        Machine(MachineConfig cfg = {}) : StateMachineEngine(std::move(cfg)) {}

        template <typename StateTag> void register_state()
        {
            StateId id = TypedStateId<StateTag>::id();
            StateMachineEngine::register_state(id);
            set_state_name(id, typeid(StateTag).name());
        }

        template <typename StateTag> bool is_in() const noexcept
        {
            return StateMachineEngine::is_in(TypedStateId<StateTag>::id());
        }

        template <typename StateTag> void set_initial()
        {
            StateMachineEngine::set_initial(TypedStateId<StateTag>::id());
        }

        template <typename StateTag> void on_entry(std::function<void()> cb)
        {
            StateMachineEngine::on_entry(
                TypedStateId<StateTag>::id(),
                [cb = std::move(cb)](const EventEnvelope&) { cb(); });
        }

        template <typename StateTag> void on_exit(std::function<void()> cb)
        {
            StateMachineEngine::on_exit(
                TypedStateId<StateTag>::id(),
                [cb = std::move(cb)](const EventEnvelope&) { cb(); });
        }
    };

} // namespace hfsm
