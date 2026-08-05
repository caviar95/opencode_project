#pragma once
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

#include "ddd/domain/core/id.hpp"
#include "ddd/domain/events/event_bus.hpp"
#include "ddd/domain/port/pubsub.hpp"
#include "ddd/domain/port/rpc.hpp"
#include "ddd/domain/process/model.hpp"

namespace ddd::domain::process {

// Aggregate root: one node in the multi-layer hierarchy.
//
// A node:
//  * belongs to a layer; controls the child layer below it via an RPC port,
//  * is "ready" only after walking through its plan (上电/预热/清洗/自检/到位), and
//  * publishes status/fault upward through an upstream port (逐级上报).
//
// It intentionally does NOT own its children: it knows only their Ids and
// reaches them through IRpc (down) and is reached upward through IUpstream.
// That models a real distributed deployment where layers are separate OS
// processes talking RPC (down) and Pub/Sub (up).
class HierNode : public port::IUpstream {
   public:
    HierNode(core::Id id, std::string name, int layer);

    // ---- setup (composition root wires these) ----
    void setPlan(std::vector<Step> plan) { plan_ = std::move(plan); }
    void setCriticalToJob(bool c) { criticalToJob_ = c; }
    void setRpc(port::IRpc* rpc) { rpc_ = rpc; }
    void setUpstream(port::IUpstream* up) { up_ = up; }
    void addChild(core::Id id) { childIds_.push_back(id); }
    // optional domain-event publisher (Alarm / ModuleStateChanged / ModuleReady)
    void setEventSink(events::IEventSink* sink) { sink_ = sink; }
    // optional observation hook (wired by composition root for diagnostics)
    void setTrace(std::function<void(const port::ProcessReport&)> trace) { trace_ = std::move(trace); }

    // ---- down-channel commands (a layer drives its children below) ----
    void advance();     // one step toward READY (may propagate RPC down)
    void startWork();   // READY -> WORK
    void finishWork();  // WORK -> DONE
    void fail(FaultLevel sev);  // inject / self-detected fault
    void clearFault();

    // ---- IUpstream: receives child status, aggregates, then reports upward ----
    void report(const port::ProcessReport& child) override;

    // ---- queries ----
    core::Id id() const { return id_; }
    const std::string& name() const { return name_; }
    int layer() const { return layer_; }
    Step step() const { return curState_; }
    bool ready() const { return ready_; }
    bool working() const { return working_; }
    FaultLevel faultLevel() const { return ownFault_; }
    bool affectsProduction() const { return blocking_; }
    bool anyChildBlocked() const { return childBlocked_; }
    bool inFault() const { return ownFault_ != FaultLevel::NONE; }

    bool isTop() const { return up_ == nullptr; }
    bool composite() const { return !childIds_.empty(); }

   private:
    void promoteSelf();          // advance one plan step
    bool allChildrenReady() const;
    void recomputeAndReport();     // refresh blocking / severity, publish upward
    port::ProcessReport buildReport() const;
    void publish(const port::ProcessReport& r) const;  // to up_
    void emit(events::EventKind kind, std::uint32_t from, std::uint32_t to,
              const char* msg) const;

    core::Id id_;
    std::string name_;
    int layer_;
    bool criticalToJob_{true};

    std::vector<Step> plan_;
    size_t pos_{0};
    Step curState_{Step::UNKNOWN};

    bool ready_{false};
    bool working_{false};

    FaultLevel ownFault_{FaultLevel::NONE};
    // Aggregated from children (via report()).
    std::map<core::Id, bool> childReady_;
    std::map<core::Id, bool> childBlock_;
    bool childBlocked_{false};
    // Decided by ImpactPolicy; may differ from own fault (告警但不阻断).
    bool blocking_{false};

    std::vector<core::Id> childIds_;

    // Ports (dependency-injected, infra-backed).
    port::IRpc* rpc_{nullptr};
    port::IUpstream* up_{nullptr};
    events::IEventSink* sink_{nullptr};

    // Last published state signature, to only emit on change (dedupe).
    std::uint64_t lastSig_{0xFFFFFFFFFFFFFFFFull};
    std::function<void(const port::ProcessReport&)> trace_;
};

}  // namespace ddd::domain::process