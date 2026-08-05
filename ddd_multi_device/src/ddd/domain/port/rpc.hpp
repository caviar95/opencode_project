#pragma once
#include "ddd/domain/core/id.hpp"
#include "ddd/domain/process/model.hpp"

namespace ddd::domain::port {

// ---- Downward RPC channel: a layer controls the child one level below.
// The receiving node responds by advancing through its plan / starting etc.
class IRpc {
   public:
    virtual ~IRpc() = default;

    // one readiness step toward READY (may be idempotent)
    virtual void advance(const core::Id& child) = 0;
    virtual void start(const core::Id& child) = 0;
    virtual void finish(const core::Id& child) = 0;
    virtual process::Step readStep(const core::Id& child) = 0;
};

}  // namespace ddd::domain::port