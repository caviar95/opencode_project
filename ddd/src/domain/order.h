#pragma once
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "../common/result.h"
#include "money.h"
#include "order_item.h"
#include "order_status.h"
#include "order_id.h"

namespace ddd {

// 领域事件（聚合写操作后发布，内容不可变）。
struct OrderSubmitted { OrderId order_id; Money total; };
struct OrderCancelled { OrderId order_id; };

// 事件发布器抽象：由 Infra 提供实现（示例为同步打印）。领域层只依赖接口。
class DomainEventPublisher {
 public:
  virtual ~DomainEventPublisher() = default;
  virtual void onOrderSubmitted(const OrderSubmitted& e) = 0;
  virtual void onOrderCancelled(const OrderCancelled& e) = 0;
};

// 聚合根：Order。所有不变量在公共方法内自检（INV-1..INV-6）。
class Order {
 public:
  Order(OrderId id, CustomerId customer, Address addr)
      : id_(id), customer_(customer), addr_(addr), status_(OrderStatus::Created) {}

  const OrderId& id() const { return id_; }
  const CustomerId& customer() const { return customer_; }
  const Address& address() const { return addr_; }
  OrderStatus status() const { return status_; }
  const std::vector<OrderItem>& items() const { return items_; }

  size_t itemCount() const { return items_.size(); }

  // INV-3：总额实时由条目计算，不冗余存储。
  Money total() const {
    Money t = Money::zero();
    for (const auto& it : items_) t = t + it.lineTotal();
    return t;
  }

  // 追加订单项；INV-5 已确认则锁定。
  Result<void> addItem(std::string product, int qty, Money price) {
    if (status_ == OrderStatus::Confirmed) return Result<void>::err(DomainError(ErrorCode::OrderLocked));
    auto r = OrderItem::create(std::move(product), qty, price);
    if (!r) return Result<void>::err(r.err());
    items_.push_back(r.value());
    return Result<void>::ok();
  }

  void setQuantity(size_t idx, int qty) {
    if (status_ == OrderStatus::Confirmed) return;
    if (idx < items_.size()) items_[idx].setQuantity(qty);
  }

  // INV-5：确认后不可删除条目。
  Result<void> removeItem(size_t idx) {
    if (status_ == OrderStatus::Confirmed) return Result<void>::err(DomainError(ErrorCode::OrderLocked));
    if (idx >= items_.size()) return Result<void>::err(DomainError(ErrorCode::NotFound));
    items_.erase(items_.begin() + idx);
    return Result<void>::ok();
  }

  // 提交：INV-1 空订单不可提交；状态 Created -> Confirmed。
  Result<Money> submit() {
    if (items_.empty()) return Result<Money>::err(DomainError(ErrorCode::EmptyOrder));
    if (!status_.canTransitTo(OrderStatus::Confirmed))
      return Result<Money>::err(DomainError(ErrorCode::StatusTransition));
    status_ = OrderStatus::Confirmed;
    return Result<Money>::ok(total());
  }

  Result<void> ship() {
    if (!status_.canTransitTo(OrderStatus::Shipped))
      return Result<void>::err(DomainError(ErrorCode::StatusTransition));
    status_ = OrderStatus::Shipped;
    return Result<void>::ok();
  }

  Result<void> complete() {
    if (!status_.canTransitTo(OrderStatus::Completed))
      return Result<void>::err(DomainError(ErrorCode::StatusTransition));
    status_ = OrderStatus::Completed;
    return Result<void>::ok();
  }

  // INV-6 发货后不可取消；终态不可再流转。
  Result<void> cancel(std::string reason = {}) {
    (void)reason;
    if (status_ == OrderStatus::Shipped) return Result<void>::err(DomainError(ErrorCode::AlreadyShipped));
    if (status_ == OrderStatus::Cancelled) return Result<void>::err(DomainError(ErrorCode::AlreadyCancelled));
    if (!status_.canTransitTo(OrderStatus::Cancelled))
      return Result<void>::err(DomainError(ErrorCode::StatusTransition));
    status_ = OrderStatus::Cancelled;
    return Result<void>::ok();
  }

  void dispatch(DomainEventPublisher* p) const {
    if (!p) return;
    if (status_ == OrderStatus::Confirmed) p->onOrderSubmitted(OrderSubmitted{id_, total()});
    if (status_ == OrderStatus::Cancelled) p->onOrderCancelled(OrderCancelled{id_});
  }

 private:
  OrderId id_;
  CustomerId customer_;
  Address addr_;
  OrderStatus status_;
  std::vector<OrderItem> items_;
};

}  // namespace ddd