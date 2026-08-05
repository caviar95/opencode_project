#pragma once
#include <memory>
#include <utility>

namespace ddd::domain::core {

// Minimal Result<T> mirroring std::expected, kept framework-free.
// Success holds value; failure carries a reason string.
template <typename T>
class [[nodiscard]] Result {
   public:
    static Result success(T value) { return Result(std::move(value), std::string(), true); }
    static Result failure(std::string reason) {
        return Result(T{}, std::move(reason), false);
    }

    bool isOk() const { return ok_; }
    bool isErr() const { return !ok_; }
    const T& value() const { return value_; }
    const std::string& error() const { return error_; }

   private:
    Result(T value, std::string error, bool ok)
        : value_(std::move(value)), error_(std::move(error)), ok_(ok) {}
    T value_;
    std::string error_;
    bool ok_;
};

}  // namespace ddd::domain::core