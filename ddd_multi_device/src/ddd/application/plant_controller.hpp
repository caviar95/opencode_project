#pragma once
#include <iostream>
#include <string>

#include "ddd/domain/core/id.hpp"
#include "ddd/domain/port/pubsub.hpp"
#include "ddd/domain/process/node.hpp"
#include "ddd/infrastructure/runtime/store.hpp"

namespace ddd::application {

// Application service + upstream sink for the 上位机 (host / SCADA).
// It drives the top node, and implements port::IUpstream so that the top
// node reports into it, closing the RPC-down / PubSub-up loop.
class PlantController : public domain::port::IUpstream {
   public:
    explicit PlantController(infrastructure::runtime::NodeStore* store) : store_(store) {}

    void runToReady(const domain::core::Id& root) {
        auto* n = store_->at(root);
        if (!n) return;
        int guard = 0;
        while (!n->ready() && guard++ < 100) n->advance();  // commanded via downlink
    }

    void startWork(const domain::core::Id& root) {
        if (auto* n = store_->at(root)) n->startWork();
    }
    void finishWork(const domain::core::Id& root) {
        if (auto* n = store_->at(root)) n->finishWork();
    }

    // Inject a fault into any node's PLC; it escalates automatically (逐级上报).
    void fail(const domain::core::Id& id, domain::process::FaultLevel sev) {
        if (auto* n = store_->at(id)) n->fail(sev);
    }
    void clearFault(const domain::core::Id& id) {
        if (auto* n = store_->at(id)) n->clearFault();
    }

    // ---- IUpstream sink for the top node ----
    void report(const domain::port::ProcessReport& e) override {
        std::cout << "    [上报 L" << e.layer << "] " << e.name
                  << " step=" << domain::process::stepLabel(e.step)
                  << " ready=" << (e.ready ? "Y" : "N")
                  << " fault=" << domain::process::faultLabel(e.fault)
                  << " affectsProduction=" << (e.affectsProduction ? "YES" : "no")
                  << " (" << e.reason << ")\n";
    }

    void dumpAll() const {
        for (auto& [id, node] : store_->all()) {
            std::cout << "  " << node->name() << " [L" << node->layer() << "] step="
                      << domain::process::stepLabel(node->step())
                      << " ready=" << (node->ready() ? "Y" : "N")
                      << " fault=" << domain::process::faultLabel(node->faultLevel())
                      << " affects=" << (node->affectsProduction() ? "Y" : "N")
                      << "\n";
        }
    }

   private:
    infrastructure::runtime::NodeStore* store_;
};

}  // namespace ddd::application