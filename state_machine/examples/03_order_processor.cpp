#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <hfsm/hfsm.hpp>
#include <string>
#include <thread>

using namespace hfsm;

// ============================================================
// Example 3: E-Commerce Order Processing Workflow
//
// States: Pending -> PaymentProcessing -> PaymentConfirmed
//         -> Preparing -> Shipped -> Delivered -> Completed
//         Cancelled (from many states)
//
// Guards: payment validation, inventory check
// Actions: charge payment, send email, update inventory
// ============================================================

// Order Events
struct PlaceOrder
{
    int order_id;
    double amount;
    int item_count;
};
struct PaymentReceived
{
    double amount;
    char transaction_id[64];
};
struct PaymentFailed
{
    int error_code;
    char reason[128];
};
struct InventoryReserved
{
};
struct InventoryShort
{
};
struct ShipOrder
{
    char carrier[32];
    char tracking_id[64];
};
struct ConfirmDelivery
{
};
struct CancelOrder
{
    int reason_code;
};
struct RefundComplete
{
};

// Order state IDs
enum class OrderState : StateId {
    Pending,
    PaymentProcessing,
    PaymentConfirmed,
    Preparing,
    Shipped,
    Delivered,
    Completed,
    Cancelled,
    Refunding,
};

const char* order_state_name(OrderState s)
{
    switch (s) {
    case OrderState::Pending:
        return "PENDING";
    case OrderState::PaymentProcessing:
        return "PAYMENT_PROCESSING";
    case OrderState::PaymentConfirmed:
        return "PAYMENT_CONFIRMED";
    case OrderState::Preparing:
        return "PREPARING";
    case OrderState::Shipped:
        return "SHIPPED";
    case OrderState::Delivered:
        return "DELIVERED";
    case OrderState::Completed:
        return "COMPLETED";
    case OrderState::Cancelled:
        return "CANCELLED";
    case OrderState::Refunding:
        return "REFUNDING";
    }
    return "UNKNOWN";
}

// Order data
struct OrderData
{
    int order_id = 0;
    double amount = 0.0;
    int item_count = 0;
    double paid_amount = 0.0;
    char transaction_id[64]{};
    char carrier[32]{};
    char tracking_id[64]{};
    int cancel_reason = 0;
    int retry_count = 0;
};

class OrderProcessor
{
public:
    StateMachineEngine engine;
    OrderData order;
    ModuleLogger log{"OrderProcessor"};

    OrderProcessor() : log("OrderProcessor")
    {
        // Register states
        for (int i = 0; i <= static_cast<int>(OrderState::Refunding); i++) {
            auto s = static_cast<OrderState>(i);
            engine.register_state(static_cast<StateId>(s));
            engine.set_state_name(static_cast<StateId>(s), order_state_name(s));
        }

        engine.set_initial(static_cast<StateId>(OrderState::Pending));

        // Entry callbacks for observability
        engine.on_entry(
            static_cast<StateId>(OrderState::Pending), [this](const auto&) {
                log.info("Order %d: entered PENDING state", order.order_id);
            });
        engine.on_entry(static_cast<StateId>(OrderState::PaymentProcessing),
                        [this](const auto&) {
                            log.info("Order %d: processing payment of $%.2f",
                                     order.order_id, order.amount);
                        });
        engine.on_entry(static_cast<StateId>(OrderState::PaymentConfirmed),
                        [this](const auto&) {
                            log.info("Order %d: payment confirmed (tx: %s)",
                                     order.order_id, order.transaction_id);
                        });
        engine.on_entry(
            static_cast<StateId>(OrderState::Preparing), [this](const auto&) {
                log.info("Order %d: preparing %d items for shipment",
                         order.order_id, order.item_count);
            });
        engine.on_entry(
            static_cast<StateId>(OrderState::Shipped), [this](const auto&) {
                log.info("Order %d: shipped via %s (tracking: %s)",
                         order.order_id, order.carrier, order.tracking_id);
            });
        engine.on_entry(static_cast<StateId>(OrderState::Delivered),
                        [this](const auto&) {
                            log.info("Order %d: delivered!", order.order_id);
                        });
        engine.on_entry(static_cast<StateId>(OrderState::Completed),
                        [this](const auto&) {
                            log.info("Order %d: order completed successfully",
                                     order.order_id);
                        });
        engine.on_entry(static_cast<StateId>(OrderState::Cancelled),
                        [this](const auto&) {
                            log.info("Order %d: CANCELLED (reason: %d)",
                                     order.order_id, order.cancel_reason);
                        });

        // ---- Transitions ----
        auto sid = [](OrderState s) { return static_cast<StateId>(s); };

        // Pending -> PaymentProcessing (on place order)
        engine.add_rule({sid(OrderState::Pending),
                         typeid(PlaceOrder),
                         sid(OrderState::PaymentProcessing),
                         false,
                         false,
                         {},
                         [this](const EventEnvelope& evt) {
                             auto& e = evt.get<PlaceOrder>();
                             order.order_id = e.order_id;
                             order.amount = e.amount;
                             order.item_count = e.item_count;
                         }});

        // PaymentProcessing -> PaymentConfirmed (payment succeeds, with guard)
        engine.add_rule(
            {sid(OrderState::PaymentProcessing), typeid(PaymentReceived),
             sid(OrderState::PaymentConfirmed), false, false,
             [](const EventEnvelope& evt) -> bool {
                 auto& e = evt.get<PaymentReceived>();
                 return e.amount > 0;
             },
             [this](const EventEnvelope& evt) {
                 auto& e = evt.get<PaymentReceived>();
                 order.paid_amount = e.amount;
                 std::strncpy(order.transaction_id, e.transaction_id,
                              sizeof(order.transaction_id) - 1);
             }});

        // PaymentProcessing -> Cancelled (payment failed, with retry logic)
        engine.add_rule({sid(OrderState::PaymentProcessing),
                         typeid(PaymentFailed), sid(OrderState::Cancelled),
                         false, false,
                         [](const EventEnvelope& evt) -> bool {
                             auto& e = evt.get<PaymentFailed>();
                             return e.error_code >= 100; // Fatal errors
                         },
                         [this](const EventEnvelope& evt) {
                             auto& e = evt.get<PaymentFailed>();
                             log.error("Payment failed (fatal): %s", e.reason);
                         }});

        // PaymentProcessing -> Pending (retry on transient failure)
        engine.add_rule(
            {sid(OrderState::PaymentProcessing), typeid(PaymentFailed),
             sid(OrderState::Pending), false, false,
             [](const EventEnvelope& evt) -> bool {
                 auto& e = evt.get<PaymentFailed>();
                 return e.error_code < 100; // Transient errors
             },
             [this](const EventEnvelope& evt) {
                 auto& e = evt.get<PaymentFailed>();
                 order.retry_count++;
                 log.warn("Payment failed (transient, attempt %d): %s",
                          order.retry_count, e.reason);
             }});

        // PaymentConfirmed -> Preparing
        engine.add_rule({sid(OrderState::PaymentConfirmed),
                         typeid(InventoryReserved),
                         sid(OrderState::Preparing)});

        // PaymentConfirmed -> Cancelled (inventory short)
        engine.add_rule({sid(OrderState::PaymentConfirmed),
                         typeid(InventoryShort),
                         sid(OrderState::Cancelled),
                         false,
                         false,
                         {},
                         [this](const EventEnvelope&) {
                             log.error("Inventory short: cancelling order");
                             order.cancel_reason = 2;
                         }});

        // Preparing -> Shipped
        engine.add_rule({sid(OrderState::Preparing), typeid(ShipOrder),
                         sid(OrderState::Shipped), false, false,
                         [](const EventEnvelope& evt) -> bool {
                             auto& e = evt.get<ShipOrder>();
                             return std::strlen(e.tracking_id) > 0;
                         },
                         [this](const EventEnvelope& evt) {
                             auto& e = evt.get<ShipOrder>();
                             std::strncpy(order.carrier, e.carrier,
                                          sizeof(order.carrier) - 1);
                             std::strncpy(order.tracking_id, e.tracking_id,
                                          sizeof(order.tracking_id) - 1);
                         }});

        // Shipped -> Delivered
        engine.add_rule({sid(OrderState::Shipped), typeid(ConfirmDelivery),
                         sid(OrderState::Delivered)});

        // Delivered -> Completed
        engine.add_rule({sid(OrderState::Delivered),
                         typeid(ConfirmDelivery),
                         sid(OrderState::Completed),
                         false,
                         false,
                         {},
                         [this](const EventEnvelope&) {
                             log.info("Order %d: marked as completed",
                                      order.order_id);
                         }});

        // Cancel from many states
        for (auto s : {OrderState::Pending, OrderState::PaymentProcessing,
                       OrderState::PaymentConfirmed, OrderState::Preparing})
        {
            engine.add_rule({sid(s),
                             typeid(CancelOrder),
                             sid(OrderState::Cancelled),
                             false,
                             false,
                             {},
                             [this](const EventEnvelope& evt) {
                                 auto& e = evt.get<CancelOrder>();
                                 order.cancel_reason = e.reason_code;
                                 log.info(
                                     "Order %d: cancel requested (reason: %d)",
                                     order.order_id, e.reason_code);
                             }});
        }
    }

    void run_happy_path()
    {
        std::printf("\n=== E-Commerce: Happy Path ===\n\n");

        engine.handle(PlaceOrder{1001, 59.99, 3});
        engine.handle(PaymentReceived{59.99, "TXN-ABC-12345"});
        engine.handle(InventoryReserved{});
        engine.handle(ShipOrder{"FedEx", "FDX-9876-5432"});
        engine.handle(ConfirmDelivery{});
        engine.handle(ConfirmDelivery{});

        std::printf("\nFinal state: %s\n",
                    engine.get_state_name(engine.current_state()));
    }

    void run_payment_failure_path()
    {
        std::printf("\n=== E-Commerce: Payment Failure with Retry ===\n\n");
        engine.reset();

        engine.handle(PlaceOrder{1002, 129.99, 1});
        engine.handle(
            PaymentFailed{50, "Network timeout"}); // Transient -> retry
        engine.handle(
            PlaceOrder{1002, 129.99, 1}); // Re-enter payment processing
        engine.handle(PaymentReceived{129.99, "TXN-DEF-67890"});
        engine.handle(InventoryReserved{});
        engine.handle(ShipOrder{"UPS", "UPS-1234-5678"});
        engine.handle(ConfirmDelivery{});
        engine.handle(ConfirmDelivery{});

        std::printf("\nFinal state: %s (retries: %d)\n",
                    engine.get_state_name(engine.current_state()),
                    order.retry_count);
    }

    void run_cancellation_path()
    {
        std::printf("\n=== E-Commerce: Order Cancellation ===\n\n");
        engine.reset();

        engine.handle(PlaceOrder{1003, 9.99, 1});
        engine.handle(CancelOrder{1}); // Customer requested cancellation

        std::printf("\nFinal state: %s\n",
                    engine.get_state_name(engine.current_state()));
    }
};

int main()
{
    Logger::instance().set_level(LogLevel::Trace);

    OrderProcessor processor;
    processor.run_happy_path();
    processor.run_payment_failure_path();
    processor.run_cancellation_path();

    return 0;
}
