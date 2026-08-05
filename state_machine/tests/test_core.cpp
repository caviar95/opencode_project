#include <cassert>
#include <cstdio>
#include <cstring>
#include <hfsm/hfsm.hpp>

using namespace hfsm;

// ============================================================
// Core Tests
// ============================================================

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name)                                                             \
    do {                                                                       \
        std::printf("  TEST: %s ... ", name);                                  \
    } while (0)

#define PASS()                                                                 \
    do {                                                                       \
        std::printf("PASSED\n");                                               \
        tests_passed++;                                                        \
    } while (0)

#define FAIL(msg)                                                              \
    do {                                                                       \
        std::printf("FAILED: %s\n", msg);                                      \
        tests_failed++;                                                        \
    } while (0)

#define ASSERT(cond, msg)                                                      \
    do {                                                                       \
        if (!(cond)) {                                                         \
            FAIL(msg);                                                         \
            return;                                                            \
        }                                                                      \
    } while (0)

// ---- Test Events ----
struct EvStart
{
};
struct EvStop
{
};
struct EvReset
{
};
struct EvError
{
    int code;
};

// ---- Test: Basic State Machine ----
void test_basic_transitions()
{
    TEST("basic state transitions");

    StateMachineEngine sm;
    sm.set_state_name(1, "Idle");
    sm.set_state_name(2, "Running");
    sm.set_state_name(3, "Stopped");

    sm.register_state(1);
    sm.register_state(2);
    sm.register_state(3);
    sm.set_initial(1);

    sm.add_rule({1, typeid(EvStart), 2, false, false, {}, {}});
    sm.add_rule({2, typeid(EvStop), 3, false, false, {}, {}});
    sm.add_rule({3, typeid(EvReset), 1, false, false, {}, {}});

    ASSERT(sm.is_in(1), "should start in Idle");
    ASSERT(sm.current_state() == 1, "current state should be Idle");

    auto r1 = sm.handle(EvStart{});
    ASSERT(r1 == EventResult::Handled, "EvStart should be handled");
    ASSERT(sm.is_in(2), "should transition to Running");

    auto r2 = sm.handle(EvStop{});
    ASSERT(r2 == EventResult::Handled, "EvStop should be handled");
    ASSERT(sm.is_in(3), "should transition to Stopped");

    auto r3 = sm.handle(EvReset{});
    ASSERT(r3 == EventResult::Handled, "EvReset should be handled");
    ASSERT(sm.is_in(1), "should transition back to Idle");

    auto r4 = sm.handle(EvReset{});
    ASSERT(r4 == EventResult::Unhandled, "EvReset in Idle should be unhandled");

    PASS();
}

// ---- Test: Guards ----
void test_guards()
{
    TEST("guards");

    StateMachineEngine sm;
    sm.set_state_name(1, "Open");
    sm.set_state_name(2, "Closed");

    sm.register_state(1);
    sm.register_state(2);
    sm.set_initial(1);

    // Guard: only allow close if error code is >= 100
    sm.add_rule({
        1,
        typeid(EvError),
        2,
        false,
        false,
        [](const EventEnvelope& evt) -> bool {
            auto& e = evt.get<EvError>();
            return e.code >= 100;
        },
        {},
    });

    // Should be rejected (code too low)
    auto r1 = sm.handle(EvError{50});
    ASSERT(r1 == EventResult::Unhandled,
           "low code should be rejected by guard");
    ASSERT(sm.is_in(1), "should stay in Open");

    // Should be handled (code high enough)
    auto r2 = sm.handle(EvError{200});
    ASSERT(r2 == EventResult::Handled, "high code should pass guard");
    ASSERT(sm.is_in(2), "should transition to Closed");

    PASS();
}

// ---- Test: Entry/Exit Actions ----
void test_entry_exit_actions()
{
    TEST("entry/exit actions");

    StateMachineEngine sm;
    sm.set_state_name(1, "Idle");
    sm.set_state_name(2, "Active");

    sm.register_state(1);
    sm.register_state(2);
    sm.set_initial(1);

    int entry_count = 0;
    int exit_count = 0;

    sm.on_entry(2, [&](const EventEnvelope&) { entry_count++; });
    sm.on_exit(1, [&](const EventEnvelope&) { exit_count++; });

    sm.add_rule({1, typeid(EvStart), 2, false, false, {}, {}});

    ASSERT(entry_count == 0, "entry should not have been called yet");
    ASSERT(exit_count == 0, "exit should not have been called yet");

    sm.handle(EvStart{});

    ASSERT(entry_count == 1, "entry should be called once");
    ASSERT(exit_count == 1, "exit should be called once");
    ASSERT(sm.is_in(2), "should be in Active");

    PASS();
}

// ---- Test: Actions on Transition ----
void test_transition_actions()
{
    TEST("actions on transition");

    StateMachineEngine sm;
    sm.set_state_name(1, "A");
    sm.set_state_name(2, "B");

    sm.register_state(1);
    sm.register_state(2);
    sm.set_initial(1);

    int action_called = 0;

    sm.add_rule({1,
                 typeid(EvStart),
                 2,
                 false,
                 false,
                 {},
                 [&](const EventEnvelope& evt) {
                     action_called++;
                     auto& e = evt.get<EvStart>();
                     (void)e;
                 }});

    sm.handle(EvStart{});
    ASSERT(action_called == 1, "action should be called once");

    PASS();
}

// ---- Test: Unhandled Events ----
void test_unhandled_events()
{
    TEST("unhandled events");

    StateMachineEngine sm;
    sm.set_state_name(1, "Idle");
    sm.register_state(1);
    sm.set_initial(1);

    // No rules defined
    auto r = sm.handle(EvStart{});
    ASSERT(r == EventResult::Unhandled, "should be unhandled");

    PASS();
}

// ---- Test: Deferred Events ----
void test_deferred_events()
{
    TEST("deferred events");

    MachineConfig cfg;
    cfg.defer_unhandled_events = true;

    StateMachineEngine sm(cfg);
    sm.set_state_name(1, "Idle");
    sm.set_state_name(2, "Ready");
    sm.register_state(1);
    sm.register_state(2);
    sm.set_initial(1);

    sm.add_rule({1, typeid(EvStart), 2, false, false, {}, {}});
    sm.add_rule({2, typeid(EvStop), 1, false, false, {}, {}});

    // EvStop is unhandled in Idle, should be deferred
    auto r1 = sm.handle(EvStop{});
    ASSERT(r1 == EventResult::Deferred, "EvStop should be deferred in Idle");

    // Now move to Ready where EvStop is handled
    auto r2 = sm.handle(EvStart{});
    ASSERT(r2 == EventResult::Handled, "EvStart should be handled");
    ASSERT(sm.is_in(2), "should be in Ready");

    // Process deferred events
    auto r3 = sm.process_deferred();
    ASSERT(r3 == EventResult::Handled, "deferred EvStop should be handled now");
    ASSERT(sm.is_in(1), "should be back in Idle after deferred event");

    PASS();
}

// ---- Test: Reset ----
void test_reset()
{
    TEST("reset");

    StateMachineEngine sm;
    sm.set_state_name(1, "Idle");
    sm.set_state_name(2, "Running");
    sm.register_state(1);
    sm.register_state(2);
    sm.set_initial(1);

    sm.add_rule({1, typeid(EvStart), 2, false, false, {}, {}});

    sm.handle(EvStart{});
    ASSERT(sm.is_in(2), "should be in Running");

    sm.reset();
    ASSERT(sm.is_in(1), "should be back in Idle after reset");

    PASS();
}

// ---- Test: Multiple Rules (Any-Source) ----
void test_any_source()
{
    TEST("any-source rules");

    StateMachineEngine sm;
    sm.set_state_name(1, "A");
    sm.set_state_name(2, "B");
    sm.set_state_name(3, "Error");

    sm.register_state(1);
    sm.register_state(2);
    sm.register_state(3);
    sm.set_initial(1);

    sm.add_rule({1, typeid(EvStart), 2, false, false, {}, {}});

    // Any-source rule: any state + EvError -> Error
    sm.add_rule({INVALID_STATE, typeid(EvError), 3, true, false, {}, {}});

    // Source 1, EvError -> Error (via any-source)
    auto r1 = sm.handle(EvError{0});
    ASSERT(r1 == EventResult::Handled,
           "EvError should be handled via any-source");
    ASSERT(sm.is_in(3), "should be in Error");

    // Reset to A, move to B, then EvError from B
    sm.reset();
    sm.handle(EvStart{});
    ASSERT(sm.is_in(2), "should be in B");

    auto r2 = sm.handle(EvError{0});
    ASSERT(r2 == EventResult::Handled,
           "EvError from B should be handled via any-source");

    PASS();
}

// ---- Test: State Names ----
void test_state_names()
{
    TEST("state names");

    StateMachineEngine sm;
    sm.set_state_name(42, "MyCustomState");
    sm.register_state(42);
    sm.set_initial(42);

    auto name = sm.get_state_name(42);
    ASSERT(std::strcmp(name, "MyCustomState") == 0,
           "should return registered name");

    auto unknown = sm.get_state_name(99);
    ASSERT(std::strcmp(unknown, "unknown") == 0,
           "unregistered state should return 'unknown'");

    PASS();
}

// ---- Test: Event Envelope ----
void test_event_envelope()
{
    TEST("event envelope");

    EvError ev{42};
    EventEnvelope env(ev);

    ASSERT(env.is<EvError>(), "should identify EvError type");
    ASSERT(!env.is<EvStart>(), "should not identify as EvStart");

    auto& extracted = env.get<EvError>();
    ASSERT(extracted.code == 42, "should extract correct event data");

    PASS();
}

// ---- Test: Typed Machine ----
struct TestMachineDef
{
};
void test_typed_machine()
{
    TEST("typed machine");

    Machine<TestMachineDef> sm;

    struct StateA
    {
    };
    struct StateB
    {
    };

    sm.template register_state<StateA>();
    sm.template register_state<StateB>();
    sm.template set_initial<StateA>();

    sm.add_rule({hfsm::TypedStateId<StateA>::id(),
                 typeid(EvStart),
                 hfsm::TypedStateId<StateB>::id(),
                 false,
                 false,
                 {},
                 {}});

    ASSERT(sm.is_in<StateA>(), "should start in StateA");

    sm.handle(EvStart{});
    ASSERT(sm.is_in<StateB>(), "should transition to StateB");

    PASS();
}

// ---- Test: Logging ----
void test_logging()
{
    TEST("logging");

    std::string last_log;
    Logger::instance().set_output(
        [&](const std::string& msg) { last_log = msg; });
    Logger::instance().set_level(LogLevel::Info);

    Logger::instance().info("TestModule", "test message");
    ASSERT(last_log.find("test message") != std::string::npos,
           "log should contain message");

    PASS();
}

// ---- Test: Deferred Event Queue ----
void test_deferred_queue()
{
    TEST("deferred event queue");

    DeferredEventQueue queue;
    ASSERT(queue.empty(), "queue should be empty initially");

    queue.defer(EventEnvelope(EvStart{}));
    queue.defer(EventEnvelope(EvStop{}));
    ASSERT(queue.size() == 2, "queue should have 2 items");

    int processed = 0;
    queue.process_all([&](const EventEnvelope& evt) -> EventResult {
        processed++;
        return EventResult::Handled;
    });

    ASSERT(processed == 2, "should process 2 events");
    ASSERT(queue.empty(), "queue should be empty after processing");

    PASS();
}

int main()
{
    std::printf("=== HFSM Core Tests ===\n\n");

    test_basic_transitions();
    test_guards();
    test_entry_exit_actions();
    test_transition_actions();
    test_unhandled_events();
    test_deferred_events();
    test_reset();
    test_any_source();
    test_state_names();
    test_event_envelope();
    test_typed_machine();
    test_logging();
    test_deferred_queue();

    std::printf("\n=== Results: %d passed, %d failed ===\n", tests_passed,
                tests_failed);

    return tests_failed > 0 ? 1 : 0;
}
