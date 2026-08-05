#pragma once
#include <cassert>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

#include "domain_error.h"

namespace ddd {

// 统一返回类型：OK 持有 T，Err 持有 DomainError。
// 领域/应用层用返回值而非异常传递业务错误（见 doc 02 第 3 节）。
template <typename T>
class Result {
public:
  using Value = T;
  using Error = DomainError;

  static Result ok(T v) { return Result(std::in_place_index<0>, std::move(v)); }
  static Result err(DomainError e) { return Result(std::in_place_index<1>, std::move(e)); }

  bool is_ok() const { return v_.index() == 0; }
  bool is_err() const { return !is_ok(); }
  explicit operator bool() const { return is_ok(); }

  // 仅在 is_ok() 时调用；否则抛异常（编程错误，而非业务流转）。
  const T& value() const {
    if (!is_ok()) throw std::logic_error("Result::value on error: " + err().message());
    return std::get<0>(v_);
  }
  T& value() {
    if (!is_ok()) throw std::logic_error("Result::value on error: " + err().message());
    return std::get<0>(v_);
  }

  const DomainError& err() const {
    if (is_ok()) throw std::logic_error("Result::err on ok");
    return std::get<1>(v_);
  }

private:
  template <std::size_t I, typename U>
  Result(std::in_place_index_t<I>, U&& u) : v_(std::in_place_index<I>, std::forward<U>(u)) {}
  std::variant<T, DomainError> v_;
};

// void 特化：只需 OK/Err 语义。
template <>
class Result<void> {
public:
  static Result ok() { return Result(); }
  static Result err(DomainError e) { Result r; r.v_ = e; return r; }
  bool is_ok() const { return !v_.has_value(); }
  bool is_err() const { return v_.has_value(); }
  explicit operator bool() const { return is_ok(); }
  const DomainError& err() const {
    if (is_ok()) throw std::logic_error("Result::err on ok");
    return *v_;
  }

private:
  std::optional<DomainError> v_;
};

}  // namespace ddd