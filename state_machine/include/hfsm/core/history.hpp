#pragma once

#include <cstddef>
#include <unordered_map>
#include <vector>

#include "../core/state.hpp"

namespace hfsm {

    // ============================================================
    // History Support (Shallow & Deep)
    // ============================================================

    /// History mode
    enum class HistoryMode : uint8_t {
        None,
        Shallow, ///< Remember immediate child
        Deep,    ///< Remember full hierarchy
    };

    /// History entry for a state
    struct HistoryEntry
    {
        StateId state_id = INVALID_STATE;
        HistoryMode mode = HistoryMode::None;
        StateId last_active = INVALID_STATE;
        std::vector<StateId> last_active_stack; ///< For deep history
    };

    /// History manager: tracks and restores state history
    class HistoryManager
    {
    public:
        void configure(StateId state_id, HistoryMode mode)
        {
            history_[state_id] =
                HistoryEntry{state_id, mode, INVALID_STATE, {}};
        }

        void record(StateId state_id, StateId active_child)
        {
            auto it = history_.find(state_id);
            if (it == history_.end())
                return;

            it->second.last_active = active_child;

            if (it->second.mode == HistoryMode::Deep) {
                it->second.last_active_stack.push_back(active_child);
            }
        }

        StateId recall(StateId state_id)
        {
            auto it = history_.find(state_id);
            if (it == history_.end() || it->second.last_active == INVALID_STATE)
            {
                return INVALID_STATE;
            }

            StateId restored = it->second.last_active;

            if (it->second.mode == HistoryMode::Deep &&
                !it->second.last_active_stack.empty())
            {
                restored = it->second.last_active_stack.front();
            }

            return restored;
        }

        void clear(StateId state_id)
        {
            auto it = history_.find(state_id);
            if (it != history_.end()) {
                it->second.last_active = INVALID_STATE;
                it->second.last_active_stack.clear();
            }
        }

        void clear_all()
        {
            history_.clear();
        }

        bool has_history(StateId state_id) const
        {
            auto it = history_.find(state_id);
            return it != history_.end() &&
                   it->second.last_active != INVALID_STATE;
        }

    private:
        std::unordered_map<StateId, HistoryEntry> history_;
    };

    // ============================================================
    // History DSL Helpers
    // ============================================================

    /// History pseudo-state
    template <HistoryMode Mode> struct History
    {
    };

    using ShallowHistory = History<HistoryMode::Shallow>;
    using DeepHistory = History<HistoryMode::Deep>;

} // namespace hfsm
