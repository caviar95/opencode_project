#include "auto_pipeline/calculator.h"

#include <stdexcept>

namespace auto_pipeline {

double Calculator::add(double a, double b) {
    last_result_ = a + b;
    return last_result_;
}

double Calculator::subtract(double a, double b) {
    last_result_ = a - b;
    return last_result_;
}

double Calculator::multiply(double a, double b) {
    last_result_ = a * b;
    return last_result_;
}

double Calculator::divide(double a, double b) {
    if (b == 0.0) {
        throw std::invalid_argument("divide by zero");
    }
    last_result_ = a / b;
    return last_result_;
}

double Calculator::apply(Operation op, double a, double b) {
    switch (op) {
        case Operation::Add:
            return add(a, b);
        case Operation::Subtract:
            return subtract(a, b);
        case Operation::Multiply:
            return multiply(a, b);
        case Operation::Divide:
            return divide(a, b);
    }
    throw std::invalid_argument("unknown operation");
}

double Calculator::last_result() const {
    return last_result_;
}

}  // namespace auto_pipeline
