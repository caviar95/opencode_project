#pragma once
#include <optional>
#include <vector>

#include "order.h"
#include "order_id.h"

namespace ddd {

// 仓储接口：定义在领域层（依赖倒置），由 Infrastructure 实现。
class OrderRepository {
 public:
  virtual ~OrderRepository() = default;
  virtual std::optional<Order> findById(const OrderId&) const = 0;
  virtual void save(const Order&) = 0;
  virtual bool remove(const OrderId&) = 0;
  virtual std::vector<Order> findPage(int offset, int limit) const = 0;
  virtual size_t count() const = 0;
};

}  // namespace ddd