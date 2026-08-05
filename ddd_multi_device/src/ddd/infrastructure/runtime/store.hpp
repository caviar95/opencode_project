#pragma once
#include <map>
#include <memory>

#include "ddd/domain/core/id.hpp"
#include "ddd/domain/port/rpc.hpp"
#include "ddd/domain/process/node.hpp"

namespace ddd::infrastructure::runtime {

// Deployment store: owns every node of a deployed hierarchy (each layer may be
// a separate embedded Linux process in the real system).
class NodeStore {
   public:
    void add(std::unique_ptr<domain::process::HierNode> node) {
        nodes_[node->id()] = std::move(node);
    }
    domain::process::HierNode* at(const domain::core::Id& id) {
        auto it = nodes_.find(id);
        return it == nodes_.end() ? nullptr : it->second.get();
    }
    const std::map<domain::core::Id, std::unique_ptr<domain::process::HierNode>>& all() const { return nodes_; }

   private:
    std::map<domain::core::Id, std::unique_ptr<domain::process::HierNode>> nodes_;
};

// IRpc adapter: routes a parent's downlink to the child node by Id,
// i.e. the "RPC between layers" transport (in-process, but behind the port).
class RpcRouter final : public domain::port::IRpc {
   public:
    explicit RpcRouter(NodeStore* store) : store_(store) {}

    void advance(const domain::core::Id& child) override {
        if (auto* n = store_->at(child)) n->advance();
    }
    void start(const domain::core::Id& child) override {
        if (auto* n = store_->at(child)) n->startWork();
    }
    void finish(const domain::core::Id& child) override {
        if (auto* n = store_->at(child)) n->finishWork();
    }
    domain::process::Step readStep(const domain::core::Id& child) override {
        auto* n = store_->at(child);
        return n ? n->step() : domain::process::Step::UNKNOWN;
    }

   private:
    NodeStore* store_;
};

}  // namespace ddd::infrastructure::runtime