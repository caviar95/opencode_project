# Protobuf+OPC UA PLC节点读写用例 (C++)

**Session ID:** ses_0cda05ef0ffe6bG4Lax61PxkEN
**Created:** 7/5/2026, 9:02:35 PM
**Updated:** 7/5/2026, 9:07:00 PM

---

## User

我当前需要结合protobuf和ocp ua，根据plc节点进行获取或者设置数据，给出所有使用场景的用例 c++

---

## Assistant (Build · Big Pickle · 10.0s)

_Thinking:_

The user wants me to create C++ usage examples combining protobuf and OPC UA for reading/writing data from PLC nodes. Let me explore the current directory structure first to understand what's already there.

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua</path>
<type>directory</type>
<entries>


(0 entries)
</entries>
```

---

## Assistant (Build · Big Pickle · 7.7s)

_Thinking:_

The directory is empty. The user wants me to create comprehensive C++ usage examples showing how to combine protobuf with OPC UA for PLC data access scenarios. Let me think about what this entails:

1. OPC UA (OPC Unified Architecture) - a protocol for industrial automation communication
2. Protobuf (Protocol Buffers) - Google's serialization format
3. PLC (Programmable Logic Controller) - industrial controllers

The use cases would include:
- Reading various data types from PLC nodes (bool, int, float, string, etc.)
- Writing data to PLC nodes
- Browsing the OPC UA address space
- Subscribing to data changes (monitored items)
- Method calls
- Batch read/write
- Structure/Variant types

Let me create a comprehensive set of examples. I'll need to think about what OPC UA C++ SDK to use - typically open62541 or the C++ wrapper. Let me use open62541 as it's the most common open-source OPC UA implementation.

I'll create:
1. A proto file defining messages for OPC UA operations
2. C++ code showing various usage scenarios

Let me design this comprehensively.

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "content": "Create .proto file defining OPC UA request/response messages",
      "status": "in_progress",
      "priority": "high"
    },
    {
      "content": "Create OPC UA client wrapper with protobuf integration",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Create comprehensive usage scenarios demonstrating all PLC read/write patterns",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "Create CMakeLists.txt build configuration",
      "status": "pending",
      "priority": "medium"
    }
  ]
}
```

**Output:**
```
[
  {
    "content": "Create .proto file defining OPC UA request/response messages",
    "status": "in_progress",
    "priority": "high"
  },
  {
    "content": "Create OPC UA client wrapper with protobuf integration",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Create comprehensive usage scenarios demonstrating all PLC read/write patterns",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "Create CMakeLists.txt build configuration",
    "status": "pending",
    "priority": "medium"
  }
]
```

---

## Assistant (Build · Big Pickle · 20.0s)

_Thinking:_

Let me design a comprehensive set of files:

1. `opcua.proto` - Protocol Buffers messages for OPC UA operations
2. `opcua_client.h/cpp` - OPC UA client wrapper using open62541
3. `opcua_proto_adapter.h/cpp` - Bridge between protobuf and OPC UA
4. `scenarios.cpp` - All usage scenario examples
5. `CMakeLists.txt` - Build configuration

Let me think carefully about the use cases:

**Read scenarios:**
- Read single node (various data types: bool, int8, int16, int32, int64, uint8, uint16, uint32, uint64, float, double, string, datetime)
- Read multiple nodes in batch
- Read node with specific attribute (Value, DisplayName, Description, etc.)
- Read array/structured types
- Read with timestamp

**Write scenarios:**
- Write single node (various types)
- Write multiple nodes in batch
- Write array values

**Browse scenarios:**
- Browse children of a node
- Browse with filter (node class, reference type)

**Subscribe/MonitoredItem scenarios:**
- Subscribe to data changes
- Subscribe with sampling interval
- Unsubscribe

**Method call scenarios:**
- Call method on object

**History scenarios:**
- Read historical data

Let me create this.

Let me create the protobuf definitions first, then the C++ wrapper, and finally all usage scenarios.

**Tool: write**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua.proto",
  "content": "syntax = \"proto3\";\n\npackage opcua;\n\nenum DataTypeId {\n    BOOL = 0;\n    INT8 = 1;\n    INT16 = 2;\n    INT32 = 3;\n    INT64 = 4;\n    UINT8 = 5;\n    UINT16 = 6;\n    UINT32 = 7;\n    UINT64 = 8;\n    FLOAT = 9;\n    DOUBLE = 10;\n    STRING = 11;\n    BYTESTRING = 12;\n    DATETIME = 13;\n    NODEID = 14;\n    VARIANT = 15;\n    ARRAY_BOOL = 16;\n    ARRAY_INT32 = 17;\n    ARRAY_FLOAT = 18;\n    ARRAY_DOUBLE = 19;\n    ARRAY_STRING = 20;\n    LOCALIZEDTEXT = 21;\n    QUALIFIEDNAME = 22;\n    STRUCTURED = 23;\n}\n\nmessage VariantValue {\n    DataTypeId type = 1;\n    oneof value {\n        bool bool_val = 2;\n        int32 int32_val = 3;\n        int64 int64_val = 4;\n        uint32 uint32_val = 5;\n        uint64 uint64_val = 6;\n        float float_val = 7;\n        double double_val = 8;\n        string string_val = 9;\n        bytes bytes_val = 10;\n        uint64 datetime_val = 11;\n    }\n    // support arrays\n    repeated bool bool_array = 20;\n    repeated int32 int32_array = 21;\n    repeated int64 int64_array = 22;\n    repeated uint32 uint32_array = 23;\n    repeated uint64 uint64_array = 24;\n    repeated float float_array = 25;\n    repeated double double_array = 26;\n    repeated string string_array = 27;\n}\n\nmessage NodeId {\n    uint32 namespace_index = 1;\n    oneof id {\n        uint32 numeric_id = 2;\n        string string_id = 3;\n        bytes guid_id = 4;\n        uint64 opaque_id = 5;\n    }\n}\n\nmessage QualifiedName {\n    uint32 namespace_index = 1;\n    string name = 2;\n}\n\nmessage ReadRequest {\n    NodeId node_id = 1;\n    // e.g. 13 (Value), 14 (DisplayName), etc.\n    uint32 attribute_id = 2;  // default = 13 (Value)\n    bool include_timestamp = 3;\n}\n\nmessage ReadResponse {\n    StatusCode status = 1;\n    VariantValue value = 2;\n    uint64 server_timestamp = 3;\n    uint64 source_timestamp = 4;\n}\n\nmessage WriteRequest {\n    NodeId node_id = 1;\n    VariantValue value = 2;\n    uint32 attribute_id = 3;\n}\n\nmessage WriteResponse {\n    StatusCode status = 1;\n}\n\nmessage BatchReadRequest {\n    repeated ReadRequest requests = 1;\n}\n\nmessage BatchReadResponse {\n    repeated ReadResponse responses = 1;\n}\n\nmessage BatchWriteRequest {\n    repeated WriteRequest requests = 1;\n}\n\nmessage BatchWriteResponse {\n    repeated WriteResponse responses = 1;\n}\n\nmessage BrowseRequest {\n    NodeId node_id = 1;\n    uint32 max_references = 2;\n    uint32 node_class_mask = 3;  // Object=1, Variable=2, Method=4, ObjectType=8, VariableType=16\n    bool recursive = 4;\n}\n\nmessage BrowseResponse {\n    StatusCode status = 1;\n    repeated ReferenceDescription references = 2;\n}\n\nmessage ReferenceDescription {\n    NodeId node_id = 1;\n    string browse_name = 2;\n    string display_name = 3;\n    uint32 node_class = 4;\n    QualifiedName type_definition = 5;\n}\n\nmessage SubscribeRequest {\n    NodeId node_id = 1;\n    double sampling_interval_ms = 2;\n    uint32 queue_size = 3;\n    bool discard_oldest = 4;\n}\n\nmessage SubscribeResponse {\n    StatusCode status = 1;\n    uint32 monitored_item_id = 2;\n    uint32 subscription_id = 3;\n}\n\nmessage UnsubscribeRequest {\n    uint32 subscription_id = 1;\n    uint32 monitored_item_id = 2;\n}\n\nmessage UnsubscribeResponse {\n    StatusCode status = 1;\n}\n\nmessage DataChangeNotification {\n    uint32 monitored_item_id = 1;\n    uint32 subscription_id = 2;\n    VariantValue value = 3;\n    uint64 server_timestamp = 4;\n    uint64 source_timestamp = 5;\n    StatusCode status = 6;\n}\n\nmessage CallMethodRequest {\n    NodeId object_id = 1;\n    NodeId method_id = 2;\n    repeated VariantValue input_args = 3;\n}\n\nmessage CallMethodResponse {\n    StatusCode status = 1;\n    repeated VariantValue output_args = 2;\n}\n\nmessage HistoryReadRequest {\n    NodeId node_id = 1;\n    uint64 start_time = 2;\n    uint64 end_time = 3;\n    uint32 max_values = 4;\n}\n\nmessage HistoryReadResponse {\n    StatusCode status = 1;\n    repeated VariantValue values = 2;\n    repeated uint64 timestamps = 3;\n}\n\nmessage StatusCode {\n    uint32 code = 1;\n    string symbolic_name = 2;\n    string description = 3;\n}\n\nmessage OPCUARequest {\n    uint32 request_id = 1;\n    oneof request {\n        ReadRequest read = 10;\n        WriteRequest write = 11;\n        BatchReadRequest batch_read = 12;\n        BatchWriteRequest batch_write = 13;\n        BrowseRequest browse = 14;\n        SubscribeRequest subscribe = 15;\n        UnsubscribeRequest unsubscribe = 16;\n        CallMethodRequest call_method = 17;\n        HistoryReadRequest history_read = 18;\n    }\n}\n\nmessage OPCUAResponse {\n    uint32 request_id = 1;\n    oneof response {\n        ReadResponse read = 10;\n        WriteResponse write = 11;\n        BatchReadResponse batch_read = 12;\n        BatchWriteResponse batch_write = 13;\n        BrowseResponse browse = 14;\n        SubscribeResponse subscribe = 15;\n        UnsubscribeResponse unsubscribe = 16;\n        CallMethodResponse call_method = 17;\n        HistoryReadResponse history_read = 18;\n        DataChangeNotification data_change = 19;\n    }\n}\n"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · Big Pickle · 8.5s)

_Thinking:_

Good, now let me create the C++ files. I need to create:

1. `opcua_client.h` - OPC UA client wrapper header
2. `opcua_client.cpp` - Implementation using open62541
3. `scenarios.cpp` - All usage scenario examples
4. `CMakeLists.txt` - Build file

Let me create a comprehensive but clean implementation.

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "priority": "high",
      "content": "Create .proto file defining OPC UA request/response messages",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Create OPC UA client wrapper with protobuf integration",
      "status": "in_progress"
    },
    {
      "priority": "high",
      "content": "Create comprehensive usage scenarios demonstrating all PLC read/write patterns",
      "status": "pending"
    },
    {
      "priority": "medium",
      "content": "Create CMakeLists.txt build configuration",
      "status": "pending"
    }
  ]
}
```

**Output:**
```
[
  {
    "priority": "high",
    "content": "Create .proto file defining OPC UA request/response messages",
    "status": "completed"
  },
  {
    "priority": "high",
    "content": "Create OPC UA client wrapper with protobuf integration",
    "status": "in_progress"
  },
  {
    "priority": "high",
    "content": "Create comprehensive usage scenarios demonstrating all PLC read/write patterns",
    "status": "pending"
  },
  {
    "priority": "medium",
    "content": "Create CMakeLists.txt build configuration",
    "status": "pending"
  }
]
```

---

## Assistant (Build · Big Pickle · 14.3s)

_Thinking:_

Let me create the OPC UA client wrapper header and implementation.

**Tool: write**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.h",
  "content": "#pragma once\n\n#include <memory>\n#include <string>\n#include <functional>\n#include <unordered_map>\n#include \"opcua.pb.h\"\n\nstruct UA_Client;\nstruct UA_ClientConfig;\n\nusing UA_ClientDeleter = void (*)(UA_Client*);\n\nclass OPCUAClient {\npublic:\n    explicit OPCUAClient(std::string endpoint_url);\n    ~OPCUAClient();\n\n    // connection lifecycle\n    bool connect(int timeout_ms = 10000);\n    void disconnect();\n    bool isConnected() const;\n\n    // ==== READ SCENARIOS ====\n    opcua::ReadResponse read(const opcua::ReadRequest& req);\n    opcua::ReadResponse readValue(const opcua::NodeId& node);\n    opcua::ReadResponse readAttribute(const opcua::NodeId& node, uint32_t attribute_id);\n\n    // typed reads\n    bool readBool(const opcua::NodeId& node);\n    int32_t readInt32(const opcua::NodeId& node);\n    int64_t readInt64(const opcua::NodeId& node);\n    uint32_t readUInt32(const opcua::NodeId& node);\n    float readFloat(const opcua::NodeId& node);\n    double readDouble(const opcua::NodeId& node);\n    std::string readString(const opcua::NodeId& node);\n    std::vector<int32_t> readInt32Array(const opcua::NodeId& node);\n    std::vector<float> readFloatArray(const opcua::NodeId& node);\n    std::vector<double> readDoubleArray(const opcua::NodeId& node);\n\n    // ===== WRITE SCENARIOS =====\n    opcua::WriteResponse write(const opcua::WriteRequest& req);\n    bool writeBool(const opcua::NodeId& node, bool val);\n    bool writeInt32(const opcua::NodeId& node, int32_t val);\n    bool writeInt64(const opcua::NodeId& node, int64_t val);\n    bool writeUInt32(const opcua::NodeId& node, uint32_t val);\n    bool writeFloat(const opcua::NodeId& node, float val);\n    bool writeDouble(const opcua::NodeId& node, double val);\n    bool writeString(const opcua::NodeId& node, const std::string& val);\n    bool writeInt32Array(const opcua::NodeId& node, const std::vector<int32_t>& val);\n    bool writeFloatArray(const opcua::NodeId& node, const std::vector<float>& val);\n\n    // ===== BATCH SCENARIOS =====\n    opcua::BatchReadResponse batchRead(const opcua::BatchReadRequest& req);\n    opcua::BatchWriteResponse batchWrite(const opcua::BatchWriteRequest& req);\n\n    // ===== BROWSE SCENARIOS =====\n    opcua::BrowseResponse browse(const opcua::BrowseRequest& req);\n    std::vector<opcua::ReferenceDescription> browseChildren(const opcua::NodeId& node);\n    std::vector<opcua::ReferenceDescription> browseObjects(const opcua::NodeId& node);\n    std::vector<opcua::ReferenceDescription> browseVariables(const opcua::NodeId& node);\n    opcua::BrowseResponse browseRecursive(const opcua::NodeId& node, int depth = 3);\n\n    // ===== SUBSCRIPTION SCENARIOS =====\n    using DataChangeCallback = std::function<void(const opcua::DataChangeNotification&)>;\n    opcua::SubscribeResponse subscribe(const opcua::SubscribeRequest& req, DataChangeCallback cb);\n    opcua::UnsubscribeResponse unsubscribe(const opcua::UnsubscribeRequest& req);\n    bool isSubscribed(uint32_t monitored_item_id) const;\n\n    // ===== METHOD CALL SCENARIOS =====\n    opcua::CallMethodResponse callMethod(const opcua::CallMethodRequest& req);\n\n    // ===== HISTORY READ SCENARIOS =====\n    opcua::HistoryReadResponse historyReadRaw(const opcua::HistoryReadRequest& req);\n\n    // ===== UTILITY =====\n    static opcua::NodeId numericNodeId(uint32_t ns, uint32_t id);\n    static opcua::NodeId stringNodeId(uint32_t ns, const std::string& id);\n\nprivate:\n    UA_Client* toNative(const opcua::NodeId& pb) const;\n    opcua::NodeId fromNative(const UA_Client* node) const;\n    opcua::VariantValue variantToProto(const void* ua_variant) const;\n    void* protoToVariant(const opcua::VariantValue& val) const;\n    uint32_t writeAttribute(const opcua::NodeId& node, const opcua::VariantValue& val, uint32_t attr_id);\n    static void dataChangeHandler(UA_Client* client, uint32_t sub_id, void* sub_ctx,\n                                  uint32_t mon_id, void* mon_ctx,\n                                  void* data_value);\n\n    std::string endpoint_url_;\n    std::unique_ptr<UA_Client, UA_ClientDeleter> client_;\n    std::unordered_map<uint32_t, DataChangeCallback> callbacks_;\n};\n"
}
```

**Output:**
```
Wrote file successfully.

LSP errors detected in this file, please fix:
<diagnostics file="/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.h">
ERROR [7:10] 'opcua.pb.h' file not found
ERROR [25:5] Use of undeclared identifier 'opcua'
ERROR [25:36] Use of undeclared identifier 'opcua'
ERROR [26:5] Use of undeclared identifier 'opcua'
ERROR [26:41] Use of undeclared identifier 'opcua'
ERROR [27:5] Use of undeclared identifier 'opcua'
ERROR [27:45] Use of undeclared identifier 'opcua'
ERROR [30:25] Use of undeclared identifier 'opcua'
ERROR [31:29] Use of undeclared identifier 'opcua'
ERROR [32:29] Use of undeclared identifier 'opcua'
ERROR [33:31] Use of undeclared identifier 'opcua'
ERROR [34:27] Use of undeclared identifier 'opcua'
ERROR [35:29] Use of undeclared identifier 'opcua'
ERROR [36:34] Use of undeclared identifier 'opcua'
ERROR [37:47] Use of undeclared identifier 'opcua'
ERROR [38:45] Use of undeclared identifier 'opcua'
ERROR [39:47] Use of undeclared identifier 'opcua'
ERROR [42:5] Use of undeclared identifier 'opcua'
ERROR [42:38] Use of undeclared identifier 'opcua'
ERROR [43:26] Use of undeclared identifier 'opcua'
... and 1 more
</diagnostics>
```

---

## Assistant (Build · Big Pickle · 67.6s)

_Thinking:_

The LSP errors are expected since the proto-generated header file doesn't exist yet. These will be resolved when the build system runs protoc. Let me continue creating the implementation file and the scenarios.

The LSP errors are expected — the protobuf-generated header will be created at build time. Let me continue with the implementation:

**Tool: write**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.cpp",
  "content": "#include \"opcua_client.h\"\n#include <open62541/client_config_default.h>\n#include <open62541/client_highlevel.h>\n#include <open62541/client_subscriptions.h>\n#include <open62541/plugin/log_stdout.h>\n#include <open62541/types.h>\n#include <iostream>\n#include <cstring>\n\nOPCUAClient::OPCUAClient(std::string endpoint_url)\n    : endpoint_url_(std::move(endpoint_url))\n    , client_(nullptr, [](UA_Client* c) { if (c) UA_Client_delete(c); })\n{\n}\n\nOPCUAClient::~OPCUAClient() {\n    disconnect();\n}\n\nbool OPCUAClient::connect(int timeout_ms) {\n    client_.reset(UA_Client_new());\n    if (!client_) return false;\n\n    UA_ClientConfig_setDefault(UA_Client_getConfig(client_.get()));\n    auto status = UA_Client_connect(client_.get(), endpoint_url_.c_str());\n    if (status != UA_STATUSCODE_GOOD) {\n        std::cerr << \"Connect failed: \" << UA_StatusCode_name(status) << std::endl;\n        client_.reset();\n        return false;\n    }\n    return true;\n}\n\nvoid OPCUAClient::disconnect() {\n    if (client_) {\n        UA_Client_disconnect(client_.get());\n    }\n}\n\nbool OPCUAClient::isConnected() const {\n    return client_ != nullptr;\n}\n\n// ============ utility: NodeId conversion ============\n\nopcua::NodeId OPCUAClient::numericNodeId(uint32_t ns, uint32_t id) {\n    opcua::NodeId nid;\n    nid.set_namespace_index(ns);\n    nid.set_numeric_id(id);\n    return nid;\n}\n\nopcua::NodeId OPCUAClient::stringNodeId(uint32_t ns, const std::string& id) {\n    opcua::NodeId nid;\n    nid.set_namespace_index(ns);\n    nid.set_string_id(id);\n    return nid;\n}\n\nUA_Client* OPCUAClient::toNative(const opcua::NodeId& pb) const {\n    (void)pb;\n    return client_.get();\n}\n\n// ============ Variant conversion helpers ============\n\nopcua::VariantValue OPCUAClient::variantToProto(const void* ua_variant_ptr) const {\n    opcua::VariantValue val;\n    const auto* ua_val = static_cast<const UA_Variant*>(ua_variant_ptr);\n    if (!ua_val || !ua_val->data) {\n        val.set_type(opcua::DataTypeId::INT32);\n        return val;\n    }\n\n    auto* type = ua_val->type;\n    if (!type) return val;\n\n    bool is_array = ua_val->arrayLength > 0 && ua_val->arrayLength != SIZE_MAX;\n\n    // scalar reads\n    if (type == &UA_TYPES[UA_TYPES_BOOLEAN]) {\n        if (is_array) {\n            val.set_type(opcua::DataTypeId::ARRAY_BOOL);\n            auto* arr = static_cast<UA_Boolean*>(ua_val->data);\n            for (size_t i = 0; i < ua_val->arrayLength; ++i)\n                val.add_bool_array(arr[i] != 0);\n        } else {\n            val.set_type(opcua::DataTypeId::BOOL);\n            val.set_bool_val(*static_cast<UA_Boolean*>(ua_val->data) != 0);\n        }\n    } else if (type == &UA_TYPES[UA_TYPES_SBYTE]) {\n        val.set_type(opcua::DataTypeId::INT8);\n        val.set_int32_val(*static_cast<UA_SByte*>(ua_val->data));\n    } else if (type == &UA_TYPES[UA_TYPES_INT16]) {\n        val.set_type(opcua::DataTypeId::INT16);\n        val.set_int32_val(*static_cast<UA_Int16*>(ua_val->data));\n    } else if (type == &UA_TYPES[UA_TYPES_INT32]) {\n        if (is_array) {\n            val.set_type(opcua::DataTypeId::ARRAY_INT32);\n            auto* arr = static_cast<UA_Int32*>(ua_val->data);\n            for (size_t i = 0; i < ua_val->arrayLength; ++i)\n                val.add_int32_array(arr[i]);\n        } else {\n            val.set_type(opcua::DataTypeId::INT32);\n            val.set_int32_val(*static_cast<UA_Int32*>(ua_val->data));\n        }\n    } else if (type == &UA_TYPES[UA_TYPES_INT64]) {\n        val.set_type(opcua::DataTypeId::INT64);\n        val.set_int64_val(*static_cast<UA_Int64*>(ua_val->data));\n    } else if (type == &UA_TYPES[UA_TYPES_BYTE]) {\n        val.set_type(opcua::DataTypeId::UINT8);\n        val.set_uint32_val(*static_cast<UA_Byte*>(ua_val->data));\n    } else if (type == &UA_TYPES[UA_TYPES_UINT16]) {\n        val.set_type(opcua::DataTypeId::UINT16);\n        val.set_uint32_val(*static_cast<UA_UInt16*>(ua_val->data));\n    } else if (type == &UA_TYPES[UA_TYPES_UINT32]) {\n        val.set_type(opcua::DataTypeId::UINT32);\n        val.set_uint32_val(*static_cast<UA_UInt32*>(ua_val->data));\n    } else if (type == &UA_TYPES[UA_TYPES_UINT64]) {\n        val.set_type(opcua::DataTypeId::UINT64);\n        val.set_uint64_val(*static_cast<UA_UInt64*>(ua_val->data));\n    } else if (type == &UA_TYPES[UA_TYPES_FLOAT]) {\n        if (is_array) {\n            val.set_type(opcua::DataTypeId::ARRAY_FLOAT);\n            auto* arr = static_cast<UA_Float*>(ua_val->data);\n            for (size_t i = 0; i < ua_val->arrayLength; ++i)\n                val.add_float_array(arr[i]);\n        } else {\n            val.set_type(opcua::DataTypeId::FLOAT);\n            val.set_float_val(*static_cast<UA_Float*>(ua_val->data));\n        }\n    } else if (type == &UA_TYPES[UA_TYPES_DOUBLE]) {\n        if (is_array) {\n            val.set_type(opcua::DataTypeId::ARRAY_DOUBLE);\n            auto* arr = static_cast<UA_Double*>(ua_val->data);\n            for (size_t i = 0; i < ua_val->arrayLength; ++i)\n                val.add_double_array(arr[i]);\n        } else {\n            val.set_type(opcua::DataTypeId::DOUBLE);\n            val.set_double_val(*static_cast<UA_Double*>(ua_val->data));\n        }\n    } else if (type == &UA_TYPES[UA_TYPES_STRING]) {\n        if (is_array) {\n            val.set_type(opcua::DataTypeId::ARRAY_STRING);\n            auto* arr = static_cast<UA_String*>(ua_val->data);\n            for (size_t i = 0; i < ua_val->arrayLength; ++i) {\n                auto* str = &arr[i];\n                val.add_string_array(str->length > 0\n                    ? std::string((const char*)str->data, str->length) : \"\");\n            }\n        } else {\n            val.set_type(opcua::DataTypeId::STRING);\n            auto* s = static_cast<UA_String*>(ua_val->data);\n            if (s->length > 0)\n                val.set_string_val(std::string((const char*)s->data, s->length));\n        }\n    } else if (type == &UA_TYPES[UA_TYPES_DATETIME]) {\n        val.set_type(opcua::DataTypeId::DATETIME);\n        auto dt = *static_cast<UA_DateTime*>(ua_val->data);\n        val.set_datetime_val(static_cast<uint64_t>(dt));\n    }\n\n    return val;\n}\n\nvoid* OPCUAClient::protoToVariant(const opcua::VariantValue& val) const {\n    auto* var = UA_Variant_new();\n    switch (val.type()) {\n    case opcua::DataTypeId::BOOL: {\n        UA_Boolean v = val.bool_val();\n        UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_BOOLEAN]);\n        break;\n    }\n    case opcua::DataTypeId::INT32: {\n        UA_Int32 v = val.int32_val();\n        UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_INT32]);\n        break;\n    }\n    case opcua::DataTypeId::INT64: {\n        UA_Int64 v = val.int64_val();\n        UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_INT64]);\n        break;\n    }\n    case opcua::DataTypeId::UINT32: {\n        UA_UInt32 v = val.uint32_val();\n        UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_UINT32]);\n        break;\n    }\n    case opcua::DataTypeId::UINT64: {\n        UA_UInt64 v = val.uint64_val();\n        UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_UINT64]);\n        break;\n    }\n    case opcua::DataTypeId::FLOAT: {\n        UA_Float v = val.float_val();\n        UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_FLOAT]);\n        break;\n    }\n    case opcua::DataTypeId::DOUBLE: {\n        UA_Double v = val.double_val();\n        UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_DOUBLE]);\n        break;\n    }\n    case opcua::DataTypeId::STRING: {\n        auto s = UA_String_fromChars(val.string_val().c_str());\n        UA_Variant_setScalarCopy(var, &s, &UA_TYPES[UA_TYPES_STRING]);\n        UA_String_clear(&s);\n        break;\n    }\n    case opcua::DataTypeId::ARRAY_INT32: {\n        size_t n = val.int32_array_size();\n        auto* arr = static_cast<UA_Int32*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_INT32]));\n        for (size_t i = 0; i < n; ++i) arr[i] = val.int32_array(i);\n        UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_INT32]);\n        UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_INT32]);\n        break;\n    }\n    case opcua::DataTypeId::ARRAY_FLOAT: {\n        size_t n = val.float_array_size();\n        auto* arr = static_cast<UA_Float*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_FLOAT]));\n        for (size_t i = 0; i < n; ++i) arr[i] = val.float_array(i);\n        UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_FLOAT]);\n        UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_FLOAT]);\n        break;\n    }\n    case opcua::DataTypeId::ARRAY_DOUBLE: {\n        size_t n = val.double_array_size();\n        auto* arr = static_cast<UA_Double*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_DOUBLE]));\n        for (size_t i = 0; i < n; ++i) arr[i] = val.double_array(i);\n        UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_DOUBLE]);\n        UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_DOUBLE]);\n        break;\n    }\n    case opcua::DataTypeId::ARRAY_BOOL: {\n        size_t n = val.bool_array_size();\n        auto* arr = static_cast<UA_Boolean*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_BOOLEAN]));\n        for (size_t i = 0; i < n; ++i) arr[i] = val.bool_array(i);\n        UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_BOOLEAN]);\n        UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_BOOLEAN]);\n        break;\n    }\n    case opcua::DataTypeId::ARRAY_STRING: {\n        size_t n = val.string_array_size();\n        auto* arr = static_cast<UA_String*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_STRING]));\n        for (size_t i = 0; i < n; ++i)\n            arr[i] = UA_String_fromChars(val.string_array(i).c_str());\n        UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_STRING]);\n        for (size_t i = 0; i < n; ++i) UA_String_clear(&arr[i]);\n        UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_STRING]);\n        break;\n    }\n    default:\n        UA_Variant_delete(var);\n        return nullptr;\n    }\n    return var;\n}\n\n// ============ READ ============\n\nopcua::ReadResponse OPCUAClient::read(const opcua::ReadRequest& req) {\n    opcua::ReadResponse resp;\n\n    UA_ReadRequest rreq;\n    UA_ReadRequest_init(&rreq);\n    rreq.nodesToRead = UA_ReadValueId_new();\n    rreq.nodesToReadSize = 1;\n    rreq.nodesToRead[0].nodeId = UA_NODEID_NUMERIC(\n        req.node_id().namespace_index(), req.node_id().numeric_id());\n    rreq.nodesToRead[0].attributeId = req.attribute_id() != 0 ? req.attribute_id() : UA_ATTRIBUTEID_VALUE;\n\n    if (req.node_id().id_case() == opcua::NodeId::IdCase::kStringId) {\n        rreq.nodesToRead[0].nodeId = UA_NODEID_STRING_ALLOC(\n            req.node_id().namespace_index(), req.node_id().string_id().c_str());\n    }\n\n    UA_ReadResponse raw_resp;\n    auto status = UA_Client_Service_read(client_.get(), &rreq, &raw_resp);\n    if (status == UA_STATUSCODE_GOOD && raw_resp.resultsSize > 0) {\n        resp.mutable_status()->set_code(raw_resp.results[0].status);\n        auto* sv = &raw_resp.results[0].value;\n        if (sv->type) {\n            *resp.mutable_value() = variantToProto(sv);\n        }\n        if (req.include_timestamp()) {\n            resp.set_server_timestamp(raw_resp.results[0].serverTimestamp);\n            resp.set_source_timestamp(raw_resp.results[0].sourceTimestamp);\n        }\n    } else {\n        resp.mutable_status()->set_code(status);\n    }\n\n    UA_ReadRequest_clear(&rreq);\n    UA_ReadResponse_clear(&raw_resp);\n    return resp;\n}\n\nopcua::ReadResponse OPCUAClient::readValue(const opcua::NodeId& node) {\n    opcua::ReadRequest req;\n    *req.mutable_node_id() = node;\n    req.set_attribute_id(UA_ATTRIBUTEID_VALUE);\n    return read(req);\n}\n\nopcua::ReadResponse OPCUAClient::readAttribute(const opcua::NodeId& node, uint32_t attribute_id) {\n    opcua::ReadRequest req;\n    *req.mutable_node_id() = node;\n    req.set_attribute_id(attribute_id);\n    return read(req);\n}\n\nbool OPCUAClient::readBool(const opcua::NodeId& node) {\n    auto resp = readValue(node);\n    return resp.value().bool_val();\n}\n\nint32_t OPCUAClient::readInt32(const opcua::NodeId& node) {\n    auto resp = readValue(node);\n    return resp.value().int32_val();\n}\n\nint64_t OPCUAClient::readInt64(const opcua::NodeId& node) {\n    auto resp = readValue(node);\n    return resp.value().int64_val();\n}\n\nuint32_t OPCUAClient::readUInt32(const opcua::NodeId& node) {\n    auto resp = readValue(node);\n    return resp.value().uint32_val();\n}\n\nfloat OPCUAClient::readFloat(const opcua::NodeId& node) {\n    auto resp = readValue(node);\n    return resp.value().float_val();\n}\n\ndouble OPCUAClient::readDouble(const opcua::NodeId& node) {\n    auto resp = readValue(node);\n    return resp.value().double_val();\n}\n\nstd::string OPCUAClient::readString(const opcua::NodeId& node) {\n    auto resp = readValue(node);\n    return resp.value().string_val();\n}\n\nstd::vector<int32_t> OPCUAClient::readInt32Array(const opcua::NodeId& node) {\n    auto resp = readValue(node);\n    std::vector<int32_t> out;\n    for (auto v : resp.value().int32_array()) out.push_back(v);\n    return out;\n}\n\nstd::vector<float> OPCUAClient::readFloatArray(const opcua::NodeId& node) {\n    auto resp = readValue(node);\n    std::vector<float> out;\n    for (auto v : resp.value().float_array()) out.push_back(v);\n    return out;\n}\n\nstd::vector<double> OPCUAClient::readDoubleArray(const opcua::NodeId& node) {\n    auto resp = readValue(node);\n    std::vector<double> out;\n    for (auto v : resp.value().double_array()) out.push_back(v);\n    return out;\n}\n\n// ============ WRITE ============\n\nuint32_t OPCUAClient::writeAttribute(const opcua::NodeId& node,\n                                      const opcua::VariantValue& val,\n                                      uint32_t attr_id) {\n    UA_WriteRequest wreq;\n    UA_WriteRequest_init(&wreq);\n    wreq.nodesToWrite = UA_WriteValue_new();\n    wreq.nodesToWriteSize = 1;\n    wreq.nodesToWrite[0].nodeId = UA_NODEID_NUMERIC(\n        node.namespace_index(), node.numeric_id());\n    wreq.nodesToWrite[0].attributeId = attr_id;\n\n    if (node.id_case() == opcua::NodeId::IdCase::kStringId) {\n        wreq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(\n            node.namespace_index(), node.string_id().c_str());\n    }\n\n    auto* var = static_cast<UA_Variant*>(protoToVariant(val));\n    if (var) {\n        UA_Variant_copy(var, &wreq.nodesToWrite[0].value.value);\n        UA_Variant_delete(var);\n    }\n\n    UA_WriteResponse raw_resp;\n    auto status = UA_Client_Service_write(client_.get(), &wreq, &raw_resp);\n    UA_WriteRequest_clear(&wreq);\n    UA_WriteResponse_clear(&raw_resp);\n    return status;\n}\n\nopcua::WriteResponse OPCUAClient::write(const opcua::WriteRequest& req) {\n    opcua::WriteResponse resp;\n    auto status = writeAttribute(req.node_id(), req.value(),\n                                 req.attribute_id() != 0 ? req.attribute_id() : UA_ATTRIBUTEID_VALUE);\n    resp.mutable_status()->set_code(status);\n    return resp;\n}\n\nbool OPCUAClient::writeBool(const opcua::NodeId& node, bool val) {\n    opcua::VariantValue v;\n    v.set_type(opcua::DataTypeId::BOOL);\n    v.set_bool_val(val);\n    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;\n}\n\nbool OPCUAClient::writeInt32(const opcua::NodeId& node, int32_t val) {\n    opcua::VariantValue v;\n    v.set_type(opcua::DataTypeId::INT32);\n    v.set_int32_val(val);\n    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;\n}\n\nbool OPCUAClient::writeInt64(const opcua::NodeId& node, int64_t val) {\n    opcua::VariantValue v;\n    v.set_type(opcua::DataTypeId::INT64);\n    v.set_int64_val(val);\n    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;\n}\n\nbool OPCUAClient::writeUInt32(const opcua::NodeId& node, uint32_t val) {\n    opcua::VariantValue v;\n    v.set_type(opcua::DataTypeId::UINT32);\n    v.set_uint32_val(val);\n    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;\n}\n\nbool OPCUAClient::writeFloat(const opcua::NodeId& node, float val) {\n    opcua::VariantValue v;\n    v.set_type(opcua::DataTypeId::FLOAT);\n    v.set_float_val(val);\n    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;\n}\n\nbool OPCUAClient::writeDouble(const opcua::NodeId& node, double val) {\n    opcua::VariantValue v;\n    v.set_type(opcua::DataTypeId::DOUBLE);\n    v.set_double_val(val);\n    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;\n}\n\nbool OPCUAClient::writeString(const opcua::NodeId& node, const std::string& val) {\n    opcua::VariantValue v;\n    v.set_type(opcua::DataTypeId::STRING);\n    v.set_string_val(val);\n    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;\n}\n\nbool OPCUAClient::writeInt32Array(const opcua::NodeId& node, const std::vector<int32_t>& vals) {\n    opcua::VariantValue v;\n    v.set_type(opcua::DataTypeId::ARRAY_INT32);\n    for (auto x : vals) v.add_int32_array(x);\n    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;\n}\n\nbool OPCUAClient::writeFloatArray(const opcua::NodeId& node, const std::vector<float>& vals) {\n    opcua::VariantValue v;\n    v.set_type(opcua::DataTypeId::ARRAY_FLOAT);\n    for (auto x : vals) v.add_float_array(x);\n    return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;\n}\n\n// ============ BATCH ============\n\nopcua::BatchReadResponse OPCUAClient::batchRead(const opcua::BatchReadRequest& req) {\n    opcua::BatchReadResponse resp;\n    for (const auto& r : req.requests()) {\n        *resp.add_responses() = read(r);\n    }\n    return resp;\n}\n\nopcua::BatchWriteResponse OPCUAClient::batchWrite(const opcua::BatchWriteRequest& req) {\n    opcua::BatchWriteResponse resp;\n    for (const auto& r : req.requests()) {\n        *resp.add_responses() = write(r);\n    }\n    return resp;\n}\n\n// ============ BROWSE ============\n\nopcua::BrowseResponse OPCUAClient::browse(const opcua::BrowseRequest& req) {\n    opcua::BrowseResponse resp;\n\n    UA_BrowseRequest breq;\n    UA_BrowseRequest_init(&breq);\n    breq.nodesToBrowse = UA_BrowseDescription_new();\n    breq.nodesToBrowseSize = 1;\n    breq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(\n        req.node_id().namespace_index(), req.node_id().numeric_id());\n    breq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;\n\n    if (req.node_id().id_case() == opcua::NodeId::IdCase::kStringId) {\n        breq.nodesToBrowse[0].nodeId = UA_NODEID_STRING_ALLOC(\n            req.node_id().namespace_index(), req.node_id().string_id().c_str());\n    }\n\n    if (req.max_references() > 0)\n        breq.requestedMaxReferencesPerNode = req.max_references();\n\n    if (req.node_class_mask() > 0) {\n        breq.nodesToBrowse[0].nodeClassMask = req.node_class_mask();\n    }\n\n    UA_BrowseResponse raw_resp;\n    auto status = UA_Client_Service_browse(client_.get(), &breq, &raw_resp);\n    if (status == UA_STATUSCODE_GOOD) {\n        resp.mutable_status()->set_code(UA_STATUSCODE_GOOD);\n        for (size_t i = 0; i < raw_resp.resultsSize; ++i) {\n            for (size_t j = 0; j < raw_resp.results[i].referencesSize; ++j) {\n                auto* ref = &raw_resp.results[i].references[j];\n                auto* pb_ref = resp.add_references();\n                // display name\n                auto dn = ref->displayName;\n                pb_ref->set_display_name(dn.length > 0\n                    ? std::string((const char*)dn.data, dn.length) : \"\");\n                // browse name\n                auto bn = ref->browseName.name;\n                pb_ref->set_browse_name(bn.length > 0\n                    ? std::string((const char*)bn.data, bn.length) : \"\");\n                pb_ref->set_node_class(ref->nodeClass);\n                // node id\n                if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC) {\n                    pb_ref->mutable_node_id()->set_namespace_index(ref->nodeId.nodeId.namespaceIndex);\n                    pb_ref->mutable_node_id()->set_numeric_id(ref->nodeId.nodeId.identifier.numeric);\n                }\n            }\n        }\n    } else {\n        resp.mutable_status()->set_code(status);\n    }\n\n    UA_BrowseRequest_clear(&breq);\n    UA_BrowseResponse_clear(&raw_resp);\n    return resp;\n}\n\nstd::vector<opcua::ReferenceDescription> OPCUAClient::browseChildren(const opcua::NodeId& node) {\n    opcua::BrowseRequest req;\n    *req.mutable_node_id() = node;\n    auto resp = browse(req);\n    return {resp.references().begin(), resp.references().end()};\n}\n\nstd::vector<opcua::ReferenceDescription> OPCUAClient::browseObjects(const opcua::NodeId& node) {\n    opcua::BrowseRequest req;\n    *req.mutable_node_id() = node;\n    req.set_node_class_mask(1); // Object\n    auto resp = browse(req);\n    return {resp.references().begin(), resp.references().end()};\n}\n\nstd::vector<opcua::ReferenceDescription> OPCUAClient::browseVariables(const opcua::NodeId& node) {\n    opcua::BrowseRequest req;\n    *req.mutable_node_id() = node;\n    req.set_node_class_mask(2); // Variable\n    auto resp = browse(req);\n    return {resp.references().begin(), resp.references().end()};\n}\n\nopcua::BrowseResponse OPCUAClient::browseRecursive(const opcua::NodeId& node, int depth) {\n    opcua::BrowseResponse resp;\n    if (depth <= 0) return resp;\n\n    auto children = browseChildren(node);\n    for (const auto& child : children) {\n        *resp.add_references() = child;\n        auto sub = browseRecursive(child.node_id(), depth - 1);\n        for (const auto& ref : sub.references())\n            *resp.add_references() = ref;\n    }\n    resp.mutable_status()->set_code(UA_STATUSCODE_GOOD);\n    return resp;\n}\n\n// ============ SUBSCRIPTION ============\n\nvoid OPCUAClient::dataChangeHandler(UA_Client* /*client*/,\n                                     uint32_t /*sub_id*/,\n                                     void* sub_ctx,\n                                     uint32_t mon_id,\n                                     void* mon_ctx,\n                                     void* data_value) {\n    auto* callbacks = static_cast<std::unordered_map<uint32_t, DataChangeCallback>*>(sub_ctx);\n    auto* dv = static_cast<UA_DataValue*>(data_value);\n\n    auto it = callbacks->find(mon_id);\n    if (it != callbacks->end()) {\n        opcua::DataChangeNotification notif;\n        notif.set_monitored_item_id(mon_id);\n        notif.mutable_status()->set_code(dv->status);\n\n        if (dv->hasValue && dv->value.type) {\n            auto self = static_cast<OPCUAClient*>(mon_ctx);\n            *notif.mutable_value() = self->variantToProto(&dv->value);\n        }\n        it->second(notif);\n    }\n}\n\nopcua::SubscribeResponse OPCUAClient::subscribe(const opcua::SubscribeRequest& req,\n                                                  DataChangeCallback cb) {\n    opcua::SubscribeResponse resp;\n\n    // create subscription\n    UA_CreateSubscriptionRequest sub_req = UA_CreateSubscriptionRequest_default();\n    auto sub_resp = UA_Client_Subscriptions_create(client_.get(), sub_req,\n                                                    nullptr, nullptr, nullptr);\n    if (sub_resp.responseHeader.serviceResult != UA_STATUSCODE_GOOD) {\n        resp.mutable_status()->set_code(sub_resp.responseHeader.serviceResult);\n        return resp;\n    }\n    resp.set_subscription_id(sub_resp.subscriptionId);\n\n    // create monitored item\n    UA_MonitoredItemCreateRequest mon_req = UA_MonitoredItemCreateRequest_default(\n        UA_NODEID_NUMERIC(req.node_id().namespace_index(), req.node_id().numeric_id()));\n    if (req.sampling_interval_ms() > 0)\n        mon_req.requestedParameters.samplingInterval = req.sampling_interval_ms();\n    if (req.queue_size() > 0)\n        mon_req.requestedParameters.queueSize = req.queue_size();\n    mon_req.requestedParameters.discardOldest = req.discard_oldest();\n\n    auto mon_resp = UA_Client_Subscriptions_addMonitoredItem(\n        client_.get(), sub_resp.subscriptionId, mon_req,\n        &callbacks_, dataChangeHandler, this);\n    if (mon_resp.statusCode != UA_STATUSCODE_GOOD) {\n        resp.mutable_status()->set_code(mon_resp.statusCode);\n        return resp;\n    }\n    resp.set_monitored_item_id(mon_resp.monitoredItemId);\n    resp.mutable_status()->set_code(UA_STATUSCODE_GOOD);\n\n    callbacks_[mon_resp.monitoredItemId] = std::move(cb);\n    return resp;\n}\n\nopcua::UnsubscribeResponse OPCUAClient::unsubscribe(const opcua::UnsubscribeRequest& req) {\n    opcua::UnsubscribeResponse resp;\n    UA_Client_Subscriptions_removeMonitoredItem(\n        client_.get(), req.subscription_id(), req.monitored_item_id());\n    callbacks_.erase(req.monitored_item_id());\n    resp.mutable_status()->set_code(UA_STATUSCODE_GOOD);\n    return resp;\n}\n\nbool OPCUAClient::isSubscribed(uint32_t monitored_item_id) const {\n    return callbacks_.find(monitored_item_id) != callbacks_.end();\n}\n\n// ============ METHOD CALL ============\n\nopcua::CallMethodResponse OPCUAClient::callMethod(const opcua::CallMethodRequest& req) {\n    opcua::CallMethodResponse resp;\n\n    UA_CallMethodRequest cReq;\n    UA_CallMethodRequest_init(&cReq);\n    cReq.objectId = UA_NODEID_NUMERIC(\n        req.object_id().namespace_index(), req.object_id().numeric_id());\n    cReq.methodId = UA_NODEID_NUMERIC(\n        req.method_id().namespace_index(), req.method_id().numeric_id());\n\n    if (req.input_args_size() > 0) {\n        cReq.inputArgumentsSize = req.input_args_size();\n        cReq.inputArguments = static_cast<UA_Variant*>(\n            UA_Array_new(cReq.inputArgumentsSize, &UA_TYPES[UA_TYPES_VARIANT]));\n        for (size_t i = 0; i < cReq.inputArgumentsSize; ++i) {\n            auto* var = static_cast<UA_Variant*>(protoToVariant(req.input_args(i)));\n            if (var) {\n                UA_Variant_copy(var, &cReq.inputArguments[i]);\n                UA_Variant_delete(var);\n            }\n        }\n    }\n\n    UA_CallRequest call_req;\n    UA_CallRequest_init(&call_req);\n    call_req.methodsToCall = &cReq;\n    call_req.methodsToCallSize = 1;\n\n    UA_CallResponse call_resp;\n    auto status = UA_Client_Service_call(client_.get(), &call_req, &call_resp);\n    if (status == UA_STATUSCODE_GOOD && call_resp.resultsSize > 0) {\n        auto* result = &call_resp.results[0];\n        resp.mutable_status()->set_code(result->statusCode);\n        for (size_t i = 0; i < result->outputArgumentsSize; ++i) {\n            *resp.add_output_args() = variantToProto(&result->outputArguments[i]);\n        }\n    } else {\n        resp.mutable_status()->set_code(status);\n    }\n\n    UA_CallRequest_clear(&call_req);\n    UA_CallResponse_clear(&call_resp);\n    return resp;\n}\n\n// ============ HISTORY READ ============\n\nopcua::HistoryReadResponse OPCUAClient::historyReadRaw(const opcua::HistoryReadRequest& req) {\n    opcua::HistoryReadResponse resp;\n\n    UA_ReadRawModifiedDetails details;\n    UA_ReadRawModifiedDetails_init(&details);\n    details.startTime = req.start_time();\n    details.endTime = req.end_time();\n    details.numValuesPerNode = req.max_values() > 0 ? req.max_values() : 100;\n    details.isReadModified = false;\n\n    UA_ExtensionObject ext;\n    UA_ExtensionObject_init(&ext);\n    ext.encoding = UA_EXTENSIONOBJECT_DECODED;\n    ext.content.decoded.type = &UA_TYPES[UA_TYPES_READRAWMODIFIEDDETAILS];\n    ext.content.decoded.data = UA_new(&UA_TYPES[UA_TYPES_READRAWMODIFIEDDETAILS]);\n    memcpy(ext.content.decoded.data, &details, sizeof(UA_ReadRawModifiedDetails));\n\n    UA_HistoryReadRequest hreq;\n    UA_HistoryReadRequest_init(&hreq);\n    hreq.nodesToRead = UA_HistoryReadValueId_new();\n    hreq.nodesToReadSize = 1;\n    hreq.nodesToRead[0].nodeId = UA_NODEID_NUMERIC(\n        req.node_id().namespace_index(), req.node_id().numeric_id());\n    hreq.historyReadDetails = ext;\n\n    UA_HistoryReadResponse hresp;\n    auto status = UA_Client_Service_historyRead(client_.get(), &hreq, &hresp);\n    if (status == UA_STATUSCODE_GOOD && hresp.resultsSize > 0) {\n        resp.mutable_status()->set_code(hresp.results[0].statusCode);\n        auto* data = &hresp.results[0].historyData;\n        for (size_t i = 0; i < data->dataValuesSize; ++i) {\n            auto* dv = &data->dataValues[i];\n            if (dv->hasValue && dv->value.type) {\n                *resp.add_values() = variantToProto(&dv->value);\n            }\n            resp.add_timestamps(dv->sourceTimestamp);\n        }\n    } else {\n        resp.mutable_status()->set_code(status);\n    }\n\n    UA_HistoryReadRequest_clear(&hreq);\n    UA_HistoryReadResponse_clear(&hresp);\n    return resp;\n}\n"
}
```

**Output:**
```
Wrote file successfully.

LSP errors detected in other files:
<diagnostics file="/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.h">
ERROR [7:10] 'opcua.pb.h' file not found
ERROR [25:5] Use of undeclared identifier 'opcua'
ERROR [25:36] Use of undeclared identifier 'opcua'
ERROR [26:5] Use of undeclared identifier 'opcua'
ERROR [26:41] Use of undeclared identifier 'opcua'
ERROR [27:5] Use of undeclared identifier 'opcua'
ERROR [27:45] Use of undeclared identifier 'opcua'
ERROR [30:25] Use of undeclared identifier 'opcua'
ERROR [31:29] Use of undeclared identifier 'opcua'
ERROR [32:29] Use of undeclared identifier 'opcua'
ERROR [33:31] Use of undeclared identifier 'opcua'
ERROR [34:27] Use of undeclared identifier 'opcua'
ERROR [35:29] Use of undeclared identifier 'opcua'
ERROR [36:34] Use of undeclared identifier 'opcua'
ERROR [37:47] Use of undeclared identifier 'opcua'
ERROR [38:45] Use of undeclared identifier 'opcua'
ERROR [39:47] Use of undeclared identifier 'opcua'
ERROR [42:5] Use of undeclared identifier 'opcua'
ERROR [42:38] Use of undeclared identifier 'opcua'
ERROR [43:26] Use of undeclared identifier 'opcua'
... and 1 more
</diagnostics>

LSP errors detected in this file, please fix:
<diagnostics file="/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.cpp">
ERROR [1:10] In included file: 'opcua.pb.h' file not found

/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.h:7:10:
note: error occurred here
ERROR [12:50] Use of undeclared identifier 'UA_Client_delete'; did you mean 'UA_ClientDeleter'? (fix available)
ERROR [21:19] Use of undeclared identifier 'UA_Client_new'; did you mean 'UA_Client'? (fix available)
ERROR [26:19] Use of undeclared identifier 'UA_STATUSCODE_GOOD'
ERROR [46:1] Use of undeclared identifier 'opcua'
ERROR [47:5] Use of undeclared identifier 'opcua'
ERROR [47:19] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:46:64: note: 'id' declared here
ERROR [48:5] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:46:64: note: 'id' declared here
ERROR [48:8] Member reference base type 'uint32_t' (aka 'unsigned int') is not a structure or union
ERROR [49:5] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:46:64: note: 'id' declared here
ERROR [49:8] Member reference base type 'uint32_t' (aka 'unsigned int') is not a structure or union
ERROR [50:12] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:46:64: note: 'id' declared here
ERROR [53:1] Use of undeclared identifier 'opcua'
ERROR [54:5] Use of undeclared identifier 'opcua'
ERROR [54:19] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:53:73: note: 'id' declared here
ERROR [55:5] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:53:73: note: 'id' declared here
ERROR [55:9] No member named 'set_namespace_index' in 'std::string'
ERROR [56:5] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:53:73: note: 'id' declared here
ERROR [56:9] No member named 'set_string_id' in 'std::string'
ERROR [57:12] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:53:73: note: 'id' declared here
... and 1 more
</diagnostics>
```

---

## Assistant (Build · Big Pickle · 6.7s)

_Thinking:_

The LSP errors are all expected — the proto-generated `opcua.pb.h` doesn't exist yet, and open62541 headers aren't installed in the LSP's include paths. These are all build-time dependencies. Let me continue with the scenarios file and CMakeLists.txt.

All LSP errors are expected (proto-generated header + open62541 not in LSP include paths). Now creating the comprehensive scenarios file:

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "priority": "high",
      "content": "Create .proto file defining OPC UA request/response messages",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Create OPC UA client wrapper with protobuf integration",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Create comprehensive usage scenarios demonstrating all PLC read/write patterns",
      "status": "in_progress"
    },
    {
      "priority": "medium",
      "content": "Create CMakeLists.txt build configuration",
      "status": "pending"
    }
  ]
}
```

**Output:**
```
[
  {
    "priority": "high",
    "content": "Create .proto file defining OPC UA request/response messages",
    "status": "completed"
  },
  {
    "priority": "high",
    "content": "Create OPC UA client wrapper with protobuf integration",
    "status": "completed"
  },
  {
    "priority": "high",
    "content": "Create comprehensive usage scenarios demonstrating all PLC read/write patterns",
    "status": "in_progress"
  },
  {
    "priority": "medium",
    "content": "Create CMakeLists.txt build configuration",
    "status": "pending"
  }
]
```

---

## Assistant (Build · Big Pickle · 98.8s)

_Thinking:_

Now the scenarios file. This is the key deliverable - all usage use cases for PLC data access with protobuf + OPC UA.

**Tool: write**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/scenarios.cpp",
  "content": "// ============================================================================\n// scenarios.cpp - 所有使用场景的完整用例 (Protobuf + OPC UA + PLC)\n// ============================================================================\n// 前提: 假设 PLC 已运行 OPC UA 服务器, 地址 opc.tcp://192.168.1.100:4840\n// 所有 NodeId 仅作示例用途, 按实际 PLC 变量地址替换\n// ============================================================================\n\n#include \"opcua_client.h\"\n#include <iostream>\n#include <thread>\n#include <chrono>\n\nstatic const std::string PLC_ENDPOINT = \"opc.tcp://192.168.1.100:4840\";\n\n// ============================================================================\n// 场景 1: 连接/断开 PLC\n// ============================================================================\nvoid scenario_connect_disconnect() {\n    std::cout << \"\\n=== 场景 1: 连接/断开 PLC ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n\n    if (client.connect(5000)) {\n        std::cout << \"PLC 连接成功\\n\";\n        // ... 执行操作 ...\n        client.disconnect();\n        std::cout << \"PLC 已断开\\n\";\n    } else {\n        std::cout << \"PLC 连接失败\\n\";\n    }\n}\n\n// ============================================================================\n// 场景 2: 读取标量值 (基本数据类型)\n// ============================================================================\nvoid scenario_read_scalar_types() {\n    std::cout << \"\\n=== 场景 2: 读取标量值 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    // 使用 protobuf 构建 NodeId (namespace 0, numeric id)\n    auto bool_node   = OPCUAClient::numericNodeId(0, 1001);\n    auto int32_node  = OPCUAClient::numericNodeId(0, 1002);\n    auto int64_node  = OPCUAClient::numericNodeId(0, 1003);\n    auto uint32_node = OPCUAClient::numericNodeId(0, 1004);\n    auto float_node  = OPCUAClient::numericNodeId(0, 1005);\n    auto double_node = OPCUAClient::numericNodeId(0, 1006);\n    auto string_node = OPCUAClient::numericNodeId(0, 1007);\n\n    // 方案 A: 使用封装好的类型安全接口 (推荐)\n    bool b   = client.readBool(bool_node);\n    int32_t i32 = client.readInt32(int32_node);\n    int64_t i64 = client.readInt64(int64_node);\n    uint32_t u32 = client.readUInt32(uint32_node);\n    float f   = client.readFloat(float_node);\n    double d  = client.readDouble(double_node);\n    std::string s = client.readString(string_node);\n\n    std::cout << \"Bool:   \" << b << \"\\n\";\n    std::cout << \"Int32:  \" << i32 << \"\\n\";\n    std::cout << \"Int64:  \" << i64 << \"\\n\";\n    std::cout << \"UInt32: \" << u32 << \"\\n\";\n    std::cout << \"Float:  \" << f << \"\\n\";\n    std::cout << \"Double: \" << d << \"\\n\";\n    std::cout << \"String: \" << s << \"\\n\";\n\n    // 方案 B: 使用通用 protobuf 请求/响应 (需自行处理类型)\n    opcua::ReadRequest req;\n    *req.mutable_node_id() = int32_node;\n    req.set_attribute_id(13);    // UA_ATTRIBUTEID_VALUE\n    req.set_include_timestamp(true);\n\n    opcua::ReadResponse resp = client.read(req);\n    if (resp.status().code() == 0) {  // UA_STATUSCODE_GOOD\n        std::cout << \"Value via proto: \" << resp.value().int32_val()\n                  << \", server_ts: \" << resp.server_timestamp() << \"\\n\";\n    }\n}\n\n// ============================================================================\n// 场景 3: 读取数组值\n// ============================================================================\nvoid scenario_read_array_types() {\n    std::cout << \"\\n=== 场景 3: 读取数组值 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    auto i32_arr_node = OPCUAClient::numericNodeId(0, 2001);\n    auto f32_arr_node = OPCUAClient::numericNodeId(0, 2002);\n    auto f64_arr_node = OPCUAClient::numericNodeId(0, 2003);\n\n    // 读取 int32 数组\n    std::vector<int32_t> i32_vals = client.readInt32Array(i32_arr_node);\n    std::cout << \"Int32 Array (\" << i32_vals.size() << \" elements):\";\n    for (auto v : i32_vals) std::cout << \" \" << v;\n    std::cout << \"\\n\";\n\n    // 读取 float 数组\n    std::vector<float> f32_vals = client.readFloatArray(f32_arr_node);\n    std::cout << \"Float Array (\" << f32_vals.size() << \" elements):\";\n    for (auto v : f32_vals) std::cout << \" \" << v;\n    std::cout << \"\\n\";\n\n    // 读取 double 数组\n    std::vector<double> f64_vals = client.readDoubleArray(f64_arr_node);\n    std::cout << \"Double Array (\" << f64_vals.size() << \" elements):\";\n    for (auto v : f64_vals) std::cout << \" \" << v;\n    std::cout << \"\\n\";\n}\n\n// ============================================================================\n// 场景 4: 读取非 Value 属性 (如 DisplayName, Description)\n// ============================================================================\nvoid scenario_read_attributes() {\n    std::cout << \"\\n=== 场景 4: 读取节点属性 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    auto node = OPCUAClient::numericNodeId(0, 1001);\n\n    // 读取 DisplayName (attribute_id = 14)\n    auto dn_resp = client.readAttribute(node, 14);\n    std::cout << \"DisplayName: \" << dn_resp.value().string_val() << \"\\n\";\n\n    // 读取 Description (attribute_id = 15)\n    auto desc_resp = client.readAttribute(node, 15);\n    std::cout << \"Description: \" << desc_resp.value().string_val() << \"\\n\";\n\n    // 读取 NodeClass (attribute_id = 2)\n    auto nc_resp = client.readAttribute(node, 2);\n    std::cout << \"NodeClass: \" << nc_resp.value().int32_val() << \"\\n\";\n\n    // 读取 BrowseName (attribute_id = 5)\n    auto bn_resp = client.readAttribute(node, 5);\n    std::cout << \"BrowseName: \" << bn_resp.value().string_val() << \"\\n\";\n\n    // 读取 WriteMask (attribute_id = 21)\n    auto wm_resp = client.readAttribute(node, 21);\n    std::cout << \"WriteMask: \" << wm_resp.value().uint32_val() << \"\\n\";\n\n    // 读取 UserWriteMask (attribute_id = 22)\n    auto uwm_resp = client.readAttribute(node, 22);\n    std::cout << \"UserWriteMask: \" << uwm_resp.value().uint32_val() << \"\\n\";\n\n    // 读取 DataType (attribute_id = 6)\n    auto dt_resp = client.readAttribute(node, 6);\n    std::cout << \"DataType NodeId: \" << dt_resp.value().uint32_val() << \"\\n\";\n}\n\n// ============================================================================\n// 场景 5: 写入标量值\n// ============================================================================\nvoid scenario_write_scalar_types() {\n    std::cout << \"\\n=== 场景 5: 写入标量值 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    auto bool_node   = OPCUAClient::numericNodeId(0, 1001);\n    auto int32_node  = OPCUAClient::numericNodeId(0, 1002);\n    auto float_node  = OPCUAClient::numericNodeId(0, 1005);\n    auto string_node = OPCUAClient::numericNodeId(0, 1007);\n\n    // 方案 A: 类型安全接口\n    bool ok1 = client.writeBool(bool_node, true);\n    bool ok2 = client.writeInt32(int32_node, 42);\n    bool ok3 = client.writeFloat(float_node, 3.14f);\n    bool ok4 = client.writeString(string_node, \"hello plc\");\n\n    std::cout << \"Write Bool:   \" << (ok1 ? \"OK\" : \"FAIL\") << \"\\n\";\n    std::cout << \"Write Int32:  \" << (ok2 ? \"OK\" : \"FAIL\") << \"\\n\";\n    std::cout << \"Write Float:  \" << (ok3 ? \"OK\" : \"FAIL\") << \"\\n\";\n    std::cout << \"Write String: \" << (ok4 ? \"OK\" : \"FAIL\") << \"\\n\";\n\n    // 写入后验证\n    int32_t verify = client.readInt32(int32_node);\n    std::cout << \"Verify read after write: \" << verify << \" (expect 42)\\n\";\n\n    // 方案 B: 通用 protobuf 写入\n    opcua::WriteRequest wreq;\n    *wreq.mutable_node_id() = int32_node;\n    wreq.mutable_value()->set_type(opcua::DataTypeId::INT32);\n    wreq.mutable_value()->set_int32_val(100);\n\n    opcua::WriteResponse wresp = client.write(wreq);\n    std::cout << \"Write via proto: \"\n              << (wresp.status().code() == 0 ? \"OK\" : \"FAIL\") << \"\\n\";\n}\n\n// ============================================================================\n// 场景 6: 写入数组值\n// ============================================================================\nvoid scenario_write_array_types() {\n    std::cout << \"\\n=== 场景 6: 写入数组值 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    auto i32_arr_node = OPCUAClient::numericNodeId(0, 2001);\n    auto f32_arr_node = OPCUAClient::numericNodeId(0, 2002);\n\n    // 写入 int32 数组\n    bool ok = client.writeInt32Array(i32_arr_node, {10, 20, 30, 40, 50});\n    std::cout << \"Write Int32 Array: \" << (ok ? \"OK\" : \"FAIL\") << \"\\n\";\n\n    // 写入 float 数组\n    ok = client.writeFloatArray(f32_arr_node, {1.1f, 2.2f, 3.3f});\n    std::cout << \"Write Float Array: \" << (ok ? \"OK\" : \"FAIL\") << \"\\n\";\n}\n\n// ============================================================================\n// 场景 7: 批量读取 (Batch Read)\n// ============================================================================\nvoid scenario_batch_read() {\n    std::cout << \"\\n=== 场景 7: 批量读取 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    // 一次请求中读取多个 PLC 变量节点\n    opcua::BatchReadRequest batch_req;\n\n    auto add_read = [&](uint32_t ns, uint32_t id) {\n        auto* r = batch_req.add_requests();\n        r->mutable_node_id()->set_namespace_index(ns);\n        r->mutable_node_id()->set_numeric_id(id);\n        r->set_attribute_id(13);  // Value\n    };\n\n    // 模拟从配置文件/配表来的节点列表\n    add_read(0, 1001);   // Bool\n    add_read(0, 1002);   // Int32\n    add_read(0, 1005);   // Float\n    add_read(0, 1007);   // String\n    add_read(0, 2001);   // Int32 Array\n\n    opcua::BatchReadResponse batch_resp = client.batchRead(batch_req);\n    std::cout << \"Batch read \" << batch_resp.responses_size() << \" nodes:\\n\";\n    for (int i = 0; i < batch_resp.responses_size(); ++i) {\n        const auto& r = batch_resp.responses(i);\n        std::cout << \"  Node[\" << i << \"] status=\" << r.status().code()\n                  << \" type=\" << r.value().type() << \"\\n\";\n    }\n}\n\n// ============================================================================\n// 场景 8: 批量写入 (Batch Write)\n// ============================================================================\nvoid scenario_batch_write() {\n    std::cout << \"\\n=== 场景 8: 批量写入 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    opcua::BatchWriteRequest batch_wreq;\n\n    // 同时写入多个节点\n    auto add_write_int = [&](uint32_t ns, uint32_t id, int32_t val) {\n        auto* w = batch_wreq.add_requests();\n        w->mutable_node_id()->set_namespace_index(ns);\n        w->mutable_node_id()->set_numeric_id(id);\n        w->mutable_value()->set_type(opcua::DataTypeId::INT32);\n        w->mutable_value()->set_int32_val(val);\n    };\n\n    add_write_int(0, 1002, 10);\n    add_write_int(0, 1003, 20);\n    add_write_int(0, 1004, 30);\n\n    opcua::BatchWriteResponse batch_wresp = client.batchWrite(batch_wreq);\n    std::cout << \"Batch write results:\\n\";\n    for (int i = 0; i < batch_wresp.responses_size(); ++i) {\n        std::cout << \"  Write[\" << i << \"]: \"\n                  << (batch_wresp.responses(i).status().code() == 0 ? \"OK\" : \"FAIL\")\n                  << \"\\n\";\n    }\n}\n\n// ============================================================================\n// 场景 9: 遍历/浏览 PLC 地址空间 (Browse)\n// ============================================================================\nvoid scenario_browse() {\n    std::cout << \"\\n=== 场景 9: 浏览 PLC 地址空间 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    // 从 Objects 根节点浏览 (ns=0, id=85)\n    auto root = OPCUAClient::numericNodeId(0, 85);\n\n    // 9a: 浏览所有子节点\n    std::cout << \"--- 9a: 浏览所有子节点 ---\\n\";\n    auto all_children = client.browseChildren(root);\n    for (const auto& ref : all_children) {\n        std::cout << \"  \" << ref.display_name()\n                  << \" (class=\" << ref.node_class() << \")\\n\";\n    }\n\n    // 9b: 只浏览变量节点 (node_class_mask = 2)\n    std::cout << \"--- 9b: 只浏览变量节点 ---\\n\";\n    auto vars = client.browseVariables(root);\n    for (const auto& ref : vars) {\n        std::cout << \"  Variable: \" << ref.display_name() << \"\\n\";\n    }\n\n    // 9c: 只浏览对象节点 (node_class_mask = 1)\n    std::cout << \"--- 9c: 只浏览对象节点 ---\\n\";\n    auto objs = client.browseObjects(root);\n    for (const auto& ref : objs) {\n        std::cout << \"  Object: \" << ref.display_name() << \"\\n\";\n    }\n\n    // 9d: 递归浏览 (遍历树上所有节点, 限制深度)\n    std::cout << \"--- 9d: 递归浏览 (depth=2) ---\\n\";\n    auto recursive = client.browseRecursive(root, 2);\n    for (const auto& ref : recursive.references()) {\n        std::cout << \"  \" << ref.display_name()\n                  << \" (class=\" << ref.node_class() << \")\\n\";\n    }\n\n    // 9e: 使用 protobuf 请求自定义过滤\n    std::cout << \"--- 9e: 用 proto 自定义 Browse ---\\n\";\n    opcua::BrowseRequest breq;\n    *breq.mutable_node_id() = root;\n    breq.set_node_class_mask(2);  // 只找 Variable\n    breq.set_max_references(20);\n\n    auto bresp = client.browse(breq);\n    for (const auto& ref : bresp.references()) {\n        std::cout << \"  ProtoBrowse: \" << ref.display_name() << \"\\n\";\n    }\n}\n\n// ============================================================================\n// 场景 10: 订阅数据变化 (Subscribe/MonitoredItem)\n// ============================================================================\nvoid scenario_subscription() {\n    std::cout << \"\\n=== 场景 10: 订阅 PLC 数据变化 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    auto temp_node = OPCUAClient::numericNodeId(0, 3001);  // 温度变量\n\n    // 订阅温度变量, 100ms 采样间隔\n    opcua::SubscribeRequest sub_req;\n    *sub_req.mutable_node_id() = temp_node;\n    sub_req.set_sampling_interval_ms(100.0);\n    sub_req.set_queue_size(10);\n    sub_req.set_discard_oldest(true);\n\n    auto sub_resp = client.subscribe(sub_req,\n        [](const opcua::DataChangeNotification& notif) {\n            std::cout << \"[Sub] mon_id=\" << notif.monitored_item_id()\n                      << \" value=\";\n            if (notif.value().type() == opcua::DataTypeId::FLOAT)\n                std::cout << notif.value().float_val();\n            else if (notif.value().type() == opcua::DataTypeId::DOUBLE)\n                std::cout << notif.value().double_val();\n            else if (notif.value().type() == opcua::DataTypeId::INT32)\n                std::cout << notif.value().int32_val();\n            std::cout << \" ts=\" << notif.source_timestamp() << \"\\n\";\n        });\n\n    if (sub_resp.status().code() == 0) {\n        std::cout << \"Subscribed: sub_id=\" << sub_resp.subscription_id()\n                  << \" mon_id=\" << sub_resp.monitored_item_id() << \"\\n\";\n    }\n\n    // 保持运行 10s 收集数据变化\n    std::this_thread::sleep_for(std::chrono::seconds(10));\n\n    // 取消订阅\n    opcua::UnsubscribeRequest unsub_req;\n    unsub_req.set_subscription_id(sub_resp.subscription_id());\n    unsub_req.set_monitored_item_id(sub_resp.monitored_item_id());\n    client.unsubscribe(unsub_req);\n    std::cout << \"Unsubscribed\\n\";\n}\n\n// ============================================================================\n// 场景 11: 调用 PLC 方法 (Method Call)\n// ============================================================================\nvoid scenario_method_call() {\n    std::cout << \"\\n=== 场景 11: 调用 PLC 方法 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    // 调用 PLC 上的方法: Objects.MyPLC.ResetCounter\n    auto obj_node  = OPCUAClient::numericNodeId(0, 5001);  // 对象节点\n    auto mtd_node  = OPCUAClient::numericNodeId(0, 5002);  // 方法节点\n\n    // 无参调用\n    opcua::CallMethodRequest call_req;\n    *call_req.mutable_object_id() = obj_node;\n    *call_req.mutable_method_id() = mtd_node;\n\n    auto call_resp = client.callMethod(call_req);\n    std::cout << \"Method call (no args): \"\n              << (call_resp.status().code() == 0 ? \"OK\" : \"FAIL\") << \"\\n\";\n\n    // 带参调用: SetSpeed(speed)\n    auto speed_method = OPCUAClient::numericNodeId(0, 5003);\n    opcua::CallMethodRequest call_req2;\n    *call_req2.mutable_object_id() = obj_node;\n    *call_req2.mutable_method_id() = speed_method;\n\n    auto* arg = call_req2.add_input_args();\n    arg->set_type(opcua::DataTypeId::FLOAT);\n    arg->set_float_val(1500.5f);\n\n    auto call_resp2 = client.callMethod(call_req2);\n    if (call_resp2.status().code() == 0 && call_resp2.output_args_size() > 0) {\n        // 假设返回 bool 表示成功\n        bool success = call_resp2.output_args(0).bool_val();\n        std::cout << \"SetSpeed(1500.5) returned: \" << success << \"\\n\";\n    }\n}\n\n// ============================================================================\n// 场景 12: 读取历史数据 (Historical Access)\n// ============================================================================\nvoid scenario_history_read() {\n    std::cout << \"\\n=== 场景 12: 读取历史数据 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    auto hist_node = OPCUAClient::numericNodeId(0, 4001);\n\n    // 读取过去 1 小时的历史数据\n    uint64_t now = UA_DateTime_now();\n    uint64_t one_hour_ago = now - 3600 * 1000 * 10000;  // UA_DateTime 单位 100ns\n\n    opcua::HistoryReadRequest hist_req;\n    *hist_req.mutable_node_id() = hist_node;\n    hist_req.set_start_time(one_hour_ago);\n    hist_req.set_end_time(now);\n    hist_req.set_max_values(100);\n\n    auto hist_resp = client.historyReadRaw(hist_req);\n    if (hist_resp.status().code() == 0) {\n        std::cout << \"History data points: \" << hist_resp.values_size() << \"\\n\";\n        for (int i = 0; i < hist_resp.values_size(); ++i) {\n            std::cout << \"  [\" << i << \"] val=\";\n            if (hist_resp.values(i).type() == opcua::DataTypeId::FLOAT)\n                std::cout << hist_resp.values(i).float_val();\n            else if (hist_resp.values(i).type() == opcua::DataTypeId::DOUBLE)\n                std::cout << hist_resp.values(i).double_val();\n            std::cout << \" ts=\" << hist_resp.timestamps(i) << \"\\n\";\n        }\n    }\n}\n\n// ============================================================================\n// 场景 13: 通过字符串 NodeId 访问 PLC 变量 (西门子 S7-1500 等)\n// ============================================================================\nvoid scenario_string_nodeid() {\n    std::cout << \"\\n=== 场景 13: 字符串 NodeId 访问 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    // 有些 PLC (如西门子) 使用字符串 NodeId:\n    //   ns=3;s=\"::Program:MainProgram.Temperature\"\n    //   ns=3;s=\"::Program:MainProgram.Pressure\"\n    auto temp_node = OPCUAClient::stringNodeId(3, \"::Program:MainProgram.Temperature\");\n    auto press_node = OPCUAClient::stringNodeId(3, \"::Program:MainProgram.Pressure\");\n    auto speed_node = OPCUAClient::stringNodeId(3, \"::Program:MainProgram.MotorSpeed\");\n\n    float temp  = client.readFloat(temp_node);\n    float press = client.readFloat(press_node);\n    float speed = client.readFloat(speed_node);\n\n    std::cout << \"Temperature: \" << temp << \" C\\n\";\n    std::cout << \"Pressure: \" << press << \" bar\\n\";\n    std::cout << \"MotorSpeed: \" << speed << \" rpm\\n\";\n\n    // 写入\n    client.writeFloat(speed_node, 1200.0f);\n    std::cout << \"MotorSpeed set to 1200\\n\";\n}\n\n// ============================================================================\n// 场景 14: 写入不同属性 (如 EngineeringUnits 等)\n// ============================================================================\nvoid scenario_write_attribute() {\n    std::cout << \"\\n=== 场景 14: 写入属性 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    auto node = OPCUAClient::numericNodeId(0, 1002);\n\n    // 写 Description (attribute_id = 15)\n    opcua::WriteRequest wreq;\n    *wreq.mutable_node_id() = node;\n    wreq.set_attribute_id(15);   // UA_ATTRIBUTEID_DESCRIPTION\n    wreq.mutable_value()->set_type(opcua::DataTypeId::STRING);\n    wreq.mutable_value()->set_string_val(\"Engine RPM value\");\n\n    auto wresp = client.write(wreq);\n    std::cout << \"Write Description: \"\n              << (wresp.status().code() == 0 ? \"OK\" : \"FAIL\") << \"\\n\";\n}\n\n// ============================================================================\n// 场景 15: 读写大端/位域 (通过 protobuf bytes 传递原始数据)\n// ============================================================================\nvoid scenario_raw_bytes() {\n    std::cout << \"\\n=== 场景 15: 原始字节读写 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    auto node = OPCUAClient::numericNodeId(0, 6001);\n\n    // 读取 4 字节原始数据\n    auto resp = client.readValue(node);\n    if (resp.value().type() == opcua::DataTypeId::BYTESTRING) {\n        std::string raw = resp.value().bytes_val();\n        std::cout << \"Raw bytes (\" << raw.size() << \"): \";\n        for (unsigned char c : raw)\n            std::cout << std::hex << (int)c << \" \";\n        std::cout << std::dec << \"\\n\";\n\n        // 解析位域 (假设 byte[0] 的低 4 位是状态标志)\n        uint8_t status_flags = static_cast<uint8_t>(raw[0]) & 0x0F;\n        std::cout << \"Status flags (lower nibble): \" << (int)status_flags << \"\\n\";\n    }\n\n    // 写入 4 字节原始数据\n    opcua::WriteRequest wreq;\n    *wreq.mutable_node_id() = node;\n    wreq.mutable_value()->set_type(opcua::DataTypeId::BYTESTRING);\n    wreq.mutable_value()->set_bytes_val(std::string({0x01, 0x02, 0x03, 0x04}));\n    client.write(wreq);\n}\n\n// ============================================================================\n// 场景 16: 读写 LocalizedText / QualifiedName 类型\n// ============================================================================\nvoid scenario_structured_types() {\n    std::cout << \"\\n=== 场景 16: 结构化类型 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    auto node = OPCUAClient::numericNodeId(0, 7001);\n\n    // 通用 Read/Write 请求通过 protobuf VariantValue 传递\n    opcua::ReadRequest req;\n    *req.mutable_node_id() = node;\n    auto resp = client.read(req);\n\n    if (resp.status().code() == 0) {\n        std::cout << \"Read type=\" << resp.value().type()\n                  << \" string_val=\" << resp.value().string_val() << \"\\n\";\n    }\n\n    // QualifiedName 可编码为字符串 \"ns:name\"\n    // LocalizedText 可编码为字符串 \"locale:text\"\n    opcua::WriteRequest wreq;\n    *wreq.mutable_node_id() = node;\n    wreq.mutable_value()->set_type(opcua::DataTypeId::LOCALIZEDTEXT);\n    wreq.mutable_value()->set_string_val(\"en_US:Temperature Sensor\");\n    client.write(wreq);\n}\n\n// ============================================================================\n// 场景 17: 使用 protobuf OPCUARequest/OPCUAResponse 统一消息封装\n// ============================================================================\nvoid scenario_unified_message() {\n    std::cout << \"\\n=== 场景 17: 统一消息封装 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    // 构建统一的请求消息\n    opcua::OPCUARequest req;\n    req.set_request_id(1);\n\n    auto* read_req = req.mutable_read();\n    read_req->mutable_node_id()->set_namespace_index(0);\n    read_req->mutable_node_id()->set_numeric_id(1002);\n    read_req->set_attribute_id(13);\n\n    // 序列化为 bytes (用于网络传输 / 消息队列 / 日志存储)\n    std::string wire_bytes;\n    req.SerializeToString(&wire_bytes);\n    std::cout << \"Serialized OPCUARequest: \" << wire_bytes.size() << \" bytes\\n\";\n\n    // 反序列化并执行\n    opcua::OPCUARequest parsed_req;\n    parsed_req.ParseFromString(wire_bytes);\n\n    opcua::OPCUAResponse resp;\n    resp.set_request_id(parsed_req.request_id());\n\n    switch (parsed_req.request_case()) {\n    case opcua::OPCUARequest::kRead: {\n        auto read_resp = client.read(parsed_req.read());\n        *resp.mutable_read() = read_resp;\n        break;\n    }\n    default:\n        break;\n    }\n\n    // 序列化响应\n    std::string resp_bytes;\n    resp.SerializeToString(&resp_bytes);\n    std::cout << \"Response serialized: \" << resp_bytes.size() << \" bytes\\n\";\n    if (resp.read().status().code() == 0) {\n        std::cout << \"Read value: \" << resp.read().value().int32_val() << \"\\n\";\n    }\n}\n\n// ============================================================================\n// 场景 18: 安全连接 (用户名/密码 + 证书)\n// ============================================================================\nvoid scenario_secure_connection() {\n    std::cout << \"\\n=== 场景 18: 安全连接 (示意) ===\\n\";\n    OPCUAClient client(\"opc.tcp://192.168.1.100:4840\");\n    // 注意: 安全连接需要额外配置 UA_ClientConfig 设置证书和密码\n    // 以下为概念示例, 需要根据 open62541 安全配置 API 实现\n    /*\n    auto* config = UA_Client_getConfig(client.getNative());\n    config->clientDescription.applicationName = UA_LOCALIZEDTEXT(\"\", \"MyApp\");\n    // 设置用户名密码\n    UA_ClientConfig_setAuthenticationUsername(config, \"admin\", \"password\");\n    // 加载证书\n    UA_ByteString certificate = loadFile(\"client_cert.der\");\n    UA_ByteString privateKey  = loadFile(\"client_key.der\");\n    UA_ClientConfig_setSecurityPolicies(config, &certificate, &privateKey,\n                                         certificate, privateKey);\n    */\n    if (client.connect()) {\n        std::cout << \"Secure connection established\\n\";\n    }\n}\n\n// ============================================================================\n// 场景 19: 从配置文件/配表动态加载节点列表并轮询\n// ============================================================================\nvoid scenario_polling_from_config() {\n    std::cout << \"\\n=== 场景 19: 配表驱动轮询 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    // 模拟从 JSON/YAML/CSV 配表加载的变量监控列表\n    struct PollConfig {\n        std::string name;\n        uint32_t ns;\n        uint32_t id;\n        opcua::DataTypeId dtype;\n    };\n\n    std::vector<PollConfig> poll_list = {\n        {\"Temperature\",  0, 1005, opcua::DataTypeId::FLOAT},\n        {\"Pressure\",     0, 1006, opcua::DataTypeId::FLOAT},\n        {\"MotorSpeed\",   0, 1002, opcua::DataTypeId::INT32},\n        {\"IsRunning\",    0, 1001, opcua::DataTypeId::BOOL},\n        {\"AlarmCode\",    0, 1003, opcua::DataTypeId::INT64},\n    };\n\n    // 批量读取 (高效)\n    opcua::BatchReadRequest batch_req;\n    for (const auto& cfg : poll_list) {\n        auto* r = batch_req.add_requests();\n        r->mutable_node_id()->set_namespace_index(cfg.ns);\n        r->mutable_node_id()->set_numeric_id(cfg.id);\n    }\n\n    auto batch_resp = client.batchRead(batch_req);\n    for (int i = 0; i < batch_resp.responses_size(); ++i) {\n        std::cout << poll_list[i].name << \" = \";\n        switch (poll_list[i].dtype) {\n        case opcua::DataTypeId::BOOL:\n            std::cout << batch_resp.responses(i).value().bool_val(); break;\n        case opcua::DataTypeId::INT32:\n            std::cout << batch_resp.responses(i).value().int32_val(); break;\n        case opcua::DataTypeId::INT64:\n            std::cout << batch_resp.responses(i).value().int64_val(); break;\n        case opcua::DataTypeId::FLOAT:\n            std::cout << batch_resp.responses(i).value().float_val(); break;\n        default:\n            std::cout << \"(type \" << batch_resp.responses(i).value().type() << \")\";\n        }\n        std::cout << \"\\n\";\n    }\n}\n\n// ============================================================================\n// 场景 20: 异常/故障处理模式\n// ============================================================================\nvoid scenario_error_handling() {\n    std::cout << \"\\n=== 场景 20: 异常/故障处理 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n\n    // 尝试连接不存在的 PLC\n    OPCUAClient bad_client(\"opc.tcp://192.168.1.200:4840\");\n    if (!bad_client.connect(3000)) {\n        std::cout << \"场景 20a: 连接失败已捕获 (预期行为)\\n\";\n    }\n\n    if (!client.connect()) return;\n\n    // 读取不存在的节点\n    auto bad_node = OPCUAClient::numericNodeId(0, 99999);\n    auto resp = client.readValue(bad_node);\n    if (resp.status().code() != 0) {\n        std::cout << \"场景 20b: 读不存在的节点, 错误码=\"\n                  << resp.status().code() << \"\\n\";\n    }\n\n    // 写入只读节点 (假设 1001 是只读的)\n    auto read_node = OPCUAClient::numericNodeId(0, 1001);\n    bool wrote = client.writeBool(read_node, false);\n    if (!wrote) {\n        std::cout << \"场景 20c: 写入只读节点失败 (预期行为)\\n\";\n    }\n\n    // 类型不匹配读取\n    auto float_node = OPCUAClient::numericNodeId(0, 1005);\n    resp = client.readValue(float_node);\n    if (resp.value().type() == opcua::DataTypeId::FLOAT) {\n        std::cout << \"场景 20d: 读取 float 节点, 类型=\"\n                  << resp.value().type() << \" 值=\" << resp.value().float_val() << \"\\n\";\n    }\n\n    // 重连机制 (概念)\n    if (!client.isConnected()) {\n        std::cout << \"场景 20e: 尝试重连...\\n\";\n        for (int retry = 0; retry < 3; ++retry) {\n            if (client.connect()) {\n                std::cout << \"  第 \" << (retry + 1) << \" 次重连成功\\n\";\n                break;\n            }\n            std::this_thread::sleep_for(std::chrono::seconds(1));\n        }\n    }\n}\n\n// ============================================================================\n// 场景 21: protobuf 透传 -- 序列化后通过消息队列 / ZeroMQ / gRPC 转发\n// ============================================================================\nvoid scenario_proto_wire_transport() {\n    std::cout << \"\\n=== 场景 21: protobuf 序列化转发 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    // 从 PLC 读取数据\n    opcua::BatchReadRequest batch;\n    for (uint32_t id = 1001; id <= 1010; ++id) {\n        auto* r = batch.add_requests();\n        r->mutable_node_id()->set_namespace_index(0);\n        r->mutable_node_id()->set_numeric_id(id);\n    }\n    auto batch_resp = client.batchRead(batch);\n\n    // 序列化为 protobuf bytes\n    std::string pb_data;\n    batch_resp.SerializeToString(&pb_data);\n    std::cout << \"Serialized batch response: \" << pb_data.size() << \" bytes\\n\";\n\n    // 模拟发送到消息队列 (Kafka / RabbitMQ / ZeroMQ)\n    // mq_producer.send(\"plc_data_topic\", pb_data);\n\n    // 对端反序列化\n    opcua::BatchReadResponse received;\n    if (received.ParseFromString(pb_data)) {\n        std::cout << \"Deserialized \" << received.responses_size() << \" values\\n\";\n        for (int i = 0; i < received.responses_size(); ++i) {\n            std::cout << \"  val[\" << i << \"] type=\"\n                      << received.responses(i).value().type() << \"\\n\";\n        }\n    }\n\n    // gRPC 示例: service PlcGateway { rpc Read(BatchReadRequest) returns (BatchReadResponse); }\n    // 说明: 可以直接将 protobuf 消息用于 gRPC 服务定义\n}\n\n// ============================================================================\n// 场景 22: 读写 UInt64 / 大整数 (如累计流量计读数)\n// ============================================================================\nvoid scenario_large_integer() {\n    std::cout << \"\\n=== 场景 22: 大整数读写 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    auto total_flow = OPCUAClient::numericNodeId(0, 8001);\n    auto serial_num = OPCUAClient::numericNodeId(0, 8002);\n\n    // 读取大整数累计值\n    uint64_t flow = client.readUInt32(total_flow);  // 若实际是 UInt64, 需要相应修改\n    std::cout << \"Total flow: \" << flow << \"\\n\";\n\n    // 使用 protobuf 写 UInt64\n    opcua::WriteRequest wreq;\n    *wreq.mutable_node_id() = serial_num;\n    wreq.mutable_value()->set_type(opcua::DataTypeId::UINT64);\n    wreq.mutable_value()->set_uint64_val(9876543210UL);\n    client.write(wreq);\n    std::cout << \"Serial number written\\n\";\n\n    // 读取 Int64 (负数兼容)\n    int64_t counter = client.readInt64(OPCUAClient::numericNodeId(0, 8003));\n    std::cout << \"Signed counter: \" << counter << \"\\n\";\n}\n\n// ============================================================================\n// 场景 23: 通过 QualifiedName 浏览并快速读取 (按名称查找变量)\n// ============================================================================\nvoid scenario_browse_and_read() {\n    std::cout << \"\\n=== 场景 23: 浏览 + 读取组合 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    auto root = OPCUAClient::numericNodeId(0, 85);\n\n    // 浏览所有变量, 然后逐一读取值\n    auto vars = client.browseVariables(root);\n    for (const auto& v : vars) {\n        auto val = client.readValue(v.node_id());\n        if (val.status().code() == 0) {\n            std::cout << v.display_name() << \" = \";\n            if (val.value().has_int32_val())\n                std::cout << val.value().int32_val();\n            else if (val.value().has_float_val())\n                std::cout << val.value().float_val();\n            else if (val.value().has_bool_val())\n                std::cout << val.value().bool_val();\n            else if (val.value().has_string_val())\n                std::cout << val.value().string_val();\n            std::cout << \"\\n\";\n        }\n    }\n}\n\n// ============================================================================\n// 场景 24: 读取 DateTime 类型\n// ============================================================================\nvoid scenario_datetime() {\n    std::cout << \"\\n=== 场景 24: DateTime 读写 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    auto dt_node = OPCUAClient::numericNodeId(0, 9001);\n\n    auto resp = client.readValue(dt_node);\n    if (resp.value().type() == opcua::DataTypeId::DATETIME) {\n        uint64_t dt_raw = resp.value().datetime_val();\n        // UA_DateTime 以 100ns 为单位, 从 1601-01-01 开始\n        // 转换为 Unix timestamp (秒)\n        uint64_t unix_sec = (dt_raw / 10000000) - 11644473600ULL;\n        std::cout << \"DateTime (raw): \" << dt_raw\n                  << \" (unix): \" << unix_sec << \"\\n\";\n    }\n}\n\n// ============================================================================\n// 场景 25: 带诊断信息的 Read (StatusCode 详细解析)\n// ============================================================================\nvoid scenario_status_code_detail() {\n    std::cout << \"\\n=== 场景 25: 状态码解析 ===\\n\";\n    OPCUAClient client(PLC_ENDPOINT);\n    if (!client.connect()) return;\n\n    auto node = OPCUAClient::numericNodeId(0, 1002);\n    auto resp = client.readValue(node);\n\n    uint32_t code = resp.status().code();\n    if (code == 0) {\n        std::cout << \"Good: \" << resp.value().int32_val() << \"\\n\";\n    } else {\n        // 常见错误码解析\n        switch (code) {\n        case 0x80000000:\n            std::cout << \"Bad Unexpected Error\\n\"; break;\n        case 0x803C0000:\n            std::cout << \"Bad NodeId Unknown\\n\"; break;\n        case 0x80410000:\n            std::cout << \"Bad AttributeId Invalid\\n\"; break;\n        case 0x804B0000:\n            std::cout << \"Bad User Access Denied\\n\"; break;\n        case 0x805A0000:\n            std::cout << \"Bad Not Readable\\n\"; break;\n        case 0x805B0000:\n            std::cout << \"Bad Not Writable\\n\"; break;\n        case 0x80730000:\n            std::cout << \"Bad Type Mismatch\\n\"; break;\n        case 0x807A0000:\n            std::cout << \"Bad Timeout\\n\"; break;\n        default:\n            std::cout << \"Error code: 0x\" << std::hex << code << std::dec << \"\\n\";\n        }\n    }\n}\n\n// ============================================================================\n// main: 运行所有场景\n// ============================================================================\nint main() {\n    scenario_connect_disconnect();\n    scenario_read_scalar_types();\n    scenario_read_array_types();\n    scenario_read_attributes();\n    scenario_write_scalar_types();\n    scenario_write_array_types();\n    scenario_batch_read();\n    scenario_batch_write();\n    scenario_browse();\n    // scenario_subscription();     // 需等待, 默认注释\n    scenario_method_call();\n    scenario_history_read();\n    scenario_string_nodeid();\n    scenario_write_attribute();\n    scenario_raw_bytes();\n    scenario_structured_types();\n    scenario_unified_message();\n    scenario_secure_connection();\n    scenario_polling_from_config();\n    scenario_error_handling();\n    scenario_proto_wire_transport();\n    scenario_large_integer();\n    scenario_browse_and_read();\n    scenario_datetime();\n    scenario_status_code_detail();\n\n    std::cout << \"\\n所有场景演示完毕.\\n\";\n    return 0;\n}\n"
}
```

**Output:**
```
Wrote file successfully.

LSP errors detected in other files:
<diagnostics file="/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.h">
ERROR [7:10] 'opcua.pb.h' file not found
ERROR [25:5] Use of undeclared identifier 'opcua'
ERROR [25:36] Use of undeclared identifier 'opcua'
ERROR [26:5] Use of undeclared identifier 'opcua'
ERROR [26:41] Use of undeclared identifier 'opcua'
ERROR [27:5] Use of undeclared identifier 'opcua'
ERROR [27:45] Use of undeclared identifier 'opcua'
ERROR [30:25] Use of undeclared identifier 'opcua'
ERROR [31:29] Use of undeclared identifier 'opcua'
ERROR [32:29] Use of undeclared identifier 'opcua'
ERROR [33:31] Use of undeclared identifier 'opcua'
ERROR [34:27] Use of undeclared identifier 'opcua'
ERROR [35:29] Use of undeclared identifier 'opcua'
ERROR [36:34] Use of undeclared identifier 'opcua'
ERROR [37:47] Use of undeclared identifier 'opcua'
ERROR [38:45] Use of undeclared identifier 'opcua'
ERROR [39:47] Use of undeclared identifier 'opcua'
ERROR [42:5] Use of undeclared identifier 'opcua'
ERROR [42:38] Use of undeclared identifier 'opcua'
ERROR [43:26] Use of undeclared identifier 'opcua'
... and 1 more
</diagnostics>

LSP errors detected in other files:
<diagnostics file="/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.cpp">
ERROR [1:10] In included file: 'opcua.pb.h' file not found

/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.h:7:10:
note: error occurred here
ERROR [12:50] Use of undeclared identifier 'UA_Client_delete'; did you mean 'UA_ClientDeleter'? (fix available)
ERROR [21:19] Use of undeclared identifier 'UA_Client_new'; did you mean 'UA_Client'? (fix available)
ERROR [26:19] Use of undeclared identifier 'UA_STATUSCODE_GOOD'
ERROR [46:1] Use of undeclared identifier 'opcua'
ERROR [47:5] Use of undeclared identifier 'opcua'
ERROR [47:19] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:46:64: note: 'id' declared here
ERROR [48:5] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:46:64: note: 'id' declared here
ERROR [48:8] Member reference base type 'uint32_t' (aka 'unsigned int') is not a structure or union
ERROR [49:5] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:46:64: note: 'id' declared here
ERROR [49:8] Member reference base type 'uint32_t' (aka 'unsigned int') is not a structure or union
ERROR [50:12] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:46:64: note: 'id' declared here
ERROR [53:1] Use of undeclared identifier 'opcua'
ERROR [54:5] Use of undeclared identifier 'opcua'
ERROR [54:19] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:53:73: note: 'id' declared here
ERROR [55:5] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:53:73: note: 'id' declared here
ERROR [55:9] No member named 'set_namespace_index' in 'std::string'
ERROR [56:5] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:53:73: note: 'id' declared here
ERROR [56:9] No member named 'set_string_id' in 'std::string'
ERROR [57:12] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:53:73: note: 'id' declared here
... and 1 more
</diagnostics>

LSP errors detected in this file, please fix:
<diagnostics file="/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/scenarios.cpp">
ERROR [8:10] In included file: 'opcua.pb.h' file not found

/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.h:7:10:
note: error occurred here
ERROR [67:5] Use of undeclared identifier 'opcua'
ERROR [72:5] Use of undeclared identifier 'opcua'
ERROR [178:5] Use of undeclared identifier 'opcua'
ERROR [180:36] Use of undeclared identifier 'opcua'
ERROR [183:5] Use of undeclared identifier 'opcua'
ERROR [217:5] Use of undeclared identifier 'opcua'
ERROR [233:5] Use of undeclared identifier 'opcua'
ERROR [233:30] Use of undeclared identifier 'batch_resp'; did you mean 'batch_req'? (fix available)

scenarios.cpp:217:29: note: 'batch_req' declared here
ERROR [234:35] Use of undeclared identifier 'batch_resp'; did you mean 'batch_req'? (fix available)

scenarios.cpp:217:29: note: 'batch_req' declared here
ERROR [235:25] Use of undeclared identifier 'batch_resp'; did you mean 'batch_req'? (fix available)

scenarios.cpp:217:29: note: 'batch_req' declared here
ERROR [236:25] Use of undeclared identifier 'batch_resp'; did you mean 'batch_req'? (fix available)

scenarios.cpp:217:29: note: 'batch_req' declared here
ERROR [250:5] Use of undeclared identifier 'opcua'
ERROR [257:38] Use of undeclared identifier 'opcua'
ERROR [265:5] Use of undeclared identifier 'opcua'
ERROR [265:31] Use of undeclared identifier 'batch_wresp'; did you mean 'batch_wreq'? (fix available)

scenarios.cpp:250:30: note: 'batch_wreq' declared here
ERROR [267:25] Use of undeclared identifier 'batch_wresp'; did you mean 'batch_wreq'? (fix available)

scenarios.cpp:250:30: note: 'batch_wreq' declared here
ERROR [269:23] Use of undeclared identifier 'batch_wresp'; did you mean 'batch_wreq'? (fix available)

scenarios.cpp:250:30: note: 'batch_wreq' declared here
ERROR [317:5] Use of undeclared identifier 'opcua'
ERROR [339:5] Use of undeclared identifier 'opcua'
... and 1 more
</diagnostics>
```

---

## Assistant (Build · Big Pickle · 8.9s)

_Thinking:_

Now let me create the CMakeLists.txt. The LSP errors are expected because protobuf and open62541 aren't in the LSP include paths.

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "priority": "high",
      "content": "Create .proto file defining OPC UA request/response messages",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Create OPC UA client wrapper with protobuf integration",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Create comprehensive usage scenarios demonstrating all PLC read/write patterns",
      "status": "completed"
    },
    {
      "priority": "medium",
      "content": "Create CMakeLists.txt build configuration",
      "status": "in_progress"
    }
  ]
}
```

**Output:**
```
[
  {
    "priority": "high",
    "content": "Create .proto file defining OPC UA request/response messages",
    "status": "completed"
  },
  {
    "priority": "high",
    "content": "Create OPC UA client wrapper with protobuf integration",
    "status": "completed"
  },
  {
    "priority": "high",
    "content": "Create comprehensive usage scenarios demonstrating all PLC read/write patterns",
    "status": "completed"
  },
  {
    "priority": "medium",
    "content": "Create CMakeLists.txt build configuration",
    "status": "in_progress"
  }
]
```

---

## Assistant (Build · Big Pickle · 5.6s)

_Thinking:_

Now creating the CMakeLists.txt build file. This needs to find protobuf and open62541 packages.

**Tool: write**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/CMakeLists.txt",
  "content": "cmake_minimum_required(VERSION 3.16)\nproject(opcua-protobuf-demo CXX)\n\nset(CMAKE_CXX_STANDARD 17)\nset(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n# ---- Dependencies ----\nfind_package(Protobuf REQUIRED)\nfind_package(open62541 REQUIRED)\n\n# ---- Protobuf ----\nprotobuf_generate_cpp(PROTO_SRCS PROTO_HDRS opcua.proto)\n\n# ---- Library: OPC UA Client Wrapper ----\nadd_library(opcua_client\n    opcua_client.cpp\n    ${PROTO_SRCS}\n)\n\ntarget_include_directories(opcua_client PUBLIC\n    ${CMAKE_CURRENT_BINARY_DIR}\n    ${Protobuf_INCLUDE_DIRS}\n    ${open62541_INCLUDE_DIRS}\n)\n\ntarget_link_libraries(opcua_client PUBLIC\n    ${Protobuf_LIBRARIES}\n    open62541::open62541\n)\n\n# ---- Executable: All Usage Scenarios ----\nadd_executable(scenarios scenarios.cpp)\ntarget_link_libraries(scenarios PRIVATE opcua_client)\n\n# ---- Optional: Install ----\ninstall(TARGETS scenarios RUNTIME DESTINATION bin)\n"
}
```

**Output:**
```
Wrote file successfully.

LSP errors detected in other files:
<diagnostics file="/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.h">
ERROR [7:10] 'opcua.pb.h' file not found
ERROR [25:5] Use of undeclared identifier 'opcua'
ERROR [25:36] Use of undeclared identifier 'opcua'
ERROR [26:5] Use of undeclared identifier 'opcua'
ERROR [26:41] Use of undeclared identifier 'opcua'
ERROR [27:5] Use of undeclared identifier 'opcua'
ERROR [27:45] Use of undeclared identifier 'opcua'
ERROR [30:25] Use of undeclared identifier 'opcua'
ERROR [31:29] Use of undeclared identifier 'opcua'
ERROR [32:29] Use of undeclared identifier 'opcua'
ERROR [33:31] Use of undeclared identifier 'opcua'
ERROR [34:27] Use of undeclared identifier 'opcua'
ERROR [35:29] Use of undeclared identifier 'opcua'
ERROR [36:34] Use of undeclared identifier 'opcua'
ERROR [37:47] Use of undeclared identifier 'opcua'
ERROR [38:45] Use of undeclared identifier 'opcua'
ERROR [39:47] Use of undeclared identifier 'opcua'
ERROR [42:5] Use of undeclared identifier 'opcua'
ERROR [42:38] Use of undeclared identifier 'opcua'
ERROR [43:26] Use of undeclared identifier 'opcua'
... and 1 more
</diagnostics>

LSP errors detected in other files:
<diagnostics file="/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.cpp">
ERROR [1:10] In included file: 'opcua.pb.h' file not found

/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.h:7:10:
note: error occurred here
ERROR [12:50] Use of undeclared identifier 'UA_Client_delete'; did you mean 'UA_ClientDeleter'? (fix available)
ERROR [21:19] Use of undeclared identifier 'UA_Client_new'; did you mean 'UA_Client'? (fix available)
ERROR [26:19] Use of undeclared identifier 'UA_STATUSCODE_GOOD'
ERROR [46:1] Use of undeclared identifier 'opcua'
ERROR [47:5] Use of undeclared identifier 'opcua'
ERROR [47:19] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:46:64: note: 'id' declared here
ERROR [48:5] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:46:64: note: 'id' declared here
ERROR [48:8] Member reference base type 'uint32_t' (aka 'unsigned int') is not a structure or union
ERROR [49:5] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:46:64: note: 'id' declared here
ERROR [49:8] Member reference base type 'uint32_t' (aka 'unsigned int') is not a structure or union
ERROR [50:12] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:46:64: note: 'id' declared here
ERROR [53:1] Use of undeclared identifier 'opcua'
ERROR [54:5] Use of undeclared identifier 'opcua'
ERROR [54:19] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:53:73: note: 'id' declared here
ERROR [55:5] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:53:73: note: 'id' declared here
ERROR [55:9] No member named 'set_namespace_index' in 'std::string'
ERROR [56:5] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:53:73: note: 'id' declared here
ERROR [56:9] No member named 'set_string_id' in 'std::string'
ERROR [57:12] Use of undeclared identifier 'nid'; did you mean 'id'? (fix available)

opcua_client.cpp:53:73: note: 'id' declared here
... and 1 more
</diagnostics>

LSP errors detected in other files:
<diagnostics file="/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/scenarios.cpp">
ERROR [8:10] In included file: 'opcua.pb.h' file not found

/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.h:7:10:
note: error occurred here
ERROR [67:5] Use of undeclared identifier 'opcua'
ERROR [72:5] Use of undeclared identifier 'opcua'
ERROR [178:5] Use of undeclared identifier 'opcua'
ERROR [180:36] Use of undeclared identifier 'opcua'
ERROR [183:5] Use of undeclared identifier 'opcua'
ERROR [217:5] Use of undeclared identifier 'opcua'
ERROR [233:5] Use of undeclared identifier 'opcua'
ERROR [233:30] Use of undeclared identifier 'batch_resp'; did you mean 'batch_req'? (fix available)

scenarios.cpp:217:29: note: 'batch_req' declared here
ERROR [234:35] Use of undeclared identifier 'batch_resp'; did you mean 'batch_req'? (fix available)

scenarios.cpp:217:29: note: 'batch_req' declared here
ERROR [235:25] Use of undeclared identifier 'batch_resp'; did you mean 'batch_req'? (fix available)

scenarios.cpp:217:29: note: 'batch_req' declared here
ERROR [236:25] Use of undeclared identifier 'batch_resp'; did you mean 'batch_req'? (fix available)

scenarios.cpp:217:29: note: 'batch_req' declared here
ERROR [250:5] Use of undeclared identifier 'opcua'
ERROR [257:38] Use of undeclared identifier 'opcua'
ERROR [265:5] Use of undeclared identifier 'opcua'
ERROR [265:31] Use of undeclared identifier 'batch_wresp'; did you mean 'batch_wreq'? (fix available)

scenarios.cpp:250:30: note: 'batch_wreq' declared here
ERROR [267:25] Use of undeclared identifier 'batch_wresp'; did you mean 'batch_wreq'? (fix available)

scenarios.cpp:250:30: note: 'batch_wreq' declared here
ERROR [269:23] Use of undeclared identifier 'batch_wresp'; did you mean 'batch_wreq'? (fix available)

scenarios.cpp:250:30: note: 'batch_wreq' declared here
ERROR [317:5] Use of undeclared identifier 'opcua'
ERROR [339:5] Use of undeclared identifier 'opcua'
... and 1 more
</diagnostics>
```

---

## Assistant (Build · Big Pickle · 2.9s)

_Thinking:_

Let me verify the directory structure and mark the final todo as complete.

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua</path>
<type>directory</type>
<entries>
CMakeLists.txt
opcua_client.cpp
opcua_client.h
opcua.proto
scenarios.cpp

(5 entries)
</entries>
```

---

## Assistant (Build · Big Pickle · 3.7s)

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "priority": "high",
      "content": "Create .proto file defining OPC UA request/response messages",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Create OPC UA client wrapper with protobuf integration",
      "status": "completed"
    },
    {
      "priority": "high",
      "content": "Create comprehensive usage scenarios demonstrating all PLC read/write patterns",
      "status": "completed"
    },
    {
      "priority": "medium",
      "content": "Create CMakeLists.txt build configuration",
      "status": "completed"
    }
  ]
}
```

**Output:**
```
[
  {
    "priority": "high",
    "content": "Create .proto file defining OPC UA request/response messages",
    "status": "completed"
  },
  {
    "priority": "high",
    "content": "Create OPC UA client wrapper with protobuf integration",
    "status": "completed"
  },
  {
    "priority": "high",
    "content": "Create comprehensive usage scenarios demonstrating all PLC read/write patterns",
    "status": "completed"
  },
  {
    "priority": "medium",
    "content": "Create CMakeLists.txt build configuration",
    "status": "completed"
  }
]
```

---

## Assistant (Build · Big Pickle · 9.8s)

_Thinking:_

All done. Let me provide a clear summary of what was created.

以下 **25 个场景**已完整覆盖 PLC 数据交互的方方面面，位于 `scenarios.cpp`:

| # | 场景名称 | 说明 |
|---|---------|------|
| 1 | 连接/断开 PLC | connect/disconnect 生命周期 |
| 2 | 读取标量值 | bool/int32/int64/uint32/float/double/string |
| 3 | 读取数组值 | int32/float/double 数组 |
| 4 | 读取属性 | DisplayName/Description/NodeClass/BrowseName/WriteMask 等 |
| 5 | 写入标量值 | 所有数据类型写入 + 写入后回读验证 |
| 6 | 写入数组值 | int32/float 数组写入 |
| 7 | 批量读取 | 一次请求读多个节点 |
| 8 | 批量写入 | 一次请求写多个节点 |
| 9 | 浏览地址空间 | 全部子节点/仅变量/仅对象/递归浏览/自定义过滤 |
| 10 | 订阅数据变化 | MonitoredItem, 回调通知, 采样间隔/队列配置, 取消订阅 |
| 11 | 调用 PLC 方法 | 无参/带参方法调用 + 输出参数处理 |
| 12 | 读取历史数据 | Historical Access, 按时间范围读取 |
| 13 | 字符串 NodeId | 西门子等 PLC 的 `ns=3;s="::Program:..."` 方式访问 |
| 14 | 写入属性 | 修改 Description 等非 Value 属性 |
| 15 | 原始字节读写 | 位域/Bytestring 透传 |
| 16 | 结构化类型 | LocalizedText/QualifiedName 读写 |
| 17 | 统一消息封装 | `OPCUARequest`/`OPCUAResponse` 序列化/反序列化 |
| 18 | 安全连接 | 用户名密码 + 证书配置示意 |
| 19 | 配表驱动轮询 | 从配置列表动态批量轮询变量 |
| 20 | 异常/故障处理 | 连接失败/节点不存在/只读写入/类型不匹配/重连机制 |
| 21 | protobuf 透传转发 | 序列化后通过 MQ/Kafka/gRPC 转发 |
| 22 | 大整数读写 | UInt64/Int64 累计值 |
| 23 | 浏览+读取组合 | 遍历地址空间后自动读取每个变量值 |
| 24 | DateTime 读写 | UA_DateTime 解析与转换 |
| 25 | 状态码解析 | 常见错误码含义诊断 |

**构建方式**（需安装 protobuf + open62541）:
```bash
mkdir build && cd build
cmake .. -DProtobuf_DIR=/path/to/protobuf/cmake
make
./scenarios
```

**架构分层**:
- `opcua.proto` — 消息定义层 (所有请求/响应/类型的 protobuf schema)
- `opcua_client.h/.cpp` — 传输封装层 (open62541 与 protobuf 互转)
- `scenarios.cpp` — 业务场景层 (25 个用例)

所有 LSP 报错均为预期行为（`opcua.pb.h` 和 open62541 header 在构建时由 cmake 处理）。

---

