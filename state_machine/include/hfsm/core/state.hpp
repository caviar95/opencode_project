#pragma once

#include <cstddef>
#include <memory>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace hfsm {

    /// State identifier (numeric)
    using StateId = std::size_t;

    /// Special state IDs
    inline constexpr StateId INVALID_STATE = ~StateId{0};
    inline constexpr StateId ROOT_STATE = 0;

    /// State configuration flags
    enum class StateFlag : uint8_t {
        None = 0,
        Initial = 1 << 0,
        Final = 1 << 1,
        History = 1 << 2,
        DeepHistory = 1 << 3,
        Parallel = 1 << 4,
        Orthogonal = 1 << 5,
    };

    inline constexpr StateFlag operator|(StateFlag a, StateFlag b)
    {
        return static_cast<StateFlag>(static_cast<uint8_t>(a) |
                                      static_cast<uint8_t>(b));
    }

    inline constexpr bool has_flag(StateFlag flags, StateFlag flag)
    {
        return (static_cast<uint8_t>(flags) & static_cast<uint8_t>(flag)) != 0;
    }

    /// Forward declarations
    template <typename C> class Machine;

    /// State descriptor: compile-time metadata for a state
    template <typename T> struct StateTraits
    {
        static constexpr bool is_composite = false;
        static constexpr bool is_orthogonal = false;
        static constexpr bool is_final = false;
        using parent_type = void;
        static constexpr StateId depth = 0;
    };

    /// State instance: runtime state context
    class StateInstance
    {
    public:
        StateInstance() = default;
        explicit StateInstance(StateId id) : id_(id) {}
        virtual ~StateInstance() = default;

        StateId id() const noexcept
        {
            return id_;
        }
        bool valid() const noexcept
        {
            return id_ != INVALID_STATE;
        }

        /// Lifecycle hooks
        virtual void entry() {}
        virtual void exit() {}
        virtual void entry_from(StateId)
        {
            entry();
        }
        virtual void exit_to(StateId)
        {
            exit();
        }

    protected:
        StateId id_ = INVALID_STATE;
    };

    /// Concrete state wrapper: ties a StateTag to a runtime instance
    template <typename Tag> class TypedState : public StateInstance
    {
    public:
        using tag_type = Tag;

        TypedState() : StateInstance(get_static_id<Tag>()) {}

        template <typename StateId> static StateId get_static_id()
        {
            static StateId id = next_id();
            return id;
        }

        static StateId static_id()
        {
            static StateId id = next_id();
            return id;
        }

    private:
        static StateId next_id()
        {
            static StateId counter = 1;
            return counter++;
        }
    };

} // namespace hfsm
