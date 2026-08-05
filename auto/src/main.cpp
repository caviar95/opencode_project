#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "auto_pipeline/calculator.h"

using auto_pipeline::Calculator;
using auto_pipeline::Operation;

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "usage: " << argv[0] << " <add|sub|mul|div> <a> <b>\n";
        return 1;
    }

    const auto parse_op = [&](const std::string& name) -> Operation {
        if (name == "add") {
            return Operation::Add;
        }
        if (name == "sub") {
            return Operation::Subtract;
        }
        if (name == "mul") {
            return Operation::Multiply;
        }
        if (name == "div") {
            return Operation::Divide;
        }
        throw std::invalid_argument("unknown operation: " + name);
    };

    try {
        const Operation op = parse_op(argv[1]);
        const double lhs = std::stod(argv[2]);
        const double rhs = std::stod(argv[3]);
        Calculator calc;
        std::cout << calc.apply(op, lhs, rhs) << '\n';
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
