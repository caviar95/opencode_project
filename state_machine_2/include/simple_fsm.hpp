#pragma once

#include <map>
#include <functional>
#include <optional>
#include <iostream>

template<typename State, typename Event>
class SimpleStateMachine {
public:
    using Action = std::function<void()>;
    using Guard = std::function<bool()>;

    struct Transition {
        State target;
        Action action;
        Guard guard;
    };

    void add_transition(State from, Event event, State to,
                        Action action = nullptr, Guard guard = nullptr) {
        auto key = std::make_pair(from, event);
        transitions_[key] = Transition{to, std::move(action), std::move(guard)};
    }

    void on_entry(State state, Action action) {
        entry_actions_[state] = std::move(action);
    }

    void on_exit(State state, Action action) {
        exit_actions_[state] = std::move(action);
    }

    bool process_event(Event event) {
        auto key = std::make_pair(current_state_, event);
        auto it = transitions_.find(key);
        if (it == transitions_.end()) {
            return false;
        }

        Transition& tr = it->second;

        if (tr.guard && !tr.guard()) {
            return false;
        }

        State previous = current_state_;

        if (auto exit_it = exit_actions_.find(previous); exit_it != exit_actions_.end()) {
            exit_it->second();
        }

        current_state_ = tr.target;

        if (tr.action) {
            tr.action();
        }

        if (auto entry_it = entry_actions_.find(current_state_); entry_it != entry_actions_.end()) {
            entry_it->second();
        }

        return true;
    }

    State current_state() const {
        return current_state_;
    }

    void reset(State initial) {
        current_state_ = initial;
        if (auto it = entry_actions_.find(initial); it != entry_actions_.end()) {
            it->second();
        }
    }

    bool has_transition(State from, Event event) const {
        return transitions_.count(std::make_pair(from, event)) > 0;
    }

    void remove_transition(State from, Event event) {
        transitions_.erase(std::make_pair(from, event));
    }

    void clear() {
        transitions_.clear();
        entry_actions_.clear();
        exit_actions_.clear();
    }

private:
    State current_state_;
    std::map<std::pair<State, Event>, Transition> transitions_;
    std::map<State, Action> entry_actions_;
    std::map<State, Action> exit_actions_;
};
