#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "auto_pipeline/calculator.h"

namespace {

int failures = 0;

void check(bool condition, const char* expr, const char* file, int line) {
    if (!condition) {
        std::cerr << file << ":" << line << " FAILED: " << expr << '\n';
        ++failures;
    }
}

void check_close(double actual, double expected, double eps = 1e-9,
                 const char* file = __builtin_FILE(), int line = __builtin_LINE()) {
    if (std::abs(actual - expected) > eps) {
        std::cerr << file << ":" << line << " FAILED: got " << actual << ", expected " << expected
                  << '\n';
        ++failures;
    }
}

#define CHECK(cond) check((cond), #cond, __FILE__, __LINE__)
#define CHECK_CLOSE(actual, expected) check_close((actual), (expected))

void test_arithmetic() {
    auto_pipeline::Calculator calc;
    CHECK_CLOSE(calc.add(2.0, 3.0), 5.0);
    CHECK_CLOSE(calc.subtract(5.0, 3.0), 2.0);
    CHECK_CLOSE(calc.multiply(4.0, 2.5), 10.0);
    CHECK_CLOSE(calc.divide(9.0, 3.0), 3.0);
}

void test_apply() {
    auto_pipeline::Calculator calc;
    using auto_pipeline::Operation;
    CHECK_CLOSE(calc.apply(Operation::Add, 1.0, 1.0), 2.0);
    CHECK_CLOSE(calc.apply(Operation::Subtract, 1.0, 1.0), 0.0);
    CHECK_CLOSE(calc.apply(Operation::Multiply, 3.0, 3.0), 9.0);
    CHECK_CLOSE(calc.apply(Operation::Divide, 8.0, 2.0), 4.0);
}

void test_last_result() {
    auto_pipeline::Calculator calc;
    const double unused = calc.multiply(6.0, 7.0);
    CHECK(unused == 42.0);
    CHECK_CLOSE(calc.last_result(), 42.0);
}

void test_divide_by_zero() {
    auto_pipeline::Calculator calc;
    bool threw = false;
    try {
        [[maybe_unused]] const double result = calc.divide(1.0, 0.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    CHECK(threw);
}

}  // namespace

int main() {
    test_arithmetic();
    test_apply();
    test_last_result();
    test_divide_by_zero();

    if (failures > 0) {
        std::cerr << failures << " test(s) failed\n";
        return EXIT_FAILURE;
    }
    std::cout << "all tests passed\n";
    return EXIT_SUCCESS;
}
