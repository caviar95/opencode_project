#pragma once
#include <string>

namespace ddd {

// 领域错误：以 code 标识，message 用于日志/展示。跨层边界用返回值而非异常传递。
enum class ErrorCode {
  None,
  NotFound,          // order not found
  EmptyOrder,        // INV-1
  InvalidQuantity,   // INV-2
  InvalidUnitPrice,  // INV-2
  StatusTransition,  // INV-4
  OrderLocked,       // INV-5 已提交不可改条目
  AlreadyShipped,    // INV-6
  AlreadyCancelled,  // INV-6
};

class DomainError {
public:
  DomainError(ErrorCode code, std::string message = {})
      : code_(code), message_(message.empty() ? defaultMessage(code) : std::move(message)) {}

  ErrorCode code() const { return code_; }
  const std::string& message() const { return message_; }

  static const char* name(ErrorCode c) {
    switch (c) {
      case ErrorCode::NotFound:          return "NotFound";
      case ErrorCode::EmptyOrder:        return "EmptyOrder";
      case ErrorCode::InvalidQuantity:   return "InvalidQuantity";
      case ErrorCode::InvalidUnitPrice:  return "InvalidUnitPrice";
      case ErrorCode::StatusTransition:  return "StatusTransition";
      case ErrorCode::OrderLocked:       return "OrderLocked";
      case ErrorCode::AlreadyShipped:    return "AlreadyShipped";
      case ErrorCode::AlreadyCancelled:  return "AlreadyCancelled";
      case ErrorCode::None:              return "None";
    }
    return "Unknown";
  }

private:
  static std::string defaultMessage(ErrorCode c) {
    switch (c) {
      case ErrorCode::NotFound:          return "order not found";
      case ErrorCode::EmptyOrder:        return "empty order cannot be submitted";
      case ErrorCode::InvalidQuantity:   return "quantity must be positive";
      case ErrorCode::InvalidUnitPrice:  return "unit price must be non-negative";
      case ErrorCode::StatusTransition:  return "illegal status transition";
      case ErrorCode::OrderLocked:       return "order items are locked after submission";
      case ErrorCode::AlreadyShipped:    return "order already shipped and cannot be cancelled";
      case ErrorCode::AlreadyCancelled:  return "order already cancelled";
      default:                           return "unknown domain error";
    }
  }

  ErrorCode code_;
  std::string message_;
};

}  // namespace ddd