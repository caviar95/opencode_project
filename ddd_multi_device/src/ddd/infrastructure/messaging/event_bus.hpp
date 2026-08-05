#pragma once
#include <functional>
#include <mutex>
#include <vector>

#include "ddd/domain/events/event_bus.hpp"

namespace ddd::infrastructure::messaging {

// EventBus adapter: implements the domain's outbound event sink (IEventSink).
// Aggregates publish() here; subscribers (boss/SCADA, loggers, alarm sinks)
// register a callback and are invoked synchronously on publish.
class EventBus final : public ddd::domain::events::IEventSink {
   public:
    using Handler = std::function<void(const ddd::domain::events::DomainEvent&)>;

    void subscribe(Handler h) {
        std::lock_guard<std::mutex> lk(mu_);
        handlers_.push_back(std::move(h));
    }

    void publish(const ddd::domain::events::DomainEvent& e) override {
        std::vector<Handler> snapshot;
        {
            std::lock_guard<std::mutex> lk(mu_);
            snapshot = handlers_;
        }
        for (auto& h : snapshot) h(e);
    }

   private:
    std::mutex mu_;
    std::vector<Handler> handlers_;
};

}  // namespace ddd::infrastructure::messaging