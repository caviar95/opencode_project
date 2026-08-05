#include <cstdio>
#include <cstdlib>
#include <hfsm/hfsm.hpp>

using namespace hfsm;

// ============================================================
// Example 1: Traffic Light State Machine
// A classic FSM example with timed transitions
// ============================================================

// Events
struct PedestrianRequest
{
};
struct TimerExpired
{
};
struct EmergencyOverride
{
    int priority;
};

// States
struct RedLight
{
    void entry()
    {
        std::printf("[Red] Cars stop, pedestrians go\n");
    }
    void exit()
    {
        std::printf("[Red] Exiting red state\n");
    }
};
struct GreenLight
{
    void entry()
    {
        std::printf("[Green] Cars go, pedestrians wait\n");
    }
    void exit()
    {
        std::printf("[Green] Exiting green state\n");
    }
};
struct YellowLight
{
    void entry()
    {
        std::printf("[Yellow] Caution! Preparing to stop\n");
    }
};
struct BlinkingRed
{
    void entry()
    {
        std::printf("[BlinkingRed] Emergency mode: flashing red\n");
    }
};

// State IDs (using simple enum for clarity)
enum class TrafficLightState : StateId {
    Red,
    Green,
    Yellow,
    BlinkingRed,
};

// Runtime transition table (manual construction for clarity)
struct TrafficLightController
{
    StateMachineEngine engine;

    TrafficLightController()
    {
        engine.set_state_name(static_cast<StateId>(TrafficLightState::Red),
                              "Red");
        engine.set_state_name(static_cast<StateId>(TrafficLightState::Green),
                              "Green");
        engine.set_state_name(static_cast<StateId>(TrafficLightState::Yellow),
                              "Yellow");
        engine.set_state_name(
            static_cast<StateId>(TrafficLightState::BlinkingRed),
            "BlinkingRed");

        engine.register_state(static_cast<StateId>(TrafficLightState::Red));
        engine.register_state(static_cast<StateId>(TrafficLightState::Green));
        engine.register_state(static_cast<StateId>(TrafficLightState::Yellow));
        engine.register_state(
            static_cast<StateId>(TrafficLightState::BlinkingRed));

        engine.set_initial(static_cast<StateId>(TrafficLightState::Red));

        // Red -> Green on timer
        engine.add_rule({static_cast<StateId>(TrafficLightState::Red),
                         typeid(TimerExpired),
                         static_cast<StateId>(TrafficLightState::Green)});

        // Green -> Yellow on timer
        engine.add_rule({static_cast<StateId>(TrafficLightState::Green),
                         typeid(TimerExpired),
                         static_cast<StateId>(TrafficLightState::Yellow)});

        // Yellow -> Red on timer
        engine.add_rule({static_cast<StateId>(TrafficLightState::Yellow),
                         typeid(TimerExpired),
                         static_cast<StateId>(TrafficLightState::Red)});

        // Any -> BlinkingRed on emergency (guarded by priority)
        engine.add_rule(
            {INVALID_STATE, typeid(EmergencyOverride),
             static_cast<StateId>(TrafficLightState::BlinkingRed), true, false,
             [](const EventEnvelope& evt) -> bool {
                 auto& e = evt.get<EmergencyOverride>();
                 return e.priority > 0;
             },
             [](const EventEnvelope& evt) {
                 auto& e = evt.get<EmergencyOverride>();
                 std::printf("[EMERGENCY] Priority %d override!\n", e.priority);
             }});

        // BlinkingRed -> Red on timer
        engine.add_rule({static_cast<StateId>(TrafficLightState::BlinkingRed),
                         typeid(TimerExpired),
                         static_cast<StateId>(TrafficLightState::Red)});
    }

    void run()
    {
        std::printf("\n=== Traffic Light Simulation ===\n\n");
        std::printf("Initial state: %s\n\n",
                    engine.get_state_name(engine.current_state()));

        // Normal cycle
        engine.handle(TimerExpired{});
        engine.handle(TimerExpired{});
        engine.handle(TimerExpired{});

        // Emergency override
        std::printf("\n--- Emergency vehicle approaching! ---\n");
        engine.handle(EmergencyOverride{5});

        // Resume normal operation
        engine.handle(TimerExpired{});

        std::printf("\n=== Simulation Complete ===\n");
    }
};

int main()
{
    TrafficLightController().run();
    return 0;
}
