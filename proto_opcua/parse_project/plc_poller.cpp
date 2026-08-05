#include "plc_poller.h"
#include <iostream>
#include <thread>
#include <algorithm>

PlcPoller::PlcPoller(PlcClient& client, NodeCache& cache)
    : client_(client), cache_(cache) {}

PlcPoller::~PlcPoller() { stop(); }

void PlcPoller::addGroup(PollGroup group) {
    std::lock_guard lk(groups_mtx_);
    groups_.push_back(std::move(group));
}

void PlcPoller::removeGroup(const std::string& name) {
    std::lock_guard lk(groups_mtx_);
    groups_.erase(
        std::remove_if(groups_.begin(), groups_.end(),
            [&](const PollGroup& g) { return g.name == name; }),
        groups_.end());
}

void PlcPoller::setGroupInterval(const std::string& name,
                                  std::chrono::milliseconds ms) {
    std::lock_guard lk(groups_mtx_);
    for (auto& g : groups_) {
        if (g.name == name) { g.interval = ms; break; }
    }
}

void PlcPoller::setGroupEnabled(const std::string& name, bool enabled) {
    std::lock_guard lk(groups_mtx_);
    for (auto& g : groups_) {
        if (g.name == name) { g.enabled = enabled; break; }
    }
}

void PlcPoller::start() {
    if (running_) return;
    running_ = true;
    poll_thread_ = std::thread(&PlcPoller::pollThread, this);
}

void PlcPoller::stop() {
    running_ = false;
    if (poll_thread_.joinable()) poll_thread_.join();
}

bool PlcPoller::running() const { return running_; }

void PlcPoller::pollOnce(const std::string& group_name) {
    std::lock_guard lk(groups_mtx_);
    if (group_name.empty()) {
        for (auto& g : groups_)
            if (g.enabled) pollGroup(g);
    } else {
        for (auto& g : groups_)
            if (g.name == group_name && g.enabled) pollGroup(g);
    }
}

void PlcPoller::pollAll() { pollOnce(""); }

void PlcPoller::setOnPollDone(std::function<void(const std::string&, size_t)> cb) {
    on_poll_done_ = std::move(cb);
}

void PlcPoller::setOnError(std::function<void(const NodeKey&, uint32_t)> cb) {
    on_error_ = std::move(cb);
}

void PlcPoller::pollThread() {
    while (running_) {
        auto next = std::chrono::steady_clock::time_point::max();
        {
            std::lock_guard lk(groups_mtx_);
            auto now = std::chrono::steady_clock::now();
            for (auto& g : groups_) {
                if (!g.enabled || g.items.empty()) continue;
                pollGroup(g);
                auto wake = now + g.interval;
                if (wake < next) next = wake;
            }
        }
        if (next == std::chrono::steady_clock::time_point::max()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } else {
            std::this_thread::sleep_until(next);
        }
    }
}

void PlcPoller::pollGroup(PollGroup& grp) {
    size_t ok = 0;
    for (auto& item : grp.items) {
        CachedValue cv = readNode(item);
        if (cv.valid()) ok++;
        cache_.put(item.key, std::move(cv));
    }
    if (on_poll_done_) on_poll_done_(grp.name, ok);
}

CachedValue PlcPoller::readNode(const PollItem& item) {
    uint32_t num_id = 0;
    try { num_id = std::stoul(item.key.id); } catch (...) {}

    CachedValue cv;
    if (num_id > 0) {
        cv = client_.readNode(item.key.ns, num_id, item.name);
    } else {
        cv = client_.readNodeString(item.key.ns, item.key.id, item.name);
    }

    if (!cv.valid() && on_error_)
        on_error_(item.key, cv.status_code);
    return cv;
}
