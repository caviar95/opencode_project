#include "ddd/domain/process/node.hpp"

#include <algorithm>

#include "ddd/domain/process/impact.hpp"

namespace ddd::domain::process {

HierNode::HierNode(core::Id id, std::string name, int layer)
    : id_(id), name_(std::move(name)), layer_(layer) {}

// ---- down-channel commands ----------------------------------------------------

void HierNode::advance() {
    if (inFault() || blocking_) return;  // faulted / production-blocked halt

    // 1) Driver: children must become ready first (drive them via RPC down).
    if (composite() && !allChildrenReady()) {
        if (rpc_) {
            for (auto& c : childIds_) rpc_->advance(c);
        }
        return;  // wait one round for the children to catch up
    }

    // 2) Walk our own plan (上电→预热→清洗→自检→到位).
    auto from = (std::uint32_t)curState_;
    promoteSelf();
    if (curState_ != (Step)from)
        emit(events::EventKind::ModuleStateChanged, from, (std::uint32_t)curState_, "步进");

    // 3) READY when the whole plan is exhausted.
    if (pos_ >= plan_.size() && !ready_) {
        ready_ = true;
        curState_ = Step::READY;
        emit(events::EventKind::ModuleReady, from, (std::uint32_t)curState_, "计划完成");
    }
    recomputeAndReport();
}

void HierNode::startWork() {
    if (!ready_ || blocking_) return;
    working_ = true;
    curState_ = Step::WORK;
    if (rpc_) {
        for (auto& c : childIds_) rpc_->start(c);
    }
    recomputeAndReport();
}

void HierNode::finishWork() {
    if (!working_) return;
    working_ = false;
    curState_ = Step::DONE;
    if (rpc_) {
        for (auto& c : childIds_) rpc_->finish(c);
    }
    recomputeAndReport();
}

void HierNode::fail(FaultLevel sev) {
    bool wasFault = inFault();
    ownFault_ = worstOf(ownFault_, sev);
    if (!wasFault) {
        emit(events::EventKind::Alarm, (std::uint32_t)FaultLevel::NONE,
             (std::uint32_t)ownFault_, "节点故障(逐级上报)");
    }
    recomputeAndReport();
}

void HierNode::clearFault() {
    if (ownFault_ == FaultLevel::NONE) return;
    ownFault_ = FaultLevel::NONE;
    emit(events::EventKind::FaultCleared, (std::uint32_t)FaultLevel::BLOCKING,
         (std::uint32_t)FaultLevel::NONE, "故障清除");
    recomputeAndReport();
}

// ---- IUpstream: escalate child status, aggregate, and keep going up --------

void HierNode::report(const port::ProcessReport& child) {
    childReady_[child.origin] = child.ready;
    childBlock_[child.origin] = child.blocking;
    recomputeAndReport();
}

// ---- aggregation -------------------------------------------------------------

void HierNode::promoteSelf() {
    if (pos_ < plan_.size()) {
        curState_ = plan_[pos_];
        ++pos_;
    }
}

bool HierNode::allChildrenReady() const {
    for (auto& id : childIds_) {
        auto it = childReady_.find(id);
        if (it == childReady_.end() || !it->second) return false;
    }
    return true;
}

void HierNode::recomputeAndReport() {
    // Aggregate child blocking.
    bool childBlock = false;
    for (auto& kv : childBlock_) {
        if (kv.second) childBlock = true;
    }
    childBlocked_ = childBlock;

    // Own fault + Impact policy decides this node's production impact.
    Impact mine = decideProductionImpact(ownFault_, criticalToJob_);

    // A node blocks production if it blocks itself (policy) OR a child blocks.
    blocking_ = mine.affectsProduction || childBlock;

    publish(buildReport());
}

port::ProcessReport HierNode::buildReport() const {
    port::ProcessReport r;
    r.origin = id_;
    r.layer = layer_;
    r.name = name_;
    r.step = curState_;
    r.fault = ownFault_;
    r.ready = ready_;
    r.blocking = blocking_;
    r.affectsProduction = blocking_;

    Impact mine = decideProductionImpact(ownFault_, criticalToJob_);
    if (ownFault_ != FaultLevel::NONE) {
        r.reason = mine.reason;
    } else if (childBlocked_) {
        r.reason = "由子节点故障逐级上报，影响生产";
    } else {
        r.reason = "正常";
    }
    return r;
}

void HierNode::publish(const port::ProcessReport& r) const {
    // Dedupe: only emit when a meaningful field changed.
    std::uint64_t sig = ((std::uint64_t)r.step << 24) | ((std::uint64_t)r.fault << 16) |
                        ((std::uint64_t)r.ready << 8) | ((std::uint64_t)r.affectsProduction << 4) |
                        ((std::uint64_t)r.blocking);
    auto self = const_cast<HierNode*>(this);
    if (sig == self->lastSig_) return;
    self->lastSig_ = sig;

    if (trace_) trace_(r);
    if (up_) up_->report(r);
}

void HierNode::emit(events::EventKind kind, std::uint32_t from, std::uint32_t to,
                    const char* msg) const {
    if (!sink_) return;
    events::DomainEvent e;
    e.kind = kind;
    e.moduleId = id_;
    e.from = from;
    e.to = to;
    e.line = (layer_ == 0);
    e.message = msg ? msg : "";
    sink_->publish(e);
}

}  // namespace ddd::domain::process