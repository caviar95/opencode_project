#pragma once
#include <ostream>
#include <string>

namespace ddd {

// 值对象：订单状态，携带合法状态机（INV-4）。
class OrderStatus {
 public:
  enum Value {
    Created,
    Confirmed,  // 已提交并确认（锁定条目）
    Shipped,    // 已发货
    Completed,  // 已完成（终态）
    Cancelled,  // 已取消（终态）
  };

  OrderStatus() = default;
  explicit OrderStatus(Value v) : v_(v) {}

  OrderStatus& operator=(Value v) { v_ = v; return *this; }

  bool operator==(OrderStatus o) const { return v_ == o.v_; }
  bool operator==(Value v) const { return v_ == v; }

  // 状态机允许表：next 是否合法。
  static bool canTransit(Value from, Value to) {
    switch (from) {
      case Created:   return to == Confirmed || to == Cancelled;
      case Confirmed: return to == Shipped || to == Cancelled;
      case Shipped:   return to == Completed;
      default:        return false;  // Completed / Cancelled 为终态
    }
  }
  bool canTransitTo(Value to) const { return canTransit(v_, to); }

  Value value() const { return v_; }
  bool isTerminal() const { return v_ == Completed || v_ == Cancelled; }

  std::string label() const {
    switch (v_) {
      case Created:    return "Created";
      case Confirmed:  return "Confirmed";
      case Shipped:    return "Shipped";
      case Completed:  return "Completed";
      case Cancelled:  return "Cancelled";
    }
    return "?";
  }

 private:
  Value v_ = Created;
};

inline std::ostream& operator<<(std::ostream& os, OrderStatus s) {
  return os << s.label();
}

}  // namespace ddd