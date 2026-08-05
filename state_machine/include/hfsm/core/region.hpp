#pragma once

#include <cstddef>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "../core/event.hpp"
#include "../core/machine.hpp"
#include "../core/state.hpp"

namespace hfsm {

    // ============================================================
    // Orthogonal Region Support
    // ============================================================

    /// Descriptor for an orthogonal region within a state
    template <typename ParentState, typename... SubStates>
    struct OrthogonalRegion
    {
        using parent_type = ParentState;
        using sub_states = std::tuple<SubStates...>;

        static constexpr std::size_t num_substates = sizeof...(SubStates);
    };

    /// Runtime region state
    struct RegionState
    {
        StateId parent_id = INVALID_STATE;
        StateId active_child = INVALID_STATE;
        StateId initial_child = INVALID_STATE;
        StateId history = INVALID_STATE;
        bool is_active = false;
    };

    /// Region manager: manages orthogonal regions within a machine
    class RegionManager
    {
    public:
        void add_region(StateId parent_id, StateId initial_child)
        {
            regions_[parent_id] = RegionState{
                parent_id, initial_child, initial_child, INVALID_STATE, false};
        }

        void activate_region(StateId parent_id)
        {
            auto it = regions_.find(parent_id);
            if (it != regions_.end()) {
                it->second.is_active = true;
                if (it->second.active_child == INVALID_STATE) {
                    it->second.active_child = it->second.initial_child;
                }
            }
        }

        void deactivate_region(StateId parent_id)
        {
            auto it = regions_.find(parent_id);
            if (it != regions_.end()) {
                it->second.is_active = false;
                it->second.history = it->second.active_child;
            }
        }

        StateId get_active_child(StateId parent_id) const
        {
            auto it = regions_.find(parent_id);
            return it != regions_.end() ? it->second.active_child
                                        : INVALID_STATE;
        }

        void set_active_child(StateId parent_id, StateId child_id)
        {
            auto it = regions_.find(parent_id);
            if (it != regions_.end()) {
                it->second.active_child = child_id;
            }
        }

        bool is_region_active(StateId parent_id) const
        {
            auto it = regions_.find(parent_id);
            return it != regions_.end() && it->second.is_active;
        }

        void clear()
        {
            regions_.clear();
        }

    private:
        std::unordered_map<StateId, RegionState> regions_;
    };

} // namespace hfsm
