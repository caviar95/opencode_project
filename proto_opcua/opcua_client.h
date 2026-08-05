#pragma once

#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include "opcua.pb.h"

struct UA_Client;
struct UA_ClientConfig;

using UA_ClientDeleter = void (*)(UA_Client*);

class OPCUAClient {
public:
    explicit OPCUAClient(std::string endpoint_url);
    ~OPCUAClient();

    // connection lifecycle
    bool connect(int timeout_ms = 10000);
    void disconnect();
    bool isConnected() const;

    // ==== READ SCENARIOS ====
    opcua::ReadResponse read(const opcua::ReadRequest& req);
    opcua::ReadResponse readValue(const opcua::NodeId& node);
    opcua::ReadResponse readAttribute(const opcua::NodeId& node, uint32_t attribute_id);

    // typed reads
    bool readBool(const opcua::NodeId& node);
    int32_t readInt32(const opcua::NodeId& node);
    int64_t readInt64(const opcua::NodeId& node);
    uint32_t readUInt32(const opcua::NodeId& node);
    float readFloat(const opcua::NodeId& node);
    double readDouble(const opcua::NodeId& node);
    std::string readString(const opcua::NodeId& node);
    std::vector<int32_t> readInt32Array(const opcua::NodeId& node);
    std::vector<float> readFloatArray(const opcua::NodeId& node);
    std::vector<double> readDoubleArray(const opcua::NodeId& node);

    // ===== WRITE SCENARIOS =====
    opcua::WriteResponse write(const opcua::WriteRequest& req);
    bool writeBool(const opcua::NodeId& node, bool val);
    bool writeInt32(const opcua::NodeId& node, int32_t val);
    bool writeInt64(const opcua::NodeId& node, int64_t val);
    bool writeUInt32(const opcua::NodeId& node, uint32_t val);
    bool writeFloat(const opcua::NodeId& node, float val);
    bool writeDouble(const opcua::NodeId& node, double val);
    bool writeString(const opcua::NodeId& node, const std::string& val);
    bool writeInt32Array(const opcua::NodeId& node, const std::vector<int32_t>& val);
    bool writeFloatArray(const opcua::NodeId& node, const std::vector<float>& val);

    // ===== BATCH SCENARIOS =====
    opcua::BatchReadResponse batchRead(const opcua::BatchReadRequest& req);
    opcua::BatchWriteResponse batchWrite(const opcua::BatchWriteRequest& req);

    // ===== BROWSE SCENARIOS =====
    opcua::BrowseResponse browse(const opcua::BrowseRequest& req);
    std::vector<opcua::ReferenceDescription> browseChildren(const opcua::NodeId& node);
    std::vector<opcua::ReferenceDescription> browseObjects(const opcua::NodeId& node);
    std::vector<opcua::ReferenceDescription> browseVariables(const opcua::NodeId& node);
    opcua::BrowseResponse browseRecursive(const opcua::NodeId& node, int depth = 3);

    // ===== SUBSCRIPTION SCENARIOS =====
    using DataChangeCallback = std::function<void(const opcua::DataChangeNotification&)>;
    opcua::SubscribeResponse subscribe(const opcua::SubscribeRequest& req, DataChangeCallback cb);
    opcua::UnsubscribeResponse unsubscribe(const opcua::UnsubscribeRequest& req);
    bool isSubscribed(uint32_t monitored_item_id) const;

    // ===== METHOD CALL SCENARIOS =====
    opcua::CallMethodResponse callMethod(const opcua::CallMethodRequest& req);

    // ===== HISTORY READ SCENARIOS =====
    opcua::HistoryReadResponse historyReadRaw(const opcua::HistoryReadRequest& req);

    UA_Client* getRaw() const { return client_.get(); }

    // ===== UTILITY =====
    static opcua::NodeId numericNodeId(uint32_t ns, uint32_t id);
    static opcua::NodeId stringNodeId(uint32_t ns, const std::string& id);

private:
    UA_Client* toNative(const opcua::NodeId& pb) const;
    opcua::NodeId fromNative(const UA_Client* node) const;
    opcua::VariantValue variantToProto(const void* ua_variant) const;
    void* protoToVariant(const opcua::VariantValue& val) const;
    uint32_t writeAttribute(const opcua::NodeId& node, const opcua::VariantValue& val, uint32_t attr_id);
    static void dataChangeHandler(UA_Client* client, uint32_t sub_id, void* sub_ctx,
                                  uint32_t mon_id, void* mon_ctx,
                                  void* data_value);

    std::string endpoint_url_;
    std::unique_ptr<UA_Client, UA_ClientDeleter> client_;
    std::unordered_map<uint32_t, DataChangeCallback> callbacks_;
};
