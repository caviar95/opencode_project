#pragma once
#include <atomic>
#include <memory>
#include <string>
#include <vector>

#include "../common/result.h"
#include "../domain/money.h"
#include "../domain/order.h"
#include "../domain/order_repository.h"
#include "order_dto.h"

namespace ddd {

// 应用层：用例编排 + 事务边界 + 组装 DTO。不写业务规则。
class OrderApplicationService {
 public:
  OrderApplicationService(OrderRepository& repo, DomainEventPublisher* publisher = nullptr)
      : repo_(repo), publisher_(publisher) {}

  // 1. 创建订单，返回新订单号。
  Result<OrderId> createOrder(const CustomerId& customer, const Address& addr);

  // 2. 追加订单项。
  Result<void> addItem(const OrderId& id, const std::string& product, int qty, const Money& price);

  // 3. 移除订单项。
  Result<void> removeItem(const OrderId& id, size_t index);

  // 4. 提交并确认（锁定条目），发布 OrderSubmitted 事件。
  Result<OrderDto> submit(const OrderId& id);

  // 5. 取消订单，发布 OrderCancelled 事件。
  Result<OrderDto> cancel(const OrderId& id, std::string reason = {});

  // 5b. 发货。
  Result<OrderDto> ship(const OrderId& id);
  // 5c. 完成。
  Result<OrderDto> complete(const OrderId& id);

  // 6. 查询全部订单（读模型 DTO）。
  Result<std::vector<OrderDto>> listOrders();

  // 7. 查询单个订单。
  Result<OrderDto> findById(const OrderId& id);

 private:
  OrderDto toDto(const Order& o) const;
  Result<void> guardExists(const OrderId& id) const;

  OrderRepository& repo_;
  DomainEventPublisher* publisher_;
  std::atomic<std::int64_t> seq_{0};
};

}  // namespace ddd