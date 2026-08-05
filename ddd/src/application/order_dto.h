#pragma once
#include <string>
#include <vector>

#include "../domain/money.h"
#include "../domain/order_status.h"

namespace ddd {

// 读模型 DTO：给应用层/呈现层展示用的只读视图，不含领域逻辑。
struct OrderItemDto {
  std::string product;
  int quantity = 0;
  Money unit_price = Money::zero();
  Money line_total = Money::zero();
};

struct OrderDto {
  std::string id;
  std::string customer_id;
  std::string status;
  Money total = Money::zero();
  int item_count = 0;
  std::vector<OrderItemDto> items;
};

}  // namespace ddd