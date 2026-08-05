#include "plc_client.h"
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <iostream>
#include <cstring>

PlcClient::PlcClient(const std::string& endpoint)
    : endpoint_(endpoint)
    , client_(nullptr, UA_ClientDeleter{}) {}

PlcClient::~PlcClient() { disconnect(); }

bool PlcClient::connect(int timeout_ms) {
    client_.reset(UA_Client_new());
    if (!client_) return false;
    UA_ClientConfig_setDefault(UA_Client_getConfig(client_.get()));
    UA_StatusCode st = UA_Client_connect(client_.get(), endpoint_.c_str());
    if (st != UA_STATUSCODE_GOOD) {
        std::cerr << "connect failed: " << UA_StatusCode_name(st) << "\n";
        client_.reset();
        return false;
    }
    return true;
}

void PlcClient::disconnect() {
    if (client_) {
        UA_Client_disconnect(client_.get());
        client_.reset();
    }
}

bool PlcClient::connected() const { return client_ != nullptr; }

UA_Client* PlcClient::raw() const { return client_.get(); }

CachedValue PlcClient::readNode(uint32_t ns, uint32_t id,
                                 const std::string& name) {
    CachedValue cv;
    cv.parsed.field_name = name;
    if (!client_) return cv;

    UA_ReadRequest req;
    UA_ReadRequest_init(&req);
    UA_ReadValueId rvid;
    UA_ReadValueId_init(&rvid);
    rvid.nodeId = UA_NODEID_NUMERIC(ns, id);
    rvid.attributeId = UA_ATTRIBUTEID_VALUE;
    req.nodesToRead = &rvid;
    req.nodesToReadSize = 1;

    UA_ReadResponse resp = UA_Client_Service_read(client_.get(), req);
    if (resp.responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
        cv.status_code = resp.responseHeader.serviceResult;
        UA_ReadResponse_clear(&resp);
        return cv;
    }
    if (resp.resultsSize > 0) {
        auto& dv = resp.results[0];
        cv.status_code = dv.status;
        cv.server_ts = dv.serverTimestamp;
        cv.source_ts = dv.sourceTimestamp;
        if (dv.value.type && dv.value.data) {
            const UA_DataType* dt = dv.value.type;
            cv.parsed = StructParser::parseValue(dv.value.data, dt, name);
        }
    }
    UA_ReadResponse_clear(&resp);
    return cv;
}

CachedValue PlcClient::readNodeString(uint32_t ns, const std::string& id,
                                       const std::string& name) {
    CachedValue cv;
    cv.parsed.field_name = name;
    if (!client_) return cv;

    UA_ReadRequest req;
    UA_ReadRequest_init(&req);
    UA_ReadValueId rvid;
    UA_ReadValueId_init(&rvid);
    rvid.nodeId = UA_NODEID_STRING_ALLOC(ns, id.c_str());
    rvid.attributeId = UA_ATTRIBUTEID_VALUE;
    req.nodesToRead = &rvid;
    req.nodesToReadSize = 1;

    UA_ReadResponse resp = UA_Client_Service_read(client_.get(), req);
    if (resp.responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
        cv.status_code = resp.responseHeader.serviceResult;
        UA_ReadResponse_clear(&resp);
        return cv;
    }
    if (resp.resultsSize > 0) {
        auto& dv = resp.results[0];
        cv.status_code = dv.status;
        cv.server_ts = dv.serverTimestamp;
        cv.source_ts = dv.sourceTimestamp;
        if (dv.value.type && dv.value.data) {
            const UA_DataType* dt = dv.value.type;
            cv.parsed = StructParser::parseValue(dv.value.data, dt, name);
        }
    }
    UA_ReadResponse_clear(&resp);
    return cv;
}

std::vector<CachedValue> PlcClient::batchRead(
    const std::vector<std::pair<uint32_t, uint32_t>>& nodes) {
    std::vector<CachedValue> results;
    if (!client_ || nodes.empty()) return results;

    size_t n = nodes.size();
    auto* rvids = static_cast<UA_ReadValueId*>(
        UA_Array_new(n, &UA_TYPES[UA_TYPES_READVALUEID]));
    for (size_t i = 0; i < n; ++i) {
        UA_ReadValueId_init(&rvids[i]);
        rvids[i].nodeId = UA_NODEID_NUMERIC(nodes[i].first, nodes[i].second);
        rvids[i].attributeId = UA_ATTRIBUTEID_VALUE;
    }

    UA_ReadRequest req;
    UA_ReadRequest_init(&req);
    req.nodesToRead = rvids;
    req.nodesToReadSize = n;

    UA_ReadResponse resp = UA_Client_Service_read(client_.get(), req);
    results.resize(n);
    for (size_t i = 0; i < n; ++i) {
        results[i].status_code = 0x80000000;
        if (i < resp.resultsSize) {
            auto& dv = resp.results[i];
            results[i].status_code = dv.status;
            results[i].server_ts = dv.serverTimestamp;
            results[i].source_ts = dv.sourceTimestamp;
            if (dv.value.type && dv.value.data) {
                results[i].parsed = StructParser::parseValue(
                    dv.value.data, dv.value.type);
            }
        }
    }
    UA_ReadResponse_clear(&resp);
    UA_Array_delete(rvids, n, &UA_TYPES[UA_TYPES_READVALUEID]);
    return results;
}

std::vector<std::pair<uint32_t, uint32_t>> PlcClient::browseVariables(
    uint32_t ns, uint32_t root_id) {
    std::vector<std::pair<uint32_t, uint32_t>> result;
    if (!client_) return result;

    UA_BrowseRequest req;
    UA_BrowseRequest_init(&req);
    UA_BrowseDescription desc;
    UA_BrowseDescription_init(&desc);
    desc.nodeId = UA_NODEID_NUMERIC(ns, root_id);
    desc.browseDirection = UA_BROWSEDIRECTION_FORWARD;
    desc.referenceTypeId = UA_NODEID_NUMERIC(0, 35);
    desc.includeSubtypes = true;
    desc.nodeClassMask = UA_NODECLASS_VARIABLE;
    desc.resultMask = UA_BROWSERESULTMASK_ALL;
    req.nodesToBrowse = &desc;
    req.nodesToBrowseSize = 1;

    UA_BrowseResponse resp = UA_Client_Service_browse(client_.get(), req);
    if (resp.responseHeader.serviceResult == UA_STATUSCODE_GOOD) {
        for (size_t i = 0; i < resp.resultsSize; ++i) {
            for (size_t j = 0; j < resp.results[i].referencesSize; ++j) {
                auto& ref = resp.results[i].references[j];
                if (ref.nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC) {
                    result.emplace_back(
                        ref.nodeId.nodeId.namespaceIndex,
                        ref.nodeId.nodeId.identifier.numeric);
                }
            }
        }
    }
    UA_BrowseResponse_clear(&resp);
    return result;
}
