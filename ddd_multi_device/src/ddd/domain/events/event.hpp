#pragma once
#include <cstdint>
#include <string>

#include "ddd/domain/core/id.hpp"
#include "ddd/domain/process/model.hpp"

namespace ddd::domain::events {

// Domain events decouple the aggregates from the outside world (事件解耦扩展单位).
// Matches docs/01-requirement §6:
//   ModuleStateChanged{moduleId,from,to}
//   ModuleReady{moduleId}
//   LineStateChanged{lineState}
//   Alarm{moduleId,message}
enum class EventKind : std::uint8_t {
    ModuleStateChanged = 1,
    ModuleReady,
    LineStateChanged,
    Alarm,
    FaultCleared
};

inline const char* eventKindLabel(EventKind k) {
    switch (k) {
        case EventKind::ModuleStateChanged: return "ModuleStateChanged";
        case EventKind::ModuleReady: return "ModuleReady";
        case EventKind::LineStateChanged: return "LineStateChanged";
        case EventKind::Alarm: return "Alarm";
        case EventKind::FaultCleared: return "FaultCleared";
    }
    return "?";
}

// Value object / payload of a published domain event.
struct DomainEvent {
    EventKind kind{EventKind::ModuleStateChanged};
    core::Id moduleId;
    std::uint32_t from{0};   // 状态值(迁移前/旧状态或旧工序)
    std::uint32_t to{0};     // (迁移后/新状态)
    bool line{false};        // 是否产线级事件
    std::string message;

    std::string describe() const {
        return std::string(eventKindLabel(kind)) + " id=" + moduleId.toString() +
               " " + std::to_string(from) + "->" + std::to_string(to) +
               (message.empty() ? "" : " msg=" + message);
    }
};

}  // namespace ddd::domain::events