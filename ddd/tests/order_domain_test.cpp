#include <iostream>
#include <string>

#include "../src/common/result.h"
#include "../src/domain/money.h"
#include "../src/domain/order.h"
#include "../src/domain/order_id.h"
#include "../src/domain/order_pricing_service.h"
#include "../src/domain/order_status.h"
#include "../src/infrastructure/in_memory_order_repository.h"

using namespace ddd;

static int failures = 0;
static int checks = 0;

#define CHECK(cond)                                            \
  do {                                                         \
    ++checks;                                                  \
    if (!(cond)) {                                              \
      std::cerr << "FAIL line " << __LINE__ << ": " << #cond << "\n"; \
      ++failures;                                              \
    }                                                          \
  } while (0)

int main() {
  // 1. 金额 INV-7
  {
    CHECK(Money::cents(150) + Money::cents(350) == Money::cents(500));
    CHECK(Money::cents(1000) * 3 == Money::cents(3000));
    CHECK(Money::cents(123456).format() == "1234.56");
    CHECK(Money::cents(-50).format() == "-0.50");
    CHECK(Money::cents(100) < Money::cents(200));
    CHECK(Money::zero().is_zero());
  }

  // 2. 订单项 INV-2
  {
    auto bad = OrderItem::create("X", 0, Money::cents(100));
    CHECK(!bad && bad.err().code() == ErrorCode::InvalidQuantity);
    auto neg = OrderItem::create("X", 1, Money::cents(-5));
    CHECK(!neg && neg.err().code() == ErrorCode::InvalidUnitPrice);
    auto ok = OrderItem::create("X", 2, Money::cents(150));
    CHECK(ok.value().lineTotal() == Money::cents(300));
  }

  // 3. 聚合总额 INV-3
  {
    Order o(OrderId("A1"), CustomerId("C1"), Address("City", "Street", "Detail"));
    o.addItem("P1", 2, Money::cents(100));
    o.addItem("P2", 3, Money::cents(200));
    CHECK(o.total() == Money::cents(800));
    CHECK(o.itemCount() == 2);
  }

  // 4. INV-1 空订单不可提交
  {
    Order o(OrderId("A2"), CustomerId("C1"), Address("City", "Street", "Detail"));
    auto r = o.submit();
    CHECK(!r);
    CHECK(r.err().code() == ErrorCode::EmptyOrder);
  }

  // 5. INV-5 提交后锁定条目
  {
    Order o(OrderId("A3"), CustomerId("C1"), Address("City", "Street", "Detail"));
    o.addItem("P1", 1, Money::cents(100));
    auto r = o.submit();
    CHECK(r);
    CHECK(o.status() == OrderStatus::Confirmed);
    auto locked = o.addItem("P2", 1, Money::cents(100));
    CHECK(!locked);
    CHECK(locked.err().code() == ErrorCode::OrderLocked);
  }

  // 6. 状态机 INV-4 / INV-6
  {
    Order o2(OrderId("A5"), CustomerId("C1"), Address("City", "Street", "Detail"));
    o2.addItem("P1", 1, Money::cents(100));
    CHECK(!o2.ship());            // Created 不可直接 ship
    CHECK(!o2.complete());
    CHECK(o2.submit());
    CHECK(o2.status() == OrderStatus::Confirmed);
    CHECK(o2.ship());
    CHECK(o2.status() == OrderStatus::Shipped);
    auto cancelled = o2.cancel(); // 发货后不可取消
    CHECK(!cancelled);
    CHECK(cancelled.err().code() == ErrorCode::AlreadyShipped);
    CHECK(o2.complete());
    CHECK(o2.status() == OrderStatus::Completed);
    CHECK(!o2.complete());        // 终态不可再流转
    CHECK(!o2.ship());
  }

  // 7. 取消
  {
    Order o(OrderId("A7"), CustomerId("C1"), Address("City", "Street", "Detail"));
    o.addItem("P1", 1, Money::cents(100));
    CHECK(o.cancel());
    CHECK(o.status() == OrderStatus::Cancelled);
    CHECK(!o.submit());           // 终态不可提交
    auto again = o.cancel();
    CHECK(!again);
    CHECK(again.err().code() == ErrorCode::AlreadyCancelled);
  }

  // 8. 仓储持久化
  {
    InMemoryOrderRepository repo;
    Order o(OrderId("B1"), CustomerId("C1"), Address("City", "Street", "Detail"));
    o.addItem("P1", 2, Money::cents(100));
    repo.save(o);
    CHECK(repo.count() == 1);
    CHECK(repo.findById(OrderId("B1")).has_value());
    CHECK(repo.findById(OrderId("NOPE")) == std::nullopt);
    CHECK(repo.findPage(0, 10).size() == 1);
    CHECK(repo.remove(OrderId("B1")));
    CHECK(repo.count() == 0);
  }

  // 9. 领域服务折扣：满 500 触发
  {
    OrderPricingService pricing;
    auto full = pricing.discount(Money::cents(60000), 10);
    CHECK(full.discount_percent == 10);
    CHECK(full.total < full.subtotal);
    auto small = pricing.discount(Money::cents(10000), 10);
    CHECK(small.discount_percent == 0);
    CHECK(small.total == small.subtotal);
  }

  // 10. Result 语义
  {
    auto ok = Result<int>::ok(42);
    CHECK(ok.is_ok() && ok.value() == 42);
    Result<int> e = Result<int>::err(DomainError(ErrorCode::NotFound));
    CHECK(e.is_err() && e.err().code() == ErrorCode::NotFound);
    Result<void> v = Result<void>::ok();
    CHECK(v.is_ok());
    Result<void> ve = Result<void>::err(DomainError(ErrorCode::EmptyOrder));
    CHECK(ve.is_err());
  }

  std::cout << "\n" << (checks - failures) << "/" << checks << " checks passed.\n";
  return failures == 0 ? 0 : 1;
}