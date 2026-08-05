#pragma once
#include <string>

#include "ddd/domain/process/model.hpp"

namespace ddd::domain::process {

// Value object deciding whether a fault affects production on this node.
//  - a hard hardware fault (BLOCKING) always stops production.
//  - a warning (ALARM) only stops production if the module is on the critical
//    path for the current job (e.g. a safety interlock or a quality gate).
struct Impact {
    bool affectsProduction{false};
    FaultLevel severity{FaultLevel::NONE};
    std::string reason;
};

inline Impact decideProductionImpact(FaultLevel sev, bool criticalToJob) {
    Impact result;
    result.severity = sev;
    switch (sev) {
        case FaultLevel::BLOCKING:
            result.affectsProduction = true;
            result.reason = "硬故障(BLOCKING)，影响生产";
            break;
        case FaultLevel::ALARM:
            result.affectsProduction = criticalToJob;
            result.reason = criticalToJob ? "安全/质量关键节点告警，影响生产" : "非关键告警，可继续生产";
            break;
        case FaultLevel::NONE:
            result.reason = "正常";
            break;
    }
    return result;
}

// Highest severity aggregator across a set of child nodes and the node itself.
inline FaultLevel aggregateFault(FaultLevel own, FaultLevel childWorst) {
    return worstOf(own, childWorst);
}

}  // namespace ddd::domain::process