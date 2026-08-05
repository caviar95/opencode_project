#include <iostream>
#include <memory>
#include <string>

#include "../application/order_application_service.h"
#include "../domain/order.h"
#include "../domain/order_id.h"
#include "../infrastructure/in_memory_order_repository.h"

using namespace ddd;

namespace {

// 事件订阅器：Infrastructure 实现 DomainEventPublisher，领域不依赖具体输出方式。
class ConsoleEventPublisher : public DomainEventPublisher {
 public:
  void onOrderSubmitted(const OrderSubmitted& e) override {
    std::cout << "  [event] OrderSubmitted " << e.order_id.value() << " total=" << e.total.format() << "\n";
  }
  void onOrderCancelled(const OrderCancelled& e) override {
    std::cout << "  [event] OrderCancelled " << e.order_id.value() << "\n";
  }
};

void printOrder(const OrderDto& d) {
  std::cout << "  #" << d.id << " [" << d.status << "] customer=" << d.customer_id
            << " total=" << d.total.format() << " items=" << d.item_count << "\n";
  for (const auto& it : d.items)
    std::cout << "      - " << it.product << " x" << it.quantity << " @ " << it.unit_price.format()
              << " = " << it.line_total.format() << "\n";
}

void printErr(const DomainError& e) {
  std::cout << "       ! error: " << DomainError::name(e.code()) << " - " << e.message() << "\n";
}

void separator() { std::cout << "\n--------------------------------------------\n\n"; }

}  // namespace

int main() {
  InMemoryOrderRepository repo;
  ConsoleEventPublisher publisher;
  OrderApplicationService svc(repo, &publisher);

  std::cout << "=== DDD 订单管理演示 ===\n";

  // 1. 创建订单
  CustomerId customer("C-1001");
  Address addr("Beijing", "Chaoyang", "xxx Road 1");
  auto created = svc.createOrder(customer, addr);
  if (!created) { std::cout << "create failed\n"; return 1; }
  OrderId order_id = created.value();
  std::cout << "\n{1} 创建订单 => " << order_id.value() << "\n";

  // 2. 追加订单项
  std::cout << "{2} 追加订单项：机械键盘 x1 @899.00，鼠标 x2 @199.00\n";
  if (auto r = svc.addItem(order_id, "机械键盘", 1, Money::cents(89900)); !r) printErr(r.err());
  if (auto r = svc.addItem(order_id, "鼠标", 2, Money::cents(19900)); !r) printErr(r.err());

  separator();

  // 3. 提交订单（触发事件，锁定条目）
  std::cout << "{3} 提交订单\n";
  auto sub = svc.submit(order_id);
  if (!sub) { printErr(sub.err()); return 1; }
  printOrder(sub.value());

  separator();

  // 4. 提交后再修改条目应失败（INV-5 OrderLocked）
  std::cout << "{4} 提交后尝试追加订单项（应被拒绝）\n";
  if (auto r = svc.addItem(order_id, "音箱", 1, Money::cents(59900)); !r) printErr(r.err());
  else std::cout << "   ! 不应成功\n";

  separator();

  // 5. 发货 -> 完成后取消应失败（INV-6）
  std::cout << "{5} 发货后再取消（应被拒绝）\n";
  if (auto r = svc.ship(order_id); r) printOrder(r.value());
  if (auto r = svc.cancel(order_id); !r) printErr(r.err());

  separator();

  // 6. 新订单演示取消 + 领域折扣服务
  std::cout << "{6} 第二个订单：演示取消\n";
  auto c2 = svc.createOrder(CustomerId("C-1002"), Address("Guangzhou", "Tianhe", "yyy Road"));
  OrderId o2 = c2.value();
  svc.addItem(o2, "显示器", 1, Money::cents(149900));
  auto sub2 = svc.submit(o2);
  printOrder(sub2.value());
  if (auto r = svc.cancel(o2, "买家申请取消"); !r) printErr(r.err());
  else printOrder(r.value());

  std::cout << "\n=== 各订单总额（领域服务折扣演示不在此展示） ===\n";
  auto list = svc.listOrders();
  for (auto& d : list.value()) printOrder(d);

  return 0;
}