#pragma once
#include <algorithm>

#include "money.h"

namespace ddd {

// 领域服务：不归属单个聚合的纯业务规则。
// 示例：满额折扣。总额达到阈值后，若未超折扣上限则打折并返回 Quote。
class OrderPricingService {
 public:
  struct Quote {
    Money subtotal;
    int discount_percent;  // 0..100
    Money total;
  };

  Quote discount(Money subtotal, int discount_percent_limit) const {
    int pct = 0;
    if (subtotal >= threshold_ && discount_percent_limit > 0) {
      pct = std::min(discount_percent_limit, max_discount_percent_);
    }
    Money total = apply(subtotal, pct);
    return Quote{subtotal, pct, total};
  }

  static Money apply(Money subtotal, int percent) {
    if (percent <= 0) return subtotal;
    std::int64_t c = subtotal.cents() * (100 - percent) / 100;
    return Money::cents(c);
  }

 private:
  Money threshold_ = Money::cents(50000);  // 满 500.00
  int max_discount_percent_ = 15;          // 折扣上限 15%
};

}  // namespace ddd