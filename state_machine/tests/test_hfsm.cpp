#include <cassert>
#include <cstdio>
#include <hfsm/core/history.hpp>
#include <hfsm/hfsm.hpp>

using namespace hfsm;

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

struct EvA
{
};
struct EvB
{
};
struct EvC
{
};
struct EvReset
{
};

// ============================================================
// History Tests
// ============================================================

void test_shallow_history()
{
    TEST("shallow history");

    HistoryManager hm;
    hm.configure(100, HistoryMode::Shallow);

    ASSERT(!hm.has_history(100), "should not have history initially");

    hm.record(100, 1);
    ASSERT(hm.has_history(100), "should have history after recording");

    StateId restored = hm.recall(100);
    ASSERT(restored == 1, "should restore last active state");

    hm.record(100, 2);
    restored = hm.recall(100);
    ASSERT(restored == 2, "should restore updated state");

    // Clear
    hm.clear(100);
    ASSERT(!hm.has_history(100), "should not have history after clear");

    PASS();
}

void test_deep_history()
{
    TEST("deep history");

    HistoryManager hm;
    hm.configure(200, HistoryMode::Deep);

    hm.record(200, 1);
    hm.record(200, 2);
    hm.record(200, 3);

    StateId restored = hm.recall(200);
    ASSERT(restored == 1, "deep history should restore first entry");

    PASS();
}

void test_history_no_config()
{
    TEST("history without config");

    HistoryManager hm;
    ASSERT(!hm.has_history(999), "unconfigured state should not have history");

    hm.record(999, 1);
    ASSERT(!hm.has_history(999),
           "unconfigured state should still not have history");

    StateId restored = hm.recall(999);
    ASSERT(restored == INVALID_STATE,
           "unconfigured recall should return INVALID");

    PASS();
}

void test_clear_all()
{
    TEST("history clear all");

    HistoryManager hm;
    hm.configure(1, HistoryMode::Shallow);
    hm.configure(2, HistoryMode::Shallow);

    hm.record(1, 10);
    hm.record(2, 20);

    hm.clear_all();

    ASSERT(!hm.has_history(1), "state 1 should be cleared");
    ASSERT(!hm.has_history(2), "state 2 should be cleared");

    PASS();
}

// ============================================================
// Region Tests
// ============================================================

void test_region_basic()
{
    TEST("region basic");

    RegionManager rm;
    rm.add_region(100, 1);

    ASSERT(!rm.is_region_active(100), "region should not be active initially");

    rm.activate_region(100);
    ASSERT(rm.is_region_active(100), "region should be active");

    StateId child = rm.get_active_child(100);
    ASSERT(child == 1, "active child should be initial child");

    rm.set_active_child(100, 2);
    child = rm.get_active_child(100);
    ASSERT(child == 2, "active child should be updated");

    rm.deactivate_region(100);
    ASSERT(!rm.is_region_active(100), "region should be inactive");

    rm.activate_region(100);
    child = rm.get_active_child(100);
    ASSERT(child == 2, "history should restore last active child");

    PASS();
}

void test_region_no_history()
{
    TEST("region without initial activation");

    RegionManager rm;
    rm.add_region(200, 5);

    // Without activate, active child should be INVALID
    ASSERT(rm.get_active_child(200) == 5, "initial child should be set");

    PASS();
}

void test_region_unknown()
{
    TEST("region unknown state");

    RegionManager rm;
    ASSERT(!rm.is_region_active(999), "unknown region should be inactive");
    ASSERT(rm.get_active_child(999) == INVALID_STATE,
           "unknown region should return INVALID");

    PASS();
}

int main()
{
    std::printf("=== HFSM Extension Tests ===\n\n");

    test_shallow_history();
    test_deep_history();
    test_history_no_config();
    test_clear_all();

    test_region_basic();
    test_region_no_history();
    test_region_unknown();

    std::printf("\n=== Results: %d passed, %d failed ===\n", tests_passed,
                tests_failed);

    return tests_failed > 0 ? 1 : 0;
}
