#pragma once

#include "struct_parser.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <chrono>
#include <functional>
#include <atomic>

struct NodeKey {
    uint32_t ns{0};
    std::string id;
    bool operator==(const NodeKey& o) const { return ns == o.ns && id == o.id; }
};

struct NodeKeyHash {
    size_t operator()(const NodeKey& k) const;
};

struct CachedValue {
    ParsedValue parsed;
    std::vector<uint8_t> raw_bytes;
    uint32_t status_code{0};
    uint64_t server_ts{0};
    uint64_t source_ts{0};
    uint64_t version{0};
    std::chrono::steady_clock::time_point update_time;
    bool valid() const { return status_code == 0; }
};

using CacheChangeCb = std::function<void(const NodeKey&, const CachedValue& old_val,
                                          const CachedValue& new_val)>;

class NodeCache {
public:
    explicit NodeCache(size_t max_size = 10000);

    void put(const NodeKey& key, CachedValue val);
    bool get(const NodeKey& key, CachedValue& out) const;
    bool has(const NodeKey& key) const;
    void remove(const NodeKey& key);
    void clear();

    std::vector<NodeKey> keys() const;
    size_t size() const;

    void setMaxSize(size_t max_size);
    void setOnChange(CacheChangeCb cb);
    uint64_t getVersion(const NodeKey& key) const;
    std::vector<NodeKey> getStaleKeys(
        std::chrono::steady_clock::duration max_age) const;
    void dump(std::ostream& os) const;

private:
    mutable std::shared_mutex mtx_;
    std::unordered_map<NodeKey, CachedValue, NodeKeyHash> data_;
    size_t max_size_{10000};
    CacheChangeCb on_change_;
    std::atomic<uint64_t> seq_{0};
};
