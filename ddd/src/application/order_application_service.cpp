#include "order_application_service.h"

namespace ddd {

namespace {
std::string makeOrderId(std::atomic<std::int64_t>& seq) {
  return "ORD-" + std::to_string(++seq);
}
}  // namespace

Result<OrderId> OrderApplicationService::createOrder(const CustomerId& customer, const Address& addr) {
  OrderId id(makeOrderId(seq_));
  Order order(id, customer, addr);
  repo_.save(order);
  return Result<OrderId>::ok(id);
}

Result<void> OrderApplicationService::guardExists(const OrderId& id) const {
  if (!repo_.findById(id)) return Result<void>::err(DomainError(ErrorCode::NotFound));
  return Result<void>::ok();
}

Result<void> OrderApplicationService::addItem(const OrderId& id, const std::string& product, int qty,
                                              const Money& price) {
  auto o = repo_.findById(id);
  if (!o) return Result<void>::err(DomainError(ErrorCode::NotFound));
  auto order = std::move(*o);
  auto r = order.addItem(product, qty, price);
  if (!r) return r;
  repo_.save(order);
  return Result<void>::ok();
}

Result<void> OrderApplicationService::removeItem(const OrderId& id, size_t index) {
  auto o = repo_.findById(id);
  if (!o) return Result<void>::err(DomainError(ErrorCode::NotFound));
  auto order = std::move(*o);
  auto r = order.removeItem(index);
  if (!r) return r;
  repo_.save(order);
  return Result<void>::ok();
}

Result<OrderDto> OrderApplicationService::submit(const OrderId& id) {
  auto o = repo_.findById(id);
  if (!o) return Result<OrderDto>::err(DomainError(ErrorCode::NotFound));
  auto order = std::move(*o);
  auto r = order.submit();
  if (!r) return Result<OrderDto>::err(r.err());
  if (publisher_) publisher_->onOrderSubmitted(OrderSubmitted{id, r.value()});
  repo_.save(order);
  return Result<OrderDto>::ok(toDto(order));
}

Result<OrderDto> OrderApplicationService::cancel(const OrderId& id, std::string reason) {
  auto o = repo_.findById(id);
  if (!o) return Result<OrderDto>::err(DomainError(ErrorCode::NotFound));
  auto order = std::move(*o);
  auto r = order.cancel(reason);
  if (!r) return Result<OrderDto>::err(r.err());
  if (publisher_) publisher_->onOrderCancelled(OrderCancelled{id});
  repo_.save(order);
  return Result<OrderDto>::ok(toDto(order));
}

Result<OrderDto> OrderApplicationService::ship(const OrderId& id) {
  auto o = repo_.findById(id);
  if (!o) return Result<OrderDto>::err(DomainError(ErrorCode::NotFound));
  auto order = std::move(*o);
  auto r = order.ship();
  if (!r) return Result<OrderDto>::err(r.err());
  repo_.save(order);
  return Result<OrderDto>::ok(toDto(order));
}

Result<OrderDto> OrderApplicationService::complete(const OrderId& id) {
  auto o = repo_.findById(id);
  if (!o) return Result<OrderDto>::err(DomainError(ErrorCode::NotFound));
  auto order = std::move(*o);
  auto r = order.complete();
  if (!r) return Result<OrderDto>::err(r.err());
  repo_.save(order);
  return Result<OrderDto>::ok(toDto(order));
}

OrderDto OrderApplicationService::toDto(const Order& o) const {
  OrderDto d;
  d.id = o.id().value();
  d.customer_id = o.customer().value();
  d.status = o.status().label();
  d.total = o.total();
  d.item_count = static_cast<int>(o.itemCount());
  for (const auto& it : o.items()) {
    OrderItemDto item;
    item.product = it.product();
    item.quantity = it.quantity();
    item.unit_price = it.unitPrice();
    item.line_total = it.lineTotal();
    d.items.push_back(std::move(item));
  }
  return d;
}

Result<std::vector<OrderDto>> OrderApplicationService::listOrders() {
  std::vector<OrderDto> out;
  for (const auto& o : repo_.findPage(0, 10000)) out.push_back(toDto(o));
  return Result<std::vector<OrderDto>>::ok(std::move(out));
}

Result<OrderDto> OrderApplicationService::findById(const OrderId& id) {
  auto o = repo_.findById(id);
  if (!o) return Result<OrderDto>::err(DomainError(ErrorCode::NotFound));
  return Result<OrderDto>::ok(toDto(*o));
}

}  // namespace ddd