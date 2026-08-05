#pragma once

#include <memory>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include <iostream>
#include <utility>

template<typename Event>
class StateMachine;

template<typename Event>
class State {
public:
    virtual ~State() = default;

    virtual void on_entry() {}
    virtual void on_exit() {}
    virtual auto handle_event(Event event, StateMachine<Event>& sm) -> State* = 0;
    virtual auto name() const -> const char* = 0;

protected:
    State() = default;

private:
    State(const State&) = delete;
    State& operator=(const State&) = delete;
};

template<typename Event>
class StateMachine {
public:
    using StatePtr = std::unique_ptr<State<Event>>;

    template<typename T, typename... Args>
    auto register_state(Args&&... args) -> T& {
        static_assert(std::is_base_of_v<State<Event>, T>, "T must derive from State<Event>");
        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *ptr;
        states_by_type_[std::type_index(typeid(T))] = ptr.get();
        state_ptrs_.push_back(std::move(ptr));
        return ref;
    }

    template<typename T>
    auto state() -> State<Event>* {
        auto it = states_by_type_.find(std::type_index(typeid(T)));
        return it != states_by_type_.end() ? it->second : nullptr;
    }

    template<typename T>
    auto start() -> bool {
        auto it = states_by_type_.find(std::type_index(typeid(T)));
        if (it == states_by_type_.end()) return false;
        current_state_ = it->second;
        current_state_->on_entry();
        return true;
    }

    auto process_event(Event event) -> bool {
        if (!current_state_) return false;

        State<Event>* next = current_state_->handle_event(event, *this);

        if (next == nullptr) return false;

        if (next != current_state_) {
            current_state_->on_exit();
            current_state_ = next;
            current_state_->on_entry();
        }

        return true;
    }

    auto current_state() const -> State<Event>* {
        return current_state_;
    }

    template<typename T>
    auto is_in() const -> bool {
        return dynamic_cast<T*>(current_state_) != nullptr;
    }

    auto size() const -> size_t {
        return state_ptrs_.size();
    }

private:
    std::vector<StatePtr> state_ptrs_;
    std::unordered_map<std::type_index, State<Event>*> states_by_type_;
    State<Event>* current_state_ = nullptr;
};
