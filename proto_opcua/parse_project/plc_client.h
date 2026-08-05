#pragma once

#include "struct_parser.h"
#include "node_cache.h"
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/types_generated.h>
#include <string>
#include <memory>

struct UA_ClientDeleter {
    void operator()(UA_Client* c) const { if (c) UA_Client_delete(c); }
};

class PlcClient {
public:
    explicit PlcClient(const std::string& endpoint);
    ~PlcClient();

    bool connect(int timeout_ms = 10000);
    void disconnect();
    bool connected() const;
    UA_Client* raw() const;

    CachedValue readNode(uint32_t ns, uint32_t id, const std::string& name = "");
    CachedValue readNodeString(uint32_t ns, const std::string& id,
                               const std::string& name = "");
    std::vector<CachedValue> batchRead(
        const std::vector<std::pair<uint32_t, uint32_t>>& nodes);

    std::vector<std::pair<uint32_t, uint32_t>> browseVariables(
        uint32_t ns = 0, uint32_t root_id = 85);

private:
    std::string endpoint_;
    std::unique_ptr<UA_Client, UA_ClientDeleter> client_;
};
