#pragma once

#include <any>
#include <cstddef>
#include <type_traits>
#include <typeindex>
#include <utility>

namespace hfsm {

    /// Base type for all events (optional, any type can be an event)
    struct Event
    {
    };

    /// Event traits: determines if a type is an event
    template <typename T, typename = void> struct is_event : std::false_type
    {
    };

    template <typename T>
    struct is_event<T, std::void_t<decltype(std::declval<T>())>>
        : std::true_type
    {
    };

    template <typename T> inline constexpr bool is_event_v = is_event<T>::value;

    /// Event envelope: wraps an event with its type info for dispatch
    class EventEnvelope
    {
    public:
        template <typename E,
                  typename = std::enable_if_t<
                      !std::is_same_v<std::decay_t<E>, EventEnvelope>>>
        explicit EventEnvelope(E&& evt)
            : data_(std::in_place_type<std::decay_t<E>>, std::forward<E>(evt)),
              type_(typeid(std::decay_t<E>))
        {
        }

        template <typename E> bool is() const noexcept
        {
            return type_ == typeid(E);
        }

        template <typename E> E& get()
        {
            return *std::any_cast<E>(&data_);
        }

        template <typename E> const E& get() const
        {
            return *std::any_cast<E>(&data_);
        }

        const std::type_index& type_info() const noexcept
        {
            return type_;
        }

    private:
        std::any data_;
        std::type_index type_;
    };

} // namespace hfsm
