#include "node_cache.h"
#include <algorithm>
#include <iomanip>

size_t NodeKeyHash::operator()(const NodeKey& k) const {
    size_t h = std::hash<uint32_t>{}(k.ns);
    h ^= std::hash<std::string>{}(k.id) << 1;
    return h;
}

NodeCache::NodeCache(size_t max_size) : max_size_(max_size) {}

void NodeCache::put(const NodeKey& key, CachedValue val) {
    val.version = ++seq_;
    val.update_time = std::chrono::steady_clock::now();
    std::unique_lock lk(mtx_);
    auto it = data_.find(key);
    if (it != data_.end()) {
        if (on_change_) {
            CachedValue old_val = it->second;
            on_change_(key, old_val, val);
        }
        it->second = std::move(val);
    } else {
        if (data_.size() >= max_size_) {
            auto oldest = data_.begin();
            for (auto it2 = data_.begin(); it2 != data_.end(); ++it2) {
                if (it2->second.update_time < oldest->second.update_time)
                    oldest = it2;
            }
            data_.erase(oldest);
        }
        if (on_change_) {
            CachedValue empty;
            on_change_(key, empty, val);
        }
        data_[key] = std::move(val);
    }
}

bool NodeCache::get(const NodeKey& key, CachedValue& out) const {
    std::shared_lock lk(mtx_);
    auto it = data_.find(key);
    if (it == data_.end()) return false;
    out = it->second;
    return true;
}

bool NodeCache::has(const NodeKey& key) const {
    std::shared_lock lk(mtx_);
    return data_.count(key) > 0;
}

void NodeCache::remove(const NodeKey& key) {
    std::unique_lock lk(mtx_);
    data_.erase(key);
}

void NodeCache::clear() {
    std::unique_lock lk(mtx_);
    data_.clear();
}

std::vector<NodeKey> NodeCache::keys() const {
    std::shared_lock lk(mtx_);
    std::vector<NodeKey> result;
    result.reserve(data_.size());
    for (auto& [k, v] : data_) result.push_back(k);
    return result;
}

size_t NodeCache::size() const {
    std::shared_lock lk(mtx_);
    return data_.size();
}

void NodeCache::setMaxSize(size_t max_size) {
    std::unique_lock lk(mtx_);
    max_size_ = max_size;
}

void NodeCache::setOnChange(CacheChangeCb cb) {
    std::unique_lock lk(mtx_);
    on_change_ = std::move(cb);
}

uint64_t NodeCache::getVersion(const NodeKey& key) const {
    std::shared_lock lk(mtx_);
    auto it = data_.find(key);
    return it != data_.end() ? it->second.version : 0;
}

std::vector<NodeKey> NodeCache::getStaleKeys(
    std::chrono::steady_clock::duration max_age) const {
    std::shared_lock lk(mtx_);
    auto now = std::chrono::steady_clock::now();
    std::vector<NodeKey> result;
    for (auto& [k, v] : data_) {
        if (now - v.update_time > max_age) result.push_back(k);
    }
    return result;
}

void NodeCache::dump(std::ostream& os) const {
    std::shared_lock lk(mtx_);
    os << "NodeCache: " << data_.size() << " entries\n";
    for (auto& [k, v] : data_) {
        os << "  ns=" << k.ns << " id=" << k.id
           << " ver=" << v.version
           << " status=" << v.status_code
           << " type=" << static_cast<int>(v.parsed.ft)
           << "\n";
    }
}
