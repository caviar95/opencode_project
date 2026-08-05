#pragma once
#include <iostream>
#include <map>
#include <vector>

#include "../domain/order.h"
#include "../domain/order_id.h"
#include "../domain/order_repository.h"

namespace ddd {

// 仓储：内存实现（Map），仅演示用。归属 Infrastructure 层，实现领域定义的接口。
class InMemoryOrderRepository : public OrderRepository {
 public:
  ~InMemoryOrderRepository() override = default;

  std::optional<Order> findById(const OrderId& id) const override {
    auto it = store_.find(id.value());
    if (it == store_.end()) return std::nullopt;
    return it->second;
  }

  void save(const Order& o) override { store_.insert_or_assign(o.id().value(), o); }

  bool remove(const OrderId& id) override { return store_.erase(id.value()) > 0; }

  std::vector<Order> findPage(int offset, int limit) const override {
    std::vector<Order> out;
    int i = 0;
    for (auto it = store_.begin(); it != store_.end(); ++it) {
      if (i >= offset && static_cast<int>(out.size()) < limit) out.push_back(it->second);
      ++i;
    }
    return out;
  }

  size_t count() const override { return store_.size(); }

 private:
  std::map<std::string, Order> store_;
};

}  // namespace ddd