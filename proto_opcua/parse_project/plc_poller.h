#pragma once

#include "plc_client.h"
#include "node_cache.h"
#include "struct_parser.h"
#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <chrono>
#include <mutex>

struct PollItem {
    NodeKey key;
    std::string name;
    std::string description;
    bool is_struct{false};
};

struct PollGroup {
    std::string name;
    std::chrono::milliseconds interval{1000};
    std::vector<PollItem> items;
    bool enabled{true};
};

class PlcPoller {
public:
    PlcPoller(PlcClient& client, NodeCache& cache);
    ~PlcPoller();

    void addGroup(PollGroup group);
    void removeGroup(const std::string& name);
    void setGroupInterval(const std::string& name, std::chrono::milliseconds ms);
    void setGroupEnabled(const std::string& name, bool enabled);

    void start();
    void stop();
    bool running() const;
    void pollOnce(const std::string& group_name = "");
    void pollAll();

    void setOnPollDone(std::function<void(const std::string& group, size_t count)> cb);
    void setOnError(std::function<void(const NodeKey& key, uint32_t status)> cb);

private:
    void pollThread();
    void pollGroup(PollGroup& grp);
    CachedValue readNode(const PollItem& item);

    PlcClient& client_;
    NodeCache& cache_;
    std::vector<PollGroup> groups_;
    std::mutex groups_mtx_;
    std::thread poll_thread_;
    std::atomic<bool> running_{false};
    std::function<void(const std::string&, size_t)> on_poll_done_;
    std::function<void(const NodeKey&, uint32_t)> on_error_;
};
