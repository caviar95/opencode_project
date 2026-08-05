#include "device_control.hpp"
#include "../include/common.hpp"
#include "../include/simple_fsm.hpp"
#include "../include/hierarchical_fsm.hpp"
#include "../include/event_driven_fsm.hpp"
#include "../include/oo_state_machine.hpp"
#include <iostream>
#include <thread>
#include <chrono>

namespace {

void print_separator(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(60, '=') << "\n";
}

void print_transition([[maybe_unused]] DeviceState from, DeviceState to, DeviceEvent event) {
    std::cout << "  [" << to_string(event) << "] " << to_string(to) << "\n";
}

} // anonymous namespace

void run_simple_fsm_example() {
    print_separator("Model 1: Simple State Machine");

    SimpleStateMachine<DeviceState, DeviceEvent> fsm;

    fsm.add_transition(DeviceState::Off, DeviceEvent::PowerOn, DeviceState::Starting);
    fsm.add_transition(DeviceState::Starting, DeviceEvent::StartComplete, DeviceState::Active);
    fsm.add_transition(DeviceState::Active, DeviceEvent::EnterStandby, DeviceState::Standby);
    fsm.add_transition(DeviceState::Standby, DeviceEvent::ExitStandby, DeviceState::Active);
    fsm.add_transition(DeviceState::Active, DeviceEvent::ErrorOccurred, DeviceState::Error);
    fsm.add_transition(DeviceState::Standby, DeviceEvent::ErrorOccurred, DeviceState::Error);
    fsm.add_transition(DeviceState::Error, DeviceEvent::ErrorCleared, DeviceState::Active);
    fsm.add_transition(DeviceState::Active, DeviceEvent::PowerOff, DeviceState::ShuttingDown);
    fsm.add_transition(DeviceState::Standby, DeviceEvent::PowerOff, DeviceState::ShuttingDown);
    fsm.add_transition(DeviceState::Error, DeviceEvent::PowerOff, DeviceState::ShuttingDown);
    fsm.add_transition(DeviceState::ShuttingDown, DeviceEvent::PowerOff, DeviceState::Off);
    fsm.add_transition(DeviceState::Active, DeviceEvent::Suspend, DeviceState::Suspended);
    fsm.add_transition(DeviceState::Suspended, DeviceEvent::Resume, DeviceState::Active);

    fsm.on_entry(DeviceState::Off, []() { std::cout << "  [Entry] Device is now Off\n"; });
    fsm.on_entry(DeviceState::Starting, []() { std::cout << "  [Entry] Device is starting up...\n"; });
    fsm.on_entry(DeviceState::Active, []() { std::cout << "  [Entry] Device is now Active\n"; });
    fsm.on_entry(DeviceState::Standby, []() { std::cout << "  [Entry] Device entered Standby mode\n"; });
    fsm.on_entry(DeviceState::Suspended, []() { std::cout << "  [Entry] Device is Suspended\n"; });
    fsm.on_entry(DeviceState::Error, []() { std::cout << "  [Entry] DEVICE ERROR!\n"; });
    fsm.on_entry(DeviceState::ShuttingDown, []() { std::cout << "  [Entry] Device is shutting down...\n"; });

    fsm.reset(DeviceState::Off);
    std::cout << "  Initial state: " << fsm.current_state() << "\n\n";

    auto process = [&](DeviceEvent event) {
        bool ok = fsm.process_event(event);
        std::cout << "  Event: " << to_string(event)
                  << " -> " << (ok ? "OK" : "IGNORED")
                  << "  [State: " << fsm.current_state() << "]\n";
    };

    process(DeviceEvent::PowerOn);
    process(DeviceEvent::StartComplete);
    process(DeviceEvent::EnterStandby);
    process(DeviceEvent::ExitStandby);
    process(DeviceEvent::Suspend);
    process(DeviceEvent::Resume);
    process(DeviceEvent::ErrorOccurred);
    process(DeviceEvent::PowerOff);
    process(DeviceEvent::PowerOff);

    std::cout << "\n  Final state: " << fsm.current_state() << "\n";
}

void run_hierarchical_fsm_example() {
    print_separator("Model 2: Hierarchical State Machine");

    HierarchicalStateMachine<DeviceState, DeviceEvent> hsm;

    hsm.add_state(DeviceState::Starting, DeviceState::Active);
    hsm.add_state(DeviceState::Standby, DeviceState::Active);
    hsm.add_state(DeviceState::Suspended, DeviceState::Active);
    hsm.add_state(DeviceState::Active, DeviceState::On);
    hsm.add_state(DeviceState::On, DeviceState::PowerOnState);
    hsm.add_state(DeviceState::Off, DeviceState::PowerOnState);
    hsm.add_state(DeviceState::Error, DeviceState::PowerOnState);
    hsm.add_state(DeviceState::ShuttingDown, DeviceState::PowerOnState);

    hsm.add_transition(DeviceState::Off, DeviceEvent::PowerOn, DeviceState::Starting);
    hsm.add_transition(DeviceState::Starting, DeviceEvent::StartComplete, DeviceState::Active);
    hsm.add_transition(DeviceState::Active, DeviceEvent::EnterStandby, DeviceState::Standby);
    hsm.add_transition(DeviceState::Standby, DeviceEvent::ExitStandby, DeviceState::Active);
    hsm.add_transition(DeviceState::Active, DeviceEvent::ErrorOccurred, DeviceState::Error);
    hsm.add_transition(DeviceState::Error, DeviceEvent::ErrorCleared, DeviceState::Active);
    hsm.add_transition(DeviceState::Active, DeviceEvent::Suspend, DeviceState::Suspended);
    hsm.add_transition(DeviceState::Suspended, DeviceEvent::Resume, DeviceState::Active);

    hsm.add_transition(DeviceState::PowerOnState, DeviceEvent::PowerOff, DeviceState::ShuttingDown);
    hsm.add_transition(DeviceState::ShuttingDown, DeviceEvent::PowerOff, DeviceState::Off);

    hsm.on_entry(DeviceState::PowerOnState, []() { std::cout << "  [Entry] Entered PowerOnState (hierarchy root)\n"; });
    hsm.on_entry(DeviceState::Off, []() { std::cout << "  [Entry] Device Off\n"; });
    hsm.on_entry(DeviceState::Starting, []() { std::cout << "  [Entry] Device Starting...\n"; });
    hsm.on_entry(DeviceState::Active, []() { std::cout << "  [Entry] Device Active\n"; });
    hsm.on_entry(DeviceState::Standby, []() { std::cout << "  [Entry] Device Standby\n"; });
    hsm.on_entry(DeviceState::Suspended, []() { std::cout << "  [Entry] Device Suspended\n"; });
    hsm.on_entry(DeviceState::Error, []() { std::cout << "  [Entry] ERROR STATE\n"; });
    hsm.on_entry(DeviceState::ShuttingDown, []() { std::cout << "  [Entry] Shutting Down...\n"; });

    hsm.reset(DeviceState::Off);
    std::cout << "  Initial state: " << hsm.current_state() << "\n";

    std::cout << "\n  State Hierarchy:\n";
    hsm.dump_hierarchy();

    auto process = [&](DeviceEvent event, const std::string& desc) {
        bool ok = hsm.process_event(event);
        std::cout << "\n  " << desc << ":\n";
        std::cout << "    Event: " << to_string(event)
                  << " -> " << (ok ? "OK" : "IGNORED")
                  << "  [State: " << hsm.current_state() << "]\n";
    };

    process(DeviceEvent::PowerOn, "Power On");
    process(DeviceEvent::StartComplete, "Startup Complete");
    process(DeviceEvent::EnterStandby, "Enter Standby");

    std::cout << "\n  Is in On state? " << (hsm.is_in_state(DeviceState::On) ? "Yes" : "No");
    std::cout << "\n  Is in PowerOnState? " << (hsm.is_in_state(DeviceState::PowerOnState) ? "Yes" : "No");

    process(DeviceEvent::ExitStandby, "Exit Standby");
    process(DeviceEvent::ErrorOccurred, "Error Occurred");
    process(DeviceEvent::PowerOff, "Power Off (handled at parent level)");

    hsm.dump_hierarchy();

    process(DeviceEvent::PowerOff, "Complete Shutdown");
    std::cout << "\n  Final state: " << hsm.current_state() << "\n";
}

void run_event_driven_fsm_example() {
    print_separator("Model 3: Event-Driven State Machine (Async)");

    EventDrivenStateMachine<DeviceState, DeviceEvent> fsm;

    fsm.add_transition(DeviceState::Off, DeviceEvent::PowerOn, DeviceState::Starting);
    fsm.add_transition(DeviceState::Starting, DeviceEvent::StartComplete, DeviceState::Active);
    fsm.add_transition(DeviceState::Active, DeviceEvent::EnterStandby, DeviceState::Standby);
    fsm.add_transition(DeviceState::Standby, DeviceEvent::ExitStandby, DeviceState::Active);

    fsm.add_transition(DeviceState::Active, DeviceEvent::ErrorOccurred, DeviceState::Error, nullptr, []() {
        static int error_count = 0;
        return ++error_count <= 1;
    });

    fsm.add_transition(DeviceState::Error, DeviceEvent::ErrorCleared, DeviceState::Active);
    fsm.add_transition(DeviceState::Active, DeviceEvent::PowerOff, DeviceState::ShuttingDown);
    fsm.add_transition(DeviceState::Standby, DeviceEvent::PowerOff, DeviceState::ShuttingDown);
    fsm.add_transition(DeviceState::Error, DeviceEvent::PowerOff, DeviceState::ShuttingDown);
    fsm.add_transition(DeviceState::ShuttingDown, DeviceEvent::PowerOff, DeviceState::Off);

    fsm.on_entry(DeviceState::Off, []() { std::cout << "  [Entry] Off\n"; });
    fsm.on_entry(DeviceState::Active, []() { std::cout << "  [Entry] Active\n"; });
    fsm.on_entry(DeviceState::Error, []() { std::cout << "  [Entry] Error\n"; });

    fsm.add_observer([](DeviceState from, DeviceState to, DeviceEvent event) {
        print_transition(from, to, event);
    });

    fsm.reset(DeviceState::Off);

    std::cout << "  Starting async processing...\n";
    fsm.start_async();

    std::cout << "\n  Posting events asynchronously...\n";
    fsm.post_event(DeviceEvent::PowerOn);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    fsm.post_event(DeviceEvent::StartComplete);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    fsm.post_event(DeviceEvent::EnterStandby);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    fsm.post_event(DeviceEvent::ExitStandby);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    fsm.post_event(DeviceEvent::ErrorOccurred);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    fsm.post_event(DeviceEvent::ErrorCleared);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    int retries = 3;
    while (fsm.pending_events() > 0 && retries-- > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "\n  Performing synchronous event processing...\n";
    bool ok = fsm.process_event(DeviceEvent::PowerOff);
    std::cout << "  Sync PowerOff: " << (ok ? "OK" : "IGNORED")
              << " [State: " << fsm.current_state() << "]\n";

    ok = fsm.process_event(DeviceEvent::PowerOff);
    std::cout << "  Sync PowerOff (shutdown complete): " << (ok ? "OK" : "IGNORED")
              << " [State: " << fsm.current_state() << "]\n";

    fsm.stop_async();
    std::cout << "\n  Final state: " << fsm.current_state() << "\n";
}

void run_device_control_example() {
    print_separator("Comprehensive Device Control Scenario");
    std::cout << "  Simulating real device lifecycle with all patterns\n";

    SimpleStateMachine<DeviceState, DeviceEvent> fsm;

    int power_level = 0;
    int error_count = 0;
    bool safety_lock = false;

    fsm.add_transition(DeviceState::Off, DeviceEvent::PowerOn, DeviceState::Starting,
        [&]() {
            std::cout << "  [Action] Initiating power sequence...\n";
            power_level = 0;
        },
        [&]() {
            return !safety_lock;
        });

    fsm.add_transition(DeviceState::Starting, DeviceEvent::StartComplete, DeviceState::Active,
        [&]() {
            std::cout << "  [Action] Setting power to operational level\n";
            power_level = 100;
        });

    fsm.add_transition(DeviceState::Active, DeviceEvent::EnterStandby, DeviceState::Standby,
        [&]() {
            std::cout << "  [Action] Reducing power to standby level\n";
            power_level = 20;
        });

    fsm.add_transition(DeviceState::Standby, DeviceEvent::ExitStandby, DeviceState::Active,
        [&]() {
            std::cout << "  [Action] Restoring power to operational level\n";
            power_level = 100;
        });

    fsm.add_transition(DeviceState::Active, DeviceEvent::ErrorOccurred, DeviceState::Error,
        [&]() {
            std::cout << "  [Action] Triggering emergency shutdown\n";
            power_level = 0;
            error_count++;
        });

    fsm.add_transition(DeviceState::Error, DeviceEvent::ErrorCleared, DeviceState::Active,
        [&]() {
            std::cout << "  [Action] Restoring after error recovery\n";
            power_level = 100;
        });

    fsm.add_transition(DeviceState::Error, DeviceEvent::PowerOff, DeviceState::ShuttingDown,
        [&]() {
            std::cout << "  [Action] Shutting down from error state\n";
        });

    fsm.add_transition(DeviceState::Active, DeviceEvent::PowerOff, DeviceState::ShuttingDown,
        [&]() {
            std::cout << "  [Action] Normal shutdown initiated\n";
        });

    fsm.add_transition(DeviceState::ShuttingDown, DeviceEvent::PowerOff, DeviceState::Off,
        [&]() {
            std::cout << "  [Action] Power fully removed\n";
            power_level = 0;
        });

    fsm.reset(DeviceState::Off);

    std::cout << "\n  Initial state: " << fsm.current_state() << "\n";
    std::cout << "  Power level: " << power_level << "%\n";

    std::cout << "\n  --- Scenario: Normal operation cycle ---\n";
    fsm.process_event(DeviceEvent::PowerOn);
    std::cout << "  State: " << fsm.current_state() << ", Power: " << power_level << "%\n";
    fsm.process_event(DeviceEvent::StartComplete);
    std::cout << "  State: " << fsm.current_state() << ", Power: " << power_level << "%\n";
    fsm.process_event(DeviceEvent::EnterStandby);
    std::cout << "  State: " << fsm.current_state() << ", Power: " << power_level << "%\n";
    fsm.process_event(DeviceEvent::ExitStandby);
    std::cout << "  State: " << fsm.current_state() << ", Power: " << power_level << "%\n";

    std::cout << "\n  --- Scenario: Error and recovery ---\n";
    fsm.process_event(DeviceEvent::ErrorOccurred);
    std::cout << "  State: " << fsm.current_state() << ", Power: " << power_level << "%\n";
    fsm.process_event(DeviceEvent::ErrorCleared);
    std::cout << "  State: " << fsm.current_state() << ", Power: " << power_level << "%\n";

    std::cout << "\n  --- Scenario: Shutdown ---\n";
    fsm.process_event(DeviceEvent::PowerOff);
    std::cout << "  State: " << fsm.current_state() << ", Power: " << power_level << "%\n";
    fsm.process_event(DeviceEvent::PowerOff);
    std::cout << "  State: " << fsm.current_state() << ", Power: " << power_level << "%\n";

    std::cout << "\n  Error count: " << error_count << "\n";
}

// ─── OO State Machine: state classes ────────────────────────────────────────

namespace {

class OffState;
class StartingState;
class ActiveState;
class StandbyState;
class SuspendedState;
class ErrorState;
class ShuttingDownState;

struct DeviceContext {
    int power_level = 0;
    int error_count = 0;
};

class OffState : public State<DeviceEvent> {
public:
    explicit OffState(DeviceContext& ctx) : ctx_(ctx) {}

    void on_entry() override {
        ctx_.power_level = 0;
    }

    auto handle_event(DeviceEvent event, StateMachine<DeviceEvent>& sm) -> State<DeviceEvent>* override {
        switch (event) {
            case DeviceEvent::PowerOn:
                std::cout << "  [OffState] PowerOn -> Starting\n";
                return sm.state<StartingState>();
            default:
                return nullptr;
        }
    }

    auto name() const -> const char* override { return "Off"; }

private:
    DeviceContext& ctx_;
};

class StartingState : public State<DeviceEvent> {
public:
    explicit StartingState(DeviceContext& ctx) : ctx_(ctx) {}

    void on_entry() override {
        std::cout << "  [Entry] Starting up...\n";
        ctx_.power_level = 0;
    }

    void on_exit() override {
        std::cout << "  [Exit] Startup phase complete\n";
    }

    auto handle_event(DeviceEvent event, StateMachine<DeviceEvent>& sm) -> State<DeviceEvent>* override {
        if (event == DeviceEvent::StartComplete) {
            std::cout << "  [StartingState] StartComplete -> Active\n";
            return sm.state<ActiveState>();
        }
        return nullptr;
    }

    auto name() const -> const char* override { return "Starting"; }

private:
    DeviceContext& ctx_;
};

class ActiveState : public State<DeviceEvent> {
public:
    explicit ActiveState(DeviceContext& ctx) : ctx_(ctx) {}

    void on_entry() override {
        std::cout << "  [Entry] Device Active\n";
        ctx_.power_level = 100;
    }

    auto handle_event(DeviceEvent event, StateMachine<DeviceEvent>& sm) -> State<DeviceEvent>* override {
        switch (event) {
            case DeviceEvent::EnterStandby:
                std::cout << "  [ActiveState] EnterStandby -> Standby\n";
                return sm.state<StandbyState>();
            case DeviceEvent::Suspend:
                std::cout << "  [ActiveState] Suspend -> Suspended\n";
                return sm.state<SuspendedState>();
            case DeviceEvent::ErrorOccurred:
                std::cout << "  [ActiveState] ErrorOccurred -> Error\n";
                ctx_.error_count++;
                return sm.state<ErrorState>();
            case DeviceEvent::PowerOff:
                std::cout << "  [ActiveState] PowerOff -> ShuttingDown\n";
                return sm.state<ShuttingDownState>();
            default:
                return nullptr;
        }
    }

    auto name() const -> const char* override { return "Active"; }

private:
    DeviceContext& ctx_;
};

class StandbyState : public State<DeviceEvent> {
public:
    explicit StandbyState(DeviceContext& ctx) : ctx_(ctx) {}

    void on_entry() override {
        std::cout << "  [Entry] Standby mode\n";
        ctx_.power_level = 20;
    }

    auto handle_event(DeviceEvent event, StateMachine<DeviceEvent>& sm) -> State<DeviceEvent>* override {
        switch (event) {
            case DeviceEvent::ExitStandby:
                std::cout << "  [StandbyState] ExitStandby -> Active\n";
                return sm.state<ActiveState>();
            case DeviceEvent::ErrorOccurred:
                std::cout << "  [StandbyState] ErrorOccurred -> Error\n";
                ctx_.error_count++;
                return sm.state<ErrorState>();
            case DeviceEvent::PowerOff:
                std::cout << "  [StandbyState] PowerOff -> ShuttingDown\n";
                return sm.state<ShuttingDownState>();
            default:
                return nullptr;
        }
    }

    auto name() const -> const char* override { return "Standby"; }

private:
    DeviceContext& ctx_;
};

class SuspendedState : public State<DeviceEvent> {
public:
    explicit SuspendedState(DeviceContext& ctx) : ctx_(ctx) {}

    void on_entry() override {
        std::cout << "  [Entry] Suspended\n";
        ctx_.power_level = 5;
    }

    auto handle_event(DeviceEvent event, StateMachine<DeviceEvent>& sm) -> State<DeviceEvent>* override {
        if (event == DeviceEvent::Resume) {
            std::cout << "  [SuspendedState] Resume -> Active\n";
            return sm.state<ActiveState>();
        }
        return nullptr;
    }

    auto name() const -> const char* override { return "Suspended"; }

private:
    DeviceContext& ctx_;
};

class ErrorState : public State<DeviceEvent> {
public:
    explicit ErrorState(DeviceContext& ctx) : ctx_(ctx) {}

    void on_entry() override {
        std::cout << "  [Entry] ERROR - powering down\n";
        ctx_.power_level = 0;
    }

    auto handle_event(DeviceEvent event, StateMachine<DeviceEvent>& sm) -> State<DeviceEvent>* override {
        switch (event) {
            case DeviceEvent::ErrorCleared:
                std::cout << "  [ErrorState] ErrorCleared -> Active\n";
                return sm.state<ActiveState>();
            case DeviceEvent::PowerOff:
                std::cout << "  [ErrorState] PowerOff -> ShuttingDown\n";
                return sm.state<ShuttingDownState>();
            default:
                return nullptr;
        }
    }

    auto name() const -> const char* override { return "Error"; }

private:
    DeviceContext& ctx_;
};

class ShuttingDownState : public State<DeviceEvent> {
public:
    explicit ShuttingDownState(DeviceContext& ctx) : ctx_(ctx) {}

    void on_entry() override {
        std::cout << "  [Entry] Shutting down...\n";
    }

    auto handle_event(DeviceEvent event, StateMachine<DeviceEvent>& sm) -> State<DeviceEvent>* override {
        if (event == DeviceEvent::PowerOff) {
            std::cout << "  [ShuttingDownState] PowerOff -> Off\n";
            ctx_.power_level = 0;
            return sm.state<OffState>();
        }
        return nullptr;
    }

    auto name() const -> const char* override { return "ShuttingDown"; }

private:
    DeviceContext& ctx_;
};

} // anonymous namespace

void run_oo_state_machine_example() {
    print_separator("Model 4: Object-Oriented State Machine (State Pattern)");

    DeviceContext ctx{0, 0};

    StateMachine<DeviceEvent> sm;
    sm.register_state<OffState>(ctx);
    sm.register_state<StartingState>(ctx);
    sm.register_state<ActiveState>(ctx);
    sm.register_state<StandbyState>(ctx);
    sm.register_state<SuspendedState>(ctx);
    sm.register_state<ErrorState>(ctx);
    sm.register_state<ShuttingDownState>(ctx);

    sm.start<OffState>();
    std::cout << "  Registered " << sm.size() << " states\n";
    std::cout << "  Initial state: " << sm.current_state()->name()
              << "  [Power: " << ctx.power_level << "%]\n";

    auto process = [&](DeviceEvent event, const char* label) {
        std::cout << "\n  --- " << label << " ---\n";
        bool ok = sm.process_event(event);
        std::cout << "  Result: " << (ok ? "OK" : "IGNORED")
                  << "  -> " << sm.current_state()->name()
                  << "  [Power: " << ctx.power_level << "%]\n";
    };

    process(DeviceEvent::PowerOn, "Power On");
    process(DeviceEvent::StartComplete, "Startup Complete");

    std::cout << "\n  Type check: is_in<ActiveState>() = "
              << (sm.is_in<ActiveState>() ? "true" : "false");

    process(DeviceEvent::EnterStandby, "Enter Standby");
    process(DeviceEvent::ExitStandby, "Exit Standby");
    process(DeviceEvent::Suspend, "Suspend");
    process(DeviceEvent::Resume, "Resume");
    process(DeviceEvent::ErrorOccurred, "Error Occurred");
    process(DeviceEvent::PowerOff, "Power Off");
    process(DeviceEvent::PowerOff, "Complete Shutdown");

    std::cout << "\n  Final state: " << sm.current_state()->name() << "\n";
    std::cout << "  Total errors: " << ctx.error_count << "\n";
}
