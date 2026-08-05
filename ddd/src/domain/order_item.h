#pragma once
#include <string>

#include "../common/result.h"
#include "money.h"

namespace ddd {

// 实体：订单项。仅能通过聚合根 Order 创建/修改（Aggregate Boundary）。
// 不变量 INV-2：quantity > 0 且 unit_price >= 0。
class OrderItem {
public:
  static Result<OrderItem> create(std::string product, int qty, Money unit_price) {
    if (qty <= 0) return Result<OrderItem>::err(DomainError(ErrorCode::InvalidQuantity));
    if (unit_price < Money::zero()) return Result<OrderItem>::err(DomainError(ErrorCode::InvalidUnitPrice));
    return Result<OrderItem>::ok(OrderItem(std::move(product), qty, unit_price));
  }

  const std::string& product() const { return product_; }
  int quantity() const { return quantity_; }
  Money unitPrice() const { return unit_price_; }

  Money lineTotal() const { return unit_price_ * quantity_; }

  void setQuantity(int q) {  // 由聚合在进入"锁定"状态后调，内部保证不违 INV-2
    if (q > 0) quantity_ = q;
  }

 private:
  OrderItem(std::string product, int qty, Money price)
      : product_(std::move(product)), quantity_(qty), unit_price_(price) {}

  std::string product_;
  int quantity_;
  Money unit_price_;
};

}  // namespace ddd