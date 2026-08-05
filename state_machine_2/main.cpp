#include "include/common.hpp"
#include "examples/device_control.hpp"
#include <iostream>
#include <limits>

void show_header() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║     C++ State Machine Models for Device Control     ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
}

void show_menu() {
    std::cout << "\nSelect a state machine model:\n";
    std::cout << "  ┌─────────────────────────────────────────────────────┐\n";
    std::cout << "  │  1. Simple State Machine                            │\n";
    std::cout << "  │     Flat FSM with transition table, entry/exit      │\n";
    std::cout << "  │     actions, guard conditions                       │\n";
    std::cout << "  ├─────────────────────────────────────────────────────┤\n";
    std::cout << "  │  2. Hierarchical State Machine                      │\n";
    std::cout << "  │     Nested states with parent-child inheritance,    │\n";
    std::cout << "  │     event propagation, LCA transition resolution    │\n";
    std::cout << "  ├─────────────────────────────────────────────────────┤\n";
    std::cout << "  │  3. Event-Driven State Machine                      │\n";
    std::cout << "  │     Async event queue, guards, deferred events,     │\n";
    std::cout << "  │     state observers, thread-safe processing         │\n";
    std::cout << "  ├─────────────────────────────────────────────────────┤\n";
    std::cout << "  │  4. Object-Oriented State Machine                    │\n";
    std::cout << "  │     GoF State Pattern: each state is a class with    │\n";
    std::cout << "  │     virtual handle_event(), on_entry(), on_exit()    │\n";
    std::cout << "  ├─────────────────────────────────────────────────────┤\n";
    std::cout << "  │  5. Comprehensive Device Control Demo               │\n";
    std::cout << "  │     Realistic device lifecycle with all patterns    │\n";
    std::cout << "  ├─────────────────────────────────────────────────────┤\n";
    std::cout << "  │  6. Run All Demos                                   │\n";
    std::cout << "  ├─────────────────────────────────────────────────────┤\n";
    std::cout << "  │  0. Exit                                            │\n";
    std::cout << "  └─────────────────────────────────────────────────────┘\n";
    std::cout << "Choice: ";
}

int get_choice() {
    int choice;
    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    return choice;
}

int main() {
    show_header();

    while (true) {
        show_menu();
        int choice = get_choice();

        switch (choice) {
            case 1:
                run_simple_fsm_example();
                break;
            case 2:
                run_hierarchical_fsm_example();
                break;
            case 3:
                run_event_driven_fsm_example();
                break;
            case 4:
                run_oo_state_machine_example();
                break;
            case 5:
                run_device_control_example();
                break;
            case 6:
                run_simple_fsm_example();
                run_hierarchical_fsm_example();
                run_event_driven_fsm_example();
                run_oo_state_machine_example();
                run_device_control_example();
                break;
            case 0:
                std::cout << "Exiting.\n";
                return 0;
            default:
                std::cout << "Invalid choice. Try again.\n";
                break;
        }

        if (choice >= 1 && choice <= 6) {
            std::cout << "\nPress Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
    }

    return 0;
}
