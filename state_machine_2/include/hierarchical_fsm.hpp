#pragma once

#include <map>
#include <functional>
#include <vector>
#include <algorithm>
#include <iostream>
#include <optional>
#include <set>

template<typename State, typename Event>
class HierarchicalStateMachine {
public:
    using Action = std::function<void()>;
    using Guard = std::function<bool()>;

    struct Transition {
        State target;
        Action action;
        Guard guard;
    };

    void add_state(State state, State parent) {
        parents_[state] = parent;
        children_[parent].push_back(state);
    }

    bool is_ancestor(State ancestor, State state) const {
        auto it = parents_.find(state);
        while (it != parents_.end()) {
            if (it->second == ancestor) return true;
            it = parents_.find(it->second);
        }
        return false;
    }

    bool is_descendant(State descendant, State state) const {
        return is_ancestor(state, descendant);
    }

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
        State current = current_state_;
        std::vector<State> handled_states;

        while (true) {
            auto key = std::make_pair(current, event);
            auto it = transitions_.find(key);
            if (it != transitions_.end()) {
                Transition& tr = it->second;
                if (!tr.guard || tr.guard()) {
                    return execute_transition(current, tr.target, tr.action);
                }
            }

            auto parent_it = parents_.find(current);
            if (parent_it == parents_.end()) break;
            current = parent_it->second;
        }

        return false;
    }

    State current_state() const {
        return current_state_;
    }

    bool is_in_state(State state) const {
        if (current_state_ == state) return true;
        return is_descendant(current_state_, state);
    }

    void reset(State initial) {
        std::vector<State> path;
        State s = initial;
        while (true) {
            path.push_back(s);
            auto it = parents_.find(s);
            if (it == parents_.end()) break;
            s = it->second;
        }

        for (auto it = path.rbegin(); it != path.rend(); ++it) {
            if (auto entry_it = entry_actions_.find(*it); entry_it != entry_actions_.end()) {
                entry_it->second();
            }
        }
        current_state_ = initial;
    }

    std::vector<State> path_to_root(State state) const {
        std::vector<State> path;
        State s = state;
        while (true) {
            path.push_back(s);
            auto it = parents_.find(s);
            if (it == parents_.end()) break;
            s = it->second;
        }
        return path;
    }

    State find_lca(State a, State b) const {
        auto path_a = path_to_root(a);
        auto path_b = path_to_root(b);

        for (auto s : path_a) {
            if (std::find(path_b.begin(), path_b.end(), s) != path_b.end()) {
                return s;
            }
        }
        return a;
    }

    void dump_hierarchy() const {
        std::set<State> roots;
        for (const auto& entry : parents_) {
            State state = entry.first;
            State parent = entry.second;
            (void)state;
            if (parents_.find(parent) == parents_.end()) {
                if (std::find(roots.begin(), roots.end(), parent) == roots.end()) {
                    bool is_child = false;
                    for (const auto& e : parents_) {
                        if (e.second == parent) { is_child = true; break; }
                    }
                    if (!is_child) roots.insert(parent);
                }
            }
        }
        if (roots.empty() && !parents_.empty()) {
            roots.insert(parents_.begin()->second);
        }

        for (State root : roots) {
            dump_node(root, 0);
        }
    }

private:
    bool execute_transition(State from, State to, Action& action) {
        State lca = find_lca(from, to);

        std::vector<State> exit_path;
        State s = from;
        while (s != lca) {
            exit_path.push_back(s);
            auto it = parents_.find(s);
            s = (it != parents_.end()) ? it->second : lca;
        }

        for (auto state : exit_path) {
            if (auto it = exit_actions_.find(state); it != exit_actions_.end()) {
                it->second();
            }
        }

        current_state_ = to;

        if (action) {
            action();
        }

        std::vector<State> entry_path;
        s = to;
        while (s != lca) {
            entry_path.push_back(s);
            auto it = parents_.find(s);
            s = (it != parents_.end()) ? it->second : lca;
        }

        for (auto it = entry_path.rbegin(); it != entry_path.rend(); ++it) {
            if (auto entry_it = entry_actions_.find(*it); entry_it != entry_actions_.end()) {
                entry_it->second();
            }
        }

        return true;
    }

    void dump_node(State state, int depth) const {
        for (int i = 0; i < depth; ++i) std::cout << "  ";
        std::cout << (current_state_ == state ? "> " : "  ") << state;
        if (depth == 0) std::cout << " (root)";
        std::cout << "\n";

        auto it = children_.find(state);
        if (it != children_.end()) {
            for (auto& child : it->second) {
                dump_node(child, depth + 1);
            }
        }
    }

    State current_state_;
    std::map<State, State> parents_;
    std::map<State, std::vector<State>> children_;
    std::map<std::pair<State, Event>, Transition> transitions_;
    std::map<State, Action> entry_actions_;
    std::map<State, Action> exit_actions_;
};
