#pragma once

#include <cstdint>

namespace auto_pipeline {

enum class Operation : std::uint8_t { Add, Subtract, Multiply, Divide };

// A stateful calculator: every operation stores its result so it can be
// queried later via last_result().
class Calculator {
   public:
    Calculator() = default;

    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);

    // Throws std::invalid_argument when b == 0
    double divide(double a, double b);

    double apply(Operation op, double a, double b);

    [[nodiscard]] double last_result() const;

   private:
    double last_result_{0.0};
};

}  // namespace auto_pipeline
