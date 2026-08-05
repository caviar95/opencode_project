#include "opcua_client.h"
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/client_subscriptions.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/types.h>
#include <iostream>
#include <cstring>

OPCUAClient::OPCUAClient(std::string endpoint_url)
    : endpoint_url_(std::move(endpoint_url))
    , client_(nullptr, [](UA_Client* c) { if (c) UA_Client_delete(c); })
{
}

OPCUAClient::~OPCUAClient() {
    disconnect();
}

bool OPCUAClient::connect(int timeout_ms) {
    client_.reset(UA_Client_new());
    if (!client_) return false;

    UA_ClientConfig_setDefault(UA_Client_getConfig(client_.get()));
    auto status = UA_Client_connect(client_.get(), endpoint_url_.c_str());
    if (status != UA_STATUSCODE_GOOD) {
        std::cerr << "Connect failed: " << UA_StatusCode_name(status) << std::endl;
        client_.reset();
        return false;
    }
    return true;
}

void OPCUAClient::disconnect() {
    if (client_) {
        UA_Client_disconnect(client_.get());
    }
}

bool OPCUAClient::isConnected() const {
    return client_ != nullptr;
}

// ============ utility: NodeId conversion ============

opcua::NodeId OPCUAClient::numericNodeId(uint32_t ns, uint32_t id) {
    opcua::NodeId nid;
    nid.set_namespace_index(ns);
    nid.set_numeric_id(id);
    return nid;
}

opcua::NodeId OPCUAClient::stringNodeId(uint32_t ns, const std::string& id) {
    opcua::NodeId nid;
    nid.set_namespace_index(ns);
    nid.set_string_id(id);
    return nid;
}

UA_Client* OPCUAClient::toNative(const opcua::NodeId& pb) const {
    (void)pb;
    return client_.get();
}

// ============ Variant conversion helpers ============

opcua::VariantValue OPCUAClient::variantToProto(const void* ua_variant_ptr) const {
    opcua::VariantValue val;
    const auto* ua_val = static_cast<const UA_Variant*>(ua_variant_ptr);
    if (!ua_val || !ua_val->data) {
        val.set_type(opcua::DataTypeId::INT32);
        return val;
    }

    auto* type = ua_val->type;
    if (!type) return val;

    bool is_array = ua_val->arrayLength > 0 && ua_val->arrayLength != SIZE_MAX;

    // scalar reads
    if (type == &UA_TYPES[UA_TYPES_BOOLEAN]) {
        if (is_array) {
            val.set_type(opcua::DataTypeId::ARRAY_BOOL);
            auto* arr = static_cast<UA_Boolean*>(ua_val->data);
            for (size_t i = 0; i < ua_val->arrayLength; ++i)
                val.add_bool_array(arr[i] != 0);
        } else {
            val.set_type(opcua::DataTypeId::BOOL);
            val.set_bool_val(*static_cast<UA_Boolean*>(ua_val->data) != 0);
        }
    } else if (type == &UA_TYPES[UA_TYPES_SBYTE]) {
        val.set_type(opcua::DataTypeId::INT8);
        val.set_int32_val(*static_cast<UA_SByte*>(ua_val->data));
    } else if (type == &UA_TYPES[UA_TYPES_INT16]) {
        val.set_type(opcua::DataTypeId::INT16);
        val.set_int32_val(*static_cast<UA_Int16*>(ua_val->data));
    } else if (type == &UA_TYPES[UA_TYPES_INT32]) {
        if (is_array) {
            val.set_type(opcua::DataTypeId::ARRAY_INT32);
            auto* arr = static_cast<UA_Int32*>(ua_val->data);
            for (size_t i = 0; i < ua_val->arrayLength; ++i)
                val.add_int32_array(arr[i]);
        } else {
            val.set_type(opcua::DataTypeId::INT32);
            val.set_int32_val(*static_cast<UA_Int32*>(ua_val->data));
        }
    } else if (type == &UA_TYPES[UA_TYPES_INT64]) {
        val.set_type(opcua::DataTypeId::INT64);
        val.set_int64_val(*static_cast<UA_Int64*>(ua_val->data));
    } else if (type == &UA_TYPES[UA_TYPES_BYTE]) {
        val.set_type(opcua::DataTypeId::UINT8);
        val.set_uint32_val(*static_cast<UA_Byte*>(ua_val->data));
    } else if (type == &UA_TYPES[UA_TYPES_UINT16]) {
        val.set_type(opcua::DataTypeId::UINT16);
        val.set_uint32_val(*static_cast<UA_UInt16*>(ua_val->data));
    } else if (type == &UA_TYPES[UA_TYPES_UINT32]) {
        val.set_type(opcua::DataTypeId::UINT32);
        val.set_uint32_val(*static_cast<UA_UInt32*>(ua_val->data));
    } else if (type == &UA_TYPES[UA_TYPES_UINT64]) {
        val.set_type(opcua::DataTypeId::UINT64);
        val.set_uint64_val(*static_cast<UA_UInt64*>(ua_val->data));
    } else if (type == &UA_TYPES[UA_TYPES_FLOAT]) {
        if (is_array) {
            val.set_type(opcua::DataTypeId::ARRAY_FLOAT);
            auto* arr = static_cast<UA_Float*>(ua_val->data);
            for (size_t i = 0; i < ua_val->arrayLength; ++i)
                val.add_float_array(arr[i]);
        } else {
            val.set_type(opcua::DataTypeId::FLOAT);
            val.set_float_val(*static_cast<UA_Float*>(ua_val->data));
        }
    } else if (type == &UA_TYPES[UA_TYPES_DOUBLE]) {
        if (is_array) {
            val.set_type(opcua::DataTypeId::ARRAY_DOUBLE);
            auto* arr = static_cast<UA_Double*>(ua_val->data);
            for (size_t i = 0; i < ua_val->arrayLength; ++i)
                val.add_double_array(arr[i]);
        } else {
            val.set_type(opcua::DataTypeId::DOUBLE);
            val.set_double_val(*static_cast<UA_Double*>(ua_val->data));
        }
    } else if (type == &UA_TYPES[UA_TYPES_STRING]) {
        if (is_array) {
            val.set_type(opcua::DataTypeId::ARRAY_STRING);
            auto* arr = static_cast<UA_String*>(ua_val->data);
            for (size_t i = 0; i < ua_val->arrayLength; ++i) {
                auto* str = &arr[i];
                val.add_string_array(str->length > 0
                    ? std::string((const char*)str->data, str->length) : "");
            }
        } else {
            val.set_type(opcua::DataTypeId::STRING);
            auto* s = static_cast<UA_String*>(ua_val->data);
            if (s->length > 0)
                val.set_string_val(std::string((const char*)s->data, s->length));
        }
    } else if (type == &UA_TYPES[UA_TYPES_DATETIME]) {
        val.set_type(opcua::DataTypeId::DATETIME);
        auto dt = *static_cast<UA_DateTime*>(ua_val->data);
        val.set_datetime_val(static_cast<uint64_t>(dt));
    }

    return val;
}

void* OPCUAClient::protoToVariant(const opcua::VariantValue& val) const {
    auto* var = UA_Variant_new();
    switch (val.type()) {
    case opcua::DataTypeId::BOOL: {
        UA_Boolean v = val.bool_val();
        UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;
    }
    case opcua::DataTypeId::INT32: {
        UA_Int32 v = val.int32_val();
        UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_INT32]);
        break;
    }
    case opcua::DataTypeId::INT64: {
        UA_Int64 v = val.int64_val();
        UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_INT64]);
        break;
    }
    case opcua::DataTypeId::UINT32: {
        UA_UInt32 v = val.uint32_val();
        UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_UINT32]);
        break;
    }
    case opcua::DataTypeId::UINT64: {
        UA_UInt64 v = val.uint64_val();
        UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_UINT64]);
        break;
    }
    case opcua::DataTypeId::FLOAT: {
        UA_Float v = val.float_val();
        UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_FLOAT]);
        break;
    }
    case opcua::DataTypeId::DOUBLE: {
        UA_Double v = val.double_val();
        UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_DOUBLE]);
        break;
    }
    case opcua::DataTypeId::STRING: {
        auto s = UA_String_fromChars(val.string_val().c_str());
        UA_Variant_setScalarCopy(var, &s, &UA_TYPES[UA_TYPES_STRING]);
        UA_String_clear(&s);
        break;
    }
    case opcua::DataTypeId::ARRAY_INT32: {
        size_t n = val.int32_array_size();
        auto* arr = static_cast<UA_Int32*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_INT32]));
        for (size_t i = 0; i < n; ++i) arr[i] = val.int32_array(i);
        UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_INT32]);
        UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_INT32]);
        break;
    }
    case opcua::DataTypeId::ARRAY_FLOAT: {
        size_t n = val.float_array_size();
        auto* arr = static_cast<UA_Float*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_FLOAT]));
        for (size_t i = 0; i < n; ++i) arr[i] = val.float_array(i);
        UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_FLOAT]);
        UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_FLOAT]);
        break;
    }
    case opcua::DataTypeId::ARRAY_DOUBLE: {
        size_t n = val.double_array_size();
        auto* arr = static_cast<UA_Double*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_DOUBLE]));
        for (size_t i = 0; i < n; ++i) arr[i] = val.double_array(i);
        UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_DOUBLE]);
        UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_DOUBLE]);
        break;
    }
    case opcua::DataTypeId::ARRAY_BOOL: {
        size_t n = val.bool_array_size();
        auto* arr = static_cast<UA_Boolean*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_BOOLEAN]));
        for (size_t i = 0; i < n; ++i) arr[i] = val.bool_array(i);
        UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_BOOLEAN]);
        UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_BOOLEAN]);
        break;
    }
    case opcua::DataTypeId::ARRAY_STRING: {
        size_t n = val.string_array_size();
        auto* arr = static_cast<UA_String*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_STRING]));
        for (size_t i = 0; i < n; ++i)
            arr[i] = UA_String_fromChars(val.string_array(i).c_str());
        UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_STRING]);
        for (size_t i = 0; i < n; ++i) UA_String_clear(&arr[i]);
        UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_STRING]);
        break;
    }
    default:
        UA_Variant_delete(var);
        return nullptr;
    }
    return var;
}

// ============ READ ============

opcua::ReadResponse OPCUAClient::read(const opcua::ReadRequest& req) {
    opcua::ReadResponse resp;

    UA_ReadRequest rreq;
    UA_ReadRequest_init(&rreq);
    rreq.nodesToRead = UA_ReadValueId_new();
    rreq.nodesToReadSize = 1;
    rreq.nodesToRead[0].nodeId = UA_NODEID_NUMERIC(
        req.node_id().namespace_index(), req.node_id().numeric_id());
    rreq.nodesToRead[0].attributeId = req.attribute_id() != 0 ? req.attribute_id() : UA_ATTRIBUTEID_VALUE;

    if (req.node_id().id_case() == opcua::NodeId::IdCase::kStringId) {
        rreq.nodesToRead[0].nodeId = UA_NODEID_STRING_ALLOC(
            req.node_id().namespace_index(), req.node_id().string_id().c_str());
    }

    UA_ReadResponse raw_resp;
    auto status = UA_Client_Service_read(client_.get(), &rreq, &raw_resp);
    if (status == UA_STATUSCODE_GOOD && raw_resp.resultsSize > 0) {
        resp.mutable_status()->set_code(raw_resp.results[0].status);
        auto* sv = &raw_resp.results[0].value;
        if (sv->type) {
            *resp.mutable_value() = variantToProto(sv);
        }
        if (req.include_timestamp()) {
            resp.set_server_timestamp(raw_resp.results[0].serverTimestamp);
            resp.set_source_timestamp(raw_resp.results[0].sourceTimestamp);
        }
    } else {
        resp.mutable_status()->set_code(status);
    }

    UA_ReadRequest_clear(&rreq);
    UA_ReadResponse_clear(&raw_resp);
    return resp;
}

opcua::ReadResponse OPCUAClient::readValue(const opcua::NodeId& node) {
    opcua::ReadRequest req;
    *req.mutable_node_id() = node;
    req.set_attribute_id(UA_ATTRIBUTEID_VALUE);
    return read(req);
}

opcua::ReadResponse OPCUAClient::readAttribute(const opcua::NodeId& node, uint32_t attribute_id) {
    opcua::ReadRequest req;
    *req.mutable_node_id() = node;
    req.set_attribute_id(attribute_id);
    return read(req);
}

bool OPCUAClient::readBool(const opcua::NodeId& node) {
    auto resp = readValue(node);
    return resp.value().bool_val();
}

int32_t OPCUAClient::readInt32(const opcua::NodeId& node) {
    auto resp = readValue(node);
    return resp.value().int32_val();
}

int64_t OPCUAClient::readInt64(const opcua::NodeId& node) {
    auto resp = readValue(node);
    return resp.value().int64_val();
}

uint32_t OPCUAClient::readUInt32(const opcua::NodeId& node) {
    auto resp = readValue(node);
    return resp.value().uint32_val();
}

float OPCUAClient::readFloat(const opcua::NodeId& node) {
    auto resp = readValue(node);
    return resp.value().float_val();
}

double OPCUAClient::readDouble(const opcua::NodeId& node) {
    auto resp = readValue(node);
    return resp.value().double_val();
}

std::string OPCUAClient::readString(const opcua::NodeId& node) {
    auto resp = readValue(node);
    return resp.value().string_val();
}

std::vector<int32_t> OPCUAClient::readInt32Array(const opcua::NodeId& node) {
    auto resp = readValue(node);
    std::vector<int32_t> out;
    for (auto v : resp.value().int32_array()) out.push_back(v);
    return out;
}

std::vector<float> OPCUAClient::readFloatArray(const opcua::NodeId& node) {
    auto resp = readValue(node);
    std::vector<float> out;
    for (auto v : resp.value().float_array()) out.push_back(v);
    return out;
}

std::vector<double> OPCUAClient::readDoubleArray(const opcua::NodeId& node) {
    auto resp = readValue(node);
    std::vector<double> out;
    for (auto v : resp.value().double_array()) out.push_back(v);
    return out;
}

// ============ WRITE ============

uint32_t OPCUAClient::writeAttribute(const opcua::NodeId& node,
                                      const opcua::VariantValue& val,
                                      uint32_t attr_id) {
    UA_WriteRequest wreq;
    UA_WriteRequest_init(&wreq);
    wreq.nodesToWrite = UA_WriteValue_new();
    wreq.nodesToWriteSize = 1;
    wreq.nodesToWrite[0].nodeId = UA_NODEID_NUMERIC(
        node.namespace_index(), node.numeric_id());
    wreq.nodesToWrite[0].attributeId = attr_id;

    if (node.id_case() == opcua::NodeId::IdCase::kStringId) {
        wreq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(
            node.namespace_index(), node.string_id().c_str());
    }

    auto* var = static_cast<UA_Variant*>(protoToVariant(val));
    if (var) {
        UA_Variant_copy(var, &wreq.nodesToWrite[0].value.value);
        UA_Variant_delete(var);
    }

    UA_WriteResponse raw_resp;
    auto status = UA_Client_Service_write(client_.get(), &wreq, &raw_resp);
    UA_WriteRequest_clear(&wreq);
    UA_WriteResponse_clear(&raw_resp);
    return status;
}

opcua::WriteResponse OPCUAClient::write(const opcua::WriteRequest& req) {
    opcua::WriteResponse resp;
    auto status = writeAttribute(req.node_id(), req.value(),
                                 req.attribute_id() != 0 ? req.attribute_id() : UA_ATTRIBUTEID_VALUE);
    resp.mutable_status()->set_code(status);
    return resp;
}

bool OPCUAClient::writeBool(const opcua::NodeId& node, bool val) {
    opcua::VariantValue v;
    v.set_type(opcua::DataTypeId::BOOL);
    v.set_bool_val(val);
    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
}

bool OPCUAClient::writeInt32(const opcua::NodeId& node, int32_t val) {
    opcua::VariantValue v;
    v.set_type(opcua::DataTypeId::INT32);
    v.set_int32_val(val);
    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
}

bool OPCUAClient::writeInt64(const opcua::NodeId& node, int64_t val) {
    opcua::VariantValue v;
    v.set_type(opcua::DataTypeId::INT64);
    v.set_int64_val(val);
    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
}

bool OPCUAClient::writeUInt32(const opcua::NodeId& node, uint32_t val) {
    opcua::VariantValue v;
    v.set_type(opcua::DataTypeId::UINT32);
    v.set_uint32_val(val);
    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
}

bool OPCUAClient::writeFloat(const opcua::NodeId& node, float val) {
    opcua::VariantValue v;
    v.set_type(opcua::DataTypeId::FLOAT);
    v.set_float_val(val);
    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
}

bool OPCUAClient::writeDouble(const opcua::NodeId& node, double val) {
    opcua::VariantValue v;
    v.set_type(opcua::DataTypeId::DOUBLE);
    v.set_double_val(val);
    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
}

bool OPCUAClient::writeString(const opcua::NodeId& node, const std::string& val) {
    opcua::VariantValue v;
    v.set_type(opcua::DataTypeId::STRING);
    v.set_string_val(val);
    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
}

bool OPCUAClient::writeInt32Array(const opcua::NodeId& node, const std::vector<int32_t>& vals) {
    opcua::VariantValue v;
    v.set_type(opcua::DataTypeId::ARRAY_INT32);
    for (auto x : vals) v.add_int32_array(x);
    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
}

bool OPCUAClient::writeFloatArray(const opcua::NodeId& node, const std::vector<float>& vals) {
    opcua::VariantValue v;
    v.set_type(opcua::DataTypeId::ARRAY_FLOAT);
    for (auto x : vals) v.add_float_array(x);
    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
}

// ============ BATCH ============

opcua::BatchReadResponse OPCUAClient::batchRead(const opcua::BatchReadRequest& req) {
    opcua::BatchReadResponse resp;
    for (const auto& r : req.requests()) {
        *resp.add_responses() = read(r);
    }
    return resp;
}

opcua::BatchWriteResponse OPCUAClient::batchWrite(const opcua::BatchWriteRequest& req) {
    opcua::BatchWriteResponse resp;
    for (const auto& r : req.requests()) {
        *resp.add_responses() = write(r);
    }
    return resp;
}

// ============ BROWSE ============

opcua::BrowseResponse OPCUAClient::browse(const opcua::BrowseRequest& req) {
    opcua::BrowseResponse resp;

    UA_BrowseRequest breq;
    UA_BrowseRequest_init(&breq);
    breq.nodesToBrowse = UA_BrowseDescription_new();
    breq.nodesToBrowseSize = 1;
    breq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(
        req.node_id().namespace_index(), req.node_id().numeric_id());
    breq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;

    if (req.node_id().id_case() == opcua::NodeId::IdCase::kStringId) {
        breq.nodesToBrowse[0].nodeId = UA_NODEID_STRING_ALLOC(
            req.node_id().namespace_index(), req.node_id().string_id().c_str());
    }

    if (req.max_references() > 0)
        breq.requestedMaxReferencesPerNode = req.max_references();

    if (req.node_class_mask() > 0) {
        breq.nodesToBrowse[0].nodeClassMask = req.node_class_mask();
    }

    UA_BrowseResponse raw_resp;
    auto status = UA_Client_Service_browse(client_.get(), &breq, &raw_resp);
    if (status == UA_STATUSCODE_GOOD) {
        resp.mutable_status()->set_code(UA_STATUSCODE_GOOD);
        for (size_t i = 0; i < raw_resp.resultsSize; ++i) {
            for (size_t j = 0; j < raw_resp.results[i].referencesSize; ++j) {
                auto* ref = &raw_resp.results[i].references[j];
                auto* pb_ref = resp.add_references();
                // display name
                auto dn = ref->displayName;
                pb_ref->set_display_name(dn.length > 0
                    ? std::string((const char*)dn.data, dn.length) : "");
                // browse name
                auto bn = ref->browseName.name;
                pb_ref->set_browse_name(bn.length > 0
                    ? std::string((const char*)bn.data, bn.length) : "");
                pb_ref->set_node_class(ref->nodeClass);
                // node id
                if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC) {
                    pb_ref->mutable_node_id()->set_namespace_index(ref->nodeId.nodeId.namespaceIndex);
                    pb_ref->mutable_node_id()->set_numeric_id(ref->nodeId.nodeId.identifier.numeric);
                }
            }
        }
    } else {
        resp.mutable_status()->set_code(status);
    }

    UA_BrowseRequest_clear(&breq);
    UA_BrowseResponse_clear(&raw_resp);
    return resp;
}

std::vector<opcua::ReferenceDescription> OPCUAClient::browseChildren(const opcua::NodeId& node) {
    opcua::BrowseRequest req;
    *req.mutable_node_id() = node;
    auto resp = browse(req);
    return {resp.references().begin(), resp.references().end()};
}

std::vector<opcua::ReferenceDescription> OPCUAClient::browseObjects(const opcua::NodeId& node) {
    opcua::BrowseRequest req;
    *req.mutable_node_id() = node;
    req.set_node_class_mask(1); // Object
    auto resp = browse(req);
    return {resp.references().begin(), resp.references().end()};
}

std::vector<opcua::ReferenceDescription> OPCUAClient::browseVariables(const opcua::NodeId& node) {
    opcua::BrowseRequest req;
    *req.mutable_node_id() = node;
    req.set_node_class_mask(2); // Variable
    auto resp = browse(req);
    return {resp.references().begin(), resp.references().end()};
}

opcua::BrowseResponse OPCUAClient::browseRecursive(const opcua::NodeId& node, int depth) {
    opcua::BrowseResponse resp;
    if (depth <= 0) return resp;

    auto children = browseChildren(node);
    for (const auto& child : children) {
        *resp.add_references() = child;
        auto sub = browseRecursive(child.node_id(), depth - 1);
        for (const auto& ref : sub.references())
            *resp.add_references() = ref;
    }
    resp.mutable_status()->set_code(UA_STATUSCODE_GOOD);
    return resp;
}

// ============ SUBSCRIPTION ============

void OPCUAClient::dataChangeHandler(UA_Client* /*client*/,
                                     uint32_t /*sub_id*/,
                                     void* sub_ctx,
                                     uint32_t mon_id,
                                     void* mon_ctx,
                                     void* data_value) {
    auto* callbacks = static_cast<std::unordered_map<uint32_t, DataChangeCallback>*>(sub_ctx);
    auto* dv = static_cast<UA_DataValue*>(data_value);

    auto it = callbacks->find(mon_id);
    if (it != callbacks->end()) {
        opcua::DataChangeNotification notif;
        notif.set_monitored_item_id(mon_id);
        notif.mutable_status()->set_code(dv->status);

        if (dv->hasValue && dv->value.type) {
            auto self = static_cast<OPCUAClient*>(mon_ctx);
            *notif.mutable_value() = self->variantToProto(&dv->value);
        }
        it->second(notif);
    }
}

opcua::SubscribeResponse OPCUAClient::subscribe(const opcua::SubscribeRequest& req,
                                                  DataChangeCallback cb) {
    opcua::SubscribeResponse resp;

    // create subscription
    UA_CreateSubscriptionRequest sub_req = UA_CreateSubscriptionRequest_default();
    auto sub_resp = UA_Client_Subscriptions_create(client_.get(), sub_req,
                                                    nullptr, nullptr, nullptr);
    if (sub_resp.responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
        resp.mutable_status()->set_code(sub_resp.responseHeader.serviceResult);
        return resp;
    }
    resp.set_subscription_id(sub_resp.subscriptionId);

    // create monitored item
    UA_MonitoredItemCreateRequest mon_req = UA_MonitoredItemCreateRequest_default(
        UA_NODEID_NUMERIC(req.node_id().namespace_index(), req.node_id().numeric_id()));
    if (req.sampling_interval_ms() > 0)
        mon_req.requestedParameters.samplingInterval = req.sampling_interval_ms();
    if (req.queue_size() > 0)
        mon_req.requestedParameters.queueSize = req.queue_size();
    mon_req.requestedParameters.discardOldest = req.discard_oldest();

    auto mon_resp = UA_Client_Subscriptions_addMonitoredItem(
        client_.get(), sub_resp.subscriptionId, mon_req,
        &callbacks_, dataChangeHandler, this);
    if (mon_resp.statusCode != UA_STATUSCODE_GOOD) {
        resp.mutable_status()->set_code(mon_resp.statusCode);
        return resp;
    }
    resp.set_monitored_item_id(mon_resp.monitoredItemId);
    resp.mutable_status()->set_code(UA_STATUSCODE_GOOD);

    callbacks_[mon_resp.monitoredItemId] = std::move(cb);
    return resp;
}

opcua::UnsubscribeResponse OPCUAClient::unsubscribe(const opcua::UnsubscribeRequest& req) {
    opcua::UnsubscribeResponse resp;
    UA_Client_Subscriptions_removeMonitoredItem(
        client_.get(), req.subscription_id(), req.monitored_item_id());
    callbacks_.erase(req.monitored_item_id());
    resp.mutable_status()->set_code(UA_STATUSCODE_GOOD);
    return resp;
}

bool OPCUAClient::isSubscribed(uint32_t monitored_item_id) const {
    return callbacks_.find(monitored_item_id) != callbacks_.end();
}

// ============ METHOD CALL ============

opcua::CallMethodResponse OPCUAClient::callMethod(const opcua::CallMethodRequest& req) {
    opcua::CallMethodResponse resp;

    UA_CallMethodRequest cReq;
    UA_CallMethodRequest_init(&cReq);
    cReq.objectId = UA_NODEID_NUMERIC(
        req.object_id().namespace_index(), req.object_id().numeric_id());
    cReq.methodId = UA_NODEID_NUMERIC(
        req.method_id().namespace_index(), req.method_id().numeric_id());

    if (req.input_args_size() > 0) {
        cReq.inputArgumentsSize = req.input_args_size();
        cReq.inputArguments = static_cast<UA_Variant*>(
            UA_Array_new(cReq.inputArgumentsSize, &UA_TYPES[UA_TYPES_VARIANT]));
        for (size_t i = 0; i < cReq.inputArgumentsSize; ++i) {
            auto* var = static_cast<UA_Variant*>(protoToVariant(req.input_args(i)));
            if (var) {
                UA_Variant_copy(var, &cReq.inputArguments[i]);
                UA_Variant_delete(var);
            }
        }
    }

    UA_CallRequest call_req;
    UA_CallRequest_init(&call_req);
    call_req.methodsToCall = &cReq;
    call_req.methodsToCallSize = 1;

    UA_CallResponse call_resp;
    auto status = UA_Client_Service_call(client_.get(), &call_req, &call_resp);
    if (status == UA_STATUSCODE_GOOD && call_resp.resultsSize > 0) {
        auto* result = &call_resp.results[0];
        resp.mutable_status()->set_code(result->statusCode);
        for (size_t i = 0; i < result->outputArgumentsSize; ++i) {
            *resp.add_output_args() = variantToProto(&result->outputArguments[i]);
        }
    } else {
        resp.mutable_status()->set_code(status);
    }

    UA_CallRequest_clear(&call_req);
    UA_CallResponse_clear(&call_resp);
    return resp;
}

// ============ HISTORY READ ============

opcua::HistoryReadResponse OPCUAClient::historyReadRaw(const opcua::HistoryReadRequest& req) {
    opcua::HistoryReadResponse resp;

    UA_ReadRawModifiedDetails details;
    UA_ReadRawModifiedDetails_init(&details);
    details.startTime = req.start_time();
    details.endTime = req.end_time();
    details.numValuesPerNode = req.max_values() > 0 ? req.max_values() : 100;
    details.isReadModified = false;

    UA_ExtensionObject ext;
    UA_ExtensionObject_init(&ext);
    ext.encoding = UA_EXTENSIONOBJECT_DECODED;
    ext.content.decoded.type = &UA_TYPES[UA_TYPES_READRAWMODIFIEDDETAILS];
    ext.content.decoded.data = UA_new(&UA_TYPES[UA_TYPES_READRAWMODIFIEDDETAILS]);
    memcpy(ext.content.decoded.data, &details, sizeof(UA_ReadRawModifiedDetails));

    UA_HistoryReadRequest hreq;
    UA_HistoryReadRequest_init(&hreq);
    hreq.nodesToRead = UA_HistoryReadValueId_new();
    hreq.nodesToReadSize = 1;
    hreq.nodesToRead[0].nodeId = UA_NODEID_NUMERIC(
        req.node_id().namespace_index(), req.node_id().numeric_id());
    hreq.historyReadDetails = ext;

    UA_HistoryReadResponse hresp;
    auto status = UA_Client_Service_historyRead(client_.get(), &hreq, &hresp);
    if (status == UA_STATUSCODE_GOOD && hresp.resultsSize > 0) {
        resp.mutable_status()->set_code(hresp.results[0].statusCode);
        auto* data = &hresp.results[0].historyData;
        for (size_t i = 0; i < data->dataValuesSize; ++i) {
            auto* dv = &data->dataValues[i];
            if (dv->hasValue && dv->value.type) {
                *resp.add_values() = variantToProto(&dv->value);
            }
            resp.add_timestamps(dv->sourceTimestamp);
        }
    } else {
        resp.mutable_status()->set_code(status);
    }

    UA_HistoryReadRequest_clear(&hreq);
    UA_HistoryReadResponse_clear(&hresp);
    return resp;
}
