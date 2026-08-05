#pragma once
#include <cstdint>
#include <string>

namespace ddd {

// 值对象：金额，以"分"为单位（INV-7），不可变。
class Money {
public:
  Money(const Money&) = default;
  Money& operator=(const Money&) = default;

  static Money cents(std::int64_t c) { return Money(c); }
  static Money zero() { return Money(0); }

  std::int64_t cents() const { return cents_; }

  Money operator+(const Money& o) const { return Money(cents_ + o.cents_); }
  Money operator-(const Money& o) const { return Money(cents_ - o.cents_); }
  Money operator*(int n) const { return Money(cents_ * n); }
  bool operator==(const Money& o) const { return cents_ == o.cents_; }
  bool operator!=(const Money& o) const { return !(*this == o); }
  bool operator<(const Money& o) const { return cents_ < o.cents_; }
  bool operator>=(const Money& o) const { return cents_ >= o.cents_; }

  bool is_zero() const { return cents_ == 0; }
  std::string format() const;  // 输出 "12.34"

private:
  explicit Money(std::int64_t cents) : cents_(cents) {}
  std::int64_t cents_;
};

}  // namespace ddd