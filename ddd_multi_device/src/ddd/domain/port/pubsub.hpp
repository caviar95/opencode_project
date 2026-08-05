#pragma once
#include <string>

#include "ddd/domain/core/id.hpp"
#include "ddd/domain/process/model.hpp"

namespace ddd::domain::port {

// ---- Upward Pub/Sub channel: status & fault propagation to upper layers.
// Every node has an "upstream"; when its state changes (advance, fault, ready)
// it reports upward. The reports hop one layer at a time, so a leaf fault
// escalates to the line -- 逐级上报.
struct ProcessReport {
    core::Id origin;     // 源头节点
    int layer{0};        // 所在层级
    std::string name;
    process::Step step{process::Step::UNKNOWN};
    process::FaultLevel fault{process::FaultLevel::NONE};
    bool ready{false};
    bool blocking{false};       // 形装配是否影响生产(逐级聚合结果)
    bool affectsProduction{false};
    std::string reason;         // 影响生产时的判据说明
};

class IUpstream {
   public:
    virtual ~IUpstream() = default;
    virtual void report(const ProcessReport& event) = 0;
};

}  // namespace ddd::domain::port