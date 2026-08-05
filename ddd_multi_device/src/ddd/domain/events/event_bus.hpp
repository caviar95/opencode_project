#pragma once
#include "ddd/domain/events/event.hpp"

namespace ddd::domain::events {

// Outbound port: a sink for domain events. Defined in the domain layer so that
// aggregates depend only on this abstraction (依赖倒置). The implementation
// (e.g. infrastructure::messaging::EventBus) is injected at the composition root.
class IEventSink {
   public:
    virtual ~IEventSink() = default;
    virtual void publish(const DomainEvent& e) = 0;
};

}  // namespace ddd::domain::events