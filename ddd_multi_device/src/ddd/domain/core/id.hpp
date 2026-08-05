#pragma once
#include <cstdint>
#include <string>

namespace ddd::domain::core {

// Identifier value object shared by aggregates.
struct Id {
    std::uint64_t value{0};

    bool operator==(const Id& o) const { return value == o.value; }
    bool operator!=(const Id& o) const { return value != o.value; }
    bool operator<(const Id& o) const { return value < o.value; }
    std::string toString() const { return std::to_string(value); }
};

}  // namespace ddd::domain::core