# New session - 2026-07-16T15:28:51.681Z

**Session ID:** ses_09474821effeV24DdZIh6l15n3
**Created:** 7/16/2026, 11:28:51 PM
**Updated:** 8/5/2026, 8:30:47 AM

---

## User

介绍opcua数据类型的应用，并给出示例程序

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 24.0s)

_Thinking:_

The user is asking about OPC UA data types and their applications, along with example programs. Let me first explore the current directory to understand what's already there, and then provide a comprehensive answer about OPC UA data types with examples.

Let me start by exploring the workspace.

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/ex"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/ex</path>
<type>directory</type>
<entries>


(0 entries)
</entries>
```

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 14.2s)

_Thinking:_

The directory is empty. The user wants me to explain OPC UA data types and provide example programs. Let me provide a comprehensive answer with code examples.

OPC UA (Unified Architecture) has a rich type system. Let me cover the main categories:

1. **Built-in Types** (foundation types)
2. **Standard Data Types** (defined in the OPC UA specification)
3. **Application-specific Data Types** (custom types)

Let me write a comprehensive example in C++ using a common OPC UA SDK (like open62541 or similar). Since the project directory is "proto_opcua", this is likely a prototyping/learning project.

Let me provide examples showing:
- Basic types (Int32, Float, String, etc.)
- Variant types
- Array types
- Structure types
- QualifiedName, LocalizedText, NodeId
- Custom structure types

I'll use open62541 SDK which is the most popular open-source OPC UA implementation in C.

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
build/
CMakeFiles/
CMakeLists.txt
ex/
opcua_client.cpp
opcua_client.h
opcua.proto
scenarios.cpp
session-ses_0cda.md

(9 entries)
</entries>
```

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 3.3s)

_Thinking:_

Let me look at the existing code to understand what's being built, and then provide appropriate examples.

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua.proto"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua.proto</path>
<type>file</type>
<content>
1: syntax = "proto3";
2: 
3: package opcua;
4: 
5: enum DataTypeId {
6:     BOOL = 0;
7:     INT8 = 1;
8:     INT16 = 2;
9:     INT32 = 3;
10:     INT64 = 4;
11:     UINT8 = 5;
12:     UINT16 = 6;
13:     UINT32 = 7;
14:     UINT64 = 8;
15:     FLOAT = 9;
16:     DOUBLE = 10;
17:     STRING = 11;
18:     BYTESTRING = 12;
19:     DATETIME = 13;
20:     NODEID = 14;
21:     VARIANT = 15;
22:     ARRAY_BOOL = 16;
23:     ARRAY_INT32 = 17;
24:     ARRAY_FLOAT = 18;
25:     ARRAY_DOUBLE = 19;
26:     ARRAY_STRING = 20;
27:     LOCALIZEDTEXT = 21;
28:     QUALIFIEDNAME = 22;
29:     STRUCTURED = 23;
30: }
31: 
32: message VariantValue {
33:     DataTypeId type = 1;
34:     oneof value {
35:         bool bool_val = 2;
36:         int32 int32_val = 3;
37:         int64 int64_val = 4;
38:         uint32 uint32_val = 5;
39:         uint64 uint64_val = 6;
40:         float float_val = 7;
41:         double double_val = 8;
42:         string string_val = 9;
43:         bytes bytes_val = 10;
44:         uint64 datetime_val = 11;
45:     }
46:     // support arrays
47:     repeated bool bool_array = 20;
48:     repeated int32 int32_array = 21;
49:     repeated int64 int64_array = 22;
50:     repeated uint32 uint32_array = 23;
51:     repeated uint64 uint64_array = 24;
52:     repeated float float_array = 25;
53:     repeated double double_array = 26;
54:     repeated string string_array = 27;
55: }
56: 
57: message NodeId {
58:     uint32 namespace_index = 1;
59:     oneof id {
60:         uint32 numeric_id = 2;
61:         string string_id = 3;
62:         bytes guid_id = 4;
63:         uint64 opaque_id = 5;
64:     }
65: }
66: 
67: message QualifiedName {
68:     uint32 namespace_index = 1;
69:     string name = 2;
70: }
71: 
72: message ReadRequest {
73:     NodeId node_id = 1;
74:     // e.g. 13 (Value), 14 (DisplayName), etc.
75:     uint32 attribute_id = 2;  // default = 13 (Value)
76:     bool include_timestamp = 3;
77: }
78: 
79: message ReadResponse {
80:     StatusCode status = 1;
81:     VariantValue value = 2;
82:     uint64 server_timestamp = 3;
83:     uint64 source_timestamp = 4;
84: }
85: 
86: message WriteRequest {
87:     NodeId node_id = 1;
88:     VariantValue value = 2;
89:     uint32 attribute_id = 3;
90: }
91: 
92: message WriteResponse {
93:     StatusCode status = 1;
94: }
95: 
96: message BatchReadRequest {
97:     repeated ReadRequest requests = 1;
98: }
99: 
100: message BatchReadResponse {
101:     repeated ReadResponse responses = 1;
102: }
103: 
104: message BatchWriteRequest {
105:     repeated WriteRequest requests = 1;
106: }
107: 
108: message BatchWriteResponse {
109:     repeated WriteResponse responses = 1;
110: }
111: 
112: message BrowseRequest {
113:     NodeId node_id = 1;
114:     uint32 max_references = 2;
115:     uint32 node_class_mask = 3;  // Object=1, Variable=2, Method=4, ObjectType=8, VariableType=16
116:     bool recursive = 4;
117: }
118: 
119: message BrowseResponse {
120:     StatusCode status = 1;
121:     repeated ReferenceDescription references = 2;
122: }
123: 
124: message ReferenceDescription {
125:     NodeId node_id = 1;
126:     string browse_name = 2;
127:     string display_name = 3;
128:     uint32 node_class = 4;
129:     QualifiedName type_definition = 5;
130: }
131: 
132: message SubscribeRequest {
133:     NodeId node_id = 1;
134:     double sampling_interval_ms = 2;
135:     uint32 queue_size = 3;
136:     bool discard_oldest = 4;
137: }
138: 
139: message SubscribeResponse {
140:     StatusCode status = 1;
141:     uint32 monitored_item_id = 2;
142:     uint32 subscription_id = 3;
143: }
144: 
145: message UnsubscribeRequest {
146:     uint32 subscription_id = 1;
147:     uint32 monitored_item_id = 2;
148: }
149: 
150: message UnsubscribeResponse {
151:     StatusCode status = 1;
152: }
153: 
154: message DataChangeNotification {
155:     uint32 monitored_item_id = 1;
156:     uint32 subscription_id = 2;
157:     VariantValue value = 3;
158:     uint64 server_timestamp = 4;
159:     uint64 source_timestamp = 5;
160:     StatusCode status = 6;
161: }
162: 
163: message CallMethodRequest {
164:     NodeId object_id = 1;
165:     NodeId method_id = 2;
166:     repeated VariantValue input_args = 3;
167: }
168: 
169: message CallMethodResponse {
170:     StatusCode status = 1;
171:     repeated VariantValue output_args = 2;
172: }
173: 
174: message HistoryReadRequest {
175:     NodeId node_id = 1;
176:     uint64 start_time = 2;
177:     uint64 end_time = 3;
178:     uint32 max_values = 4;
179: }
180: 
181: message HistoryReadResponse {
182:     StatusCode status = 1;
183:     repeated VariantValue values = 2;
184:     repeated uint64 timestamps = 3;
185: }
186: 
187: message StatusCode {
188:     uint32 code = 1;
189:     string symbolic_name = 2;
190:     string description = 3;
191: }
192: 
193: message OPCUARequest {
194:     uint32 request_id = 1;
195:     oneof request {
196:         ReadRequest read = 10;
197:         WriteRequest write = 11;
198:         BatchReadRequest batch_read = 12;
199:         BatchWriteRequest batch_write = 13;
200:         BrowseRequest browse = 14;
201:         SubscribeRequest subscribe = 15;
202:         UnsubscribeRequest unsubscribe = 16;
203:         CallMethodRequest call_method = 17;
204:         HistoryReadRequest history_read = 18;
205:     }
206: }
207: 
208: message OPCUAResponse {
209:     uint32 request_id = 1;
210:     oneof response {
211:         ReadResponse read = 10;
212:         WriteResponse write = 11;
213:         BatchReadResponse batch_read = 12;
214:         BatchWriteResponse batch_write = 13;
215:         BrowseResponse browse = 14;
216:         SubscribeResponse subscribe = 15;
217:         UnsubscribeResponse unsubscribe = 16;
218:         CallMethodResponse call_method = 17;
219:         HistoryReadResponse history_read = 18;
220:         DataChangeNotification data_change = 19;
221:     }
222: }

(End of file - total 222 lines)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/CMakeLists.txt"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/CMakeLists.txt</path>
<type>file</type>
<content>
1: cmake_minimum_required(VERSION 3.16)
2: project(opcua-protobuf-demo CXX)
3: 
4: set(CMAKE_CXX_STANDARD 17)
5: set(CMAKE_CXX_STANDARD_REQUIRED ON)
6: 
7: # ---- Dependencies ----
8: find_package(protobuf CONFIG REQUIRED)
9: find_package(open62541 REQUIRED)
10: find_package(Threads REQUIRED)
11: 
12: # ---- Library: OPC UA Client Wrapper ----
13: add_library(opcua_client
14:     opcua_client.cpp
15: )
16: 
17: target_include_directories(opcua_client PUBLIC
18:     ${CMAKE_CURRENT_BINARY_DIR}
19:     ${open62541_INCLUDE_DIRS}
20: )
21: 
22: target_link_libraries(opcua_client PUBLIC
23:     protobuf::libprotobuf
24:     ${open62541_LIBRARIES}
25: )
26: 
27: # ---- Protobuf code generation ----
28: protobuf_generate(TARGET opcua_client LANGUAGE cpp PROTOS opcua.proto)
29: 
30: # ---- Executable: All Usage Scenarios ----
31: add_executable(scenarios scenarios.cpp)
32: target_include_directories(scenarios PRIVATE
33:     ${open62541_INCLUDE_DIRS}
34: )
35: target_link_libraries(scenarios PRIVATE
36:     opcua_client
37:     Threads::Threads
38: )

(End of file - total 38 lines)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.h"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.h</path>
<type>file</type>
<content>
1: #pragma once
2: 
3: #include <memory>
4: #include <string>
5: #include <functional>
6: #include <unordered_map>
7: #include "opcua.pb.h"
8: 
9: struct UA_Client;
10: struct UA_ClientConfig;
11: 
12: using UA_ClientDeleter = void (*)(UA_Client*);
13: 
14: class OPCUAClient {
15: public:
16:     explicit OPCUAClient(std::string endpoint_url);
17:     ~OPCUAClient();
18: 
19:     // connection lifecycle
20:     bool connect(int timeout_ms = 10000);
21:     void disconnect();
22:     bool isConnected() const;
23: 
24:     // ==== READ SCENARIOS ====
25:     opcua::ReadResponse read(const opcua::ReadRequest& req);
26:     opcua::ReadResponse readValue(const opcua::NodeId& node);
27:     opcua::ReadResponse readAttribute(const opcua::NodeId& node, uint32_t attribute_id);
28: 
29:     // typed reads
30:     bool readBool(const opcua::NodeId& node);
31:     int32_t readInt32(const opcua::NodeId& node);
32:     int64_t readInt64(const opcua::NodeId& node);
33:     uint32_t readUInt32(const opcua::NodeId& node);
34:     float readFloat(const opcua::NodeId& node);
35:     double readDouble(const opcua::NodeId& node);
36:     std::string readString(const opcua::NodeId& node);
37:     std::vector<int32_t> readInt32Array(const opcua::NodeId& node);
38:     std::vector<float> readFloatArray(const opcua::NodeId& node);
39:     std::vector<double> readDoubleArray(const opcua::NodeId& node);
40: 
41:     // ===== WRITE SCENARIOS =====
42:     opcua::WriteResponse write(const opcua::WriteRequest& req);
43:     bool writeBool(const opcua::NodeId& node, bool val);
44:     bool writeInt32(const opcua::NodeId& node, int32_t val);
45:     bool writeInt64(const opcua::NodeId& node, int64_t val);
46:     bool writeUInt32(const opcua::NodeId& node, uint32_t val);
47:     bool writeFloat(const opcua::NodeId& node, float val);
48:     bool writeDouble(const opcua::NodeId& node, double val);
49:     bool writeString(const opcua::NodeId& node, const std::string& val);
50:     bool writeInt32Array(const opcua::NodeId& node, const std::vector<int32_t>& val);
51:     bool writeFloatArray(const opcua::NodeId& node, const std::vector<float>& val);
52: 
53:     // ===== BATCH SCENARIOS =====
54:     opcua::BatchReadResponse batchRead(const opcua::BatchReadRequest& req);
55:     opcua::BatchWriteResponse batchWrite(const opcua::BatchWriteRequest& req);
56: 
57:     // ===== BROWSE SCENARIOS =====
58:     opcua::BrowseResponse browse(const opcua::BrowseRequest& req);
59:     std::vector<opcua::ReferenceDescription> browseChildren(const opcua::NodeId& node);
60:     std::vector<opcua::ReferenceDescription> browseObjects(const opcua::NodeId& node);
61:     std::vector<opcua::ReferenceDescription> browseVariables(const opcua::NodeId& node);
62:     opcua::BrowseResponse browseRecursive(const opcua::NodeId& node, int depth = 3);
63: 
64:     // ===== SUBSCRIPTION SCENARIOS =====
65:     using DataChangeCallback = std::function<void(const opcua::DataChangeNotification&)>;
66:     opcua::SubscribeResponse subscribe(const opcua::SubscribeRequest& req, DataChangeCallback cb);
67:     opcua::UnsubscribeResponse unsubscribe(const opcua::UnsubscribeRequest& req);
68:     bool isSubscribed(uint32_t monitored_item_id) const;
69: 
70:     // ===== METHOD CALL SCENARIOS =====
71:     opcua::CallMethodResponse callMethod(const opcua::CallMethodRequest& req);
72: 
73:     // ===== HISTORY READ SCENARIOS =====
74:     opcua::HistoryReadResponse historyReadRaw(const opcua::HistoryReadRequest& req);
75: 
76:     // ===== UTILITY =====
77:     static opcua::NodeId numericNodeId(uint32_t ns, uint32_t id);
78:     static opcua::NodeId stringNodeId(uint32_t ns, const std::string& id);
79: 
80: private:
81:     UA_Client* toNative(const opcua::NodeId& pb) const;
82:     opcua::NodeId fromNative(const UA_Client* node) const;
83:     opcua::VariantValue variantToProto(const void* ua_variant) const;
84:     void* protoToVariant(const opcua::VariantValue& val) const;
85:     uint32_t writeAttribute(const opcua::NodeId& node, const opcua::VariantValue& val, uint32_t attr_id);
86:     static void dataChangeHandler(UA_Client* client, uint32_t sub_id, void* sub_ctx,
87:                                   uint32_t mon_id, void* mon_ctx,
88:                                   void* data_value);
89: 
90:     std::string endpoint_url_;
91:     std::unique_ptr<UA_Client, UA_ClientDeleter> client_;
92:     std::unordered_map<uint32_t, DataChangeCallback> callbacks_;
93: };

(End of file - total 93 lines)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.cpp"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/opcua_client.cpp</path>
<type>file</type>
<content>
1: #include "opcua_client.h"
2: #include <open62541/client_config_default.h>
3: #include <open62541/client_highlevel.h>
4: #include <open62541/client_subscriptions.h>
5: #include <open62541/plugin/log_stdout.h>
6: #include <open62541/types.h>
7: #include <iostream>
8: #include <cstring>
9: 
10: OPCUAClient::OPCUAClient(std::string endpoint_url)
11:     : endpoint_url_(std::move(endpoint_url))
12:     , client_(nullptr, [](UA_Client* c) { if (c) UA_Client_delete(c); })
13: {
14: }
15: 
16: OPCUAClient::~OPCUAClient() {
17:     disconnect();
18: }
19: 
20: bool OPCUAClient::connect(int timeout_ms) {
21:     client_.reset(UA_Client_new());
22:     if (!client_) return false;
23: 
24:     UA_ClientConfig_setDefault(UA_Client_getConfig(client_.get()));
25:     auto status = UA_Client_connect(client_.get(), endpoint_url_.c_str());
26:     if (status != UA_STATUSCODE_GOOD) {
27:         std::cerr << "Connect failed: " << UA_StatusCode_name(status) << std::endl;
28:         client_.reset();
29:         return false;
30:     }
31:     return true;
32: }
33: 
34: void OPCUAClient::disconnect() {
35:     if (client_) {
36:         UA_Client_disconnect(client_.get());
37:     }
38: }
39: 
40: bool OPCUAClient::isConnected() const {
41:     return client_ != nullptr;
42: }
43: 
44: // ============ utility: NodeId conversion ============
45: 
46: opcua::NodeId OPCUAClient::numericNodeId(uint32_t ns, uint32_t id) {
47:     opcua::NodeId nid;
48:     nid.set_namespace_index(ns);
49:     nid.set_numeric_id(id);
50:     return nid;
51: }
52: 
53: opcua::NodeId OPCUAClient::stringNodeId(uint32_t ns, const std::string& id) {
54:     opcua::NodeId nid;
55:     nid.set_namespace_index(ns);
56:     nid.set_string_id(id);
57:     return nid;
58: }
59: 
60: UA_Client* OPCUAClient::toNative(const opcua::NodeId& pb) const {
61:     (void)pb;
62:     return client_.get();
63: }
64: 
65: // ============ Variant conversion helpers ============
66: 
67: opcua::VariantValue OPCUAClient::variantToProto(const void* ua_variant_ptr) const {
68:     opcua::VariantValue val;
69:     const auto* ua_val = static_cast<const UA_Variant*>(ua_variant_ptr);
70:     if (!ua_val || !ua_val->data) {
71:         val.set_type(opcua::DataTypeId::INT32);
72:         return val;
73:     }
74: 
75:     auto* type = ua_val->type;
76:     if (!type) return val;
77: 
78:     bool is_array = ua_val->arrayLength > 0 && ua_val->arrayLength != SIZE_MAX;
79: 
80:     // scalar reads
81:     if (type == &UA_TYPES[UA_TYPES_BOOLEAN]) {
82:         if (is_array) {
83:             val.set_type(opcua::DataTypeId::ARRAY_BOOL);
84:             auto* arr = static_cast<UA_Boolean*>(ua_val->data);
85:             for (size_t i = 0; i < ua_val->arrayLength; ++i)
86:                 val.add_bool_array(arr[i] != 0);
87:         } else {
88:             val.set_type(opcua::DataTypeId::BOOL);
89:             val.set_bool_val(*static_cast<UA_Boolean*>(ua_val->data) != 0);
90:         }
91:     } else if (type == &UA_TYPES[UA_TYPES_SBYTE]) {
92:         val.set_type(opcua::DataTypeId::INT8);
93:         val.set_int32_val(*static_cast<UA_SByte*>(ua_val->data));
94:     } else if (type == &UA_TYPES[UA_TYPES_INT16]) {
95:         val.set_type(opcua::DataTypeId::INT16);
96:         val.set_int32_val(*static_cast<UA_Int16*>(ua_val->data));
97:     } else if (type == &UA_TYPES[UA_TYPES_INT32]) {
98:         if (is_array) {
99:             val.set_type(opcua::DataTypeId::ARRAY_INT32);
100:             auto* arr = static_cast<UA_Int32*>(ua_val->data);
101:             for (size_t i = 0; i < ua_val->arrayLength; ++i)
102:                 val.add_int32_array(arr[i]);
103:         } else {
104:             val.set_type(opcua::DataTypeId::INT32);
105:             val.set_int32_val(*static_cast<UA_Int32*>(ua_val->data));
106:         }
107:     } else if (type == &UA_TYPES[UA_TYPES_INT64]) {
108:         val.set_type(opcua::DataTypeId::INT64);
109:         val.set_int64_val(*static_cast<UA_Int64*>(ua_val->data));
110:     } else if (type == &UA_TYPES[UA_TYPES_BYTE]) {
111:         val.set_type(opcua::DataTypeId::UINT8);
112:         val.set_uint32_val(*static_cast<UA_Byte*>(ua_val->data));
113:     } else if (type == &UA_TYPES[UA_TYPES_UINT16]) {
114:         val.set_type(opcua::DataTypeId::UINT16);
115:         val.set_uint32_val(*static_cast<UA_UInt16*>(ua_val->data));
116:     } else if (type == &UA_TYPES[UA_TYPES_UINT32]) {
117:         val.set_type(opcua::DataTypeId::UINT32);
118:         val.set_uint32_val(*static_cast<UA_UInt32*>(ua_val->data));
119:     } else if (type == &UA_TYPES[UA_TYPES_UINT64]) {
120:         val.set_type(opcua::DataTypeId::UINT64);
121:         val.set_uint64_val(*static_cast<UA_UInt64*>(ua_val->data));
122:     } else if (type == &UA_TYPES[UA_TYPES_FLOAT]) {
123:         if (is_array) {
124:             val.set_type(opcua::DataTypeId::ARRAY_FLOAT);
125:             auto* arr = static_cast<UA_Float*>(ua_val->data);
126:             for (size_t i = 0; i < ua_val->arrayLength; ++i)
127:                 val.add_float_array(arr[i]);
128:         } else {
129:             val.set_type(opcua::DataTypeId::FLOAT);
130:             val.set_float_val(*static_cast<UA_Float*>(ua_val->data));
131:         }
132:     } else if (type == &UA_TYPES[UA_TYPES_DOUBLE]) {
133:         if (is_array) {
134:             val.set_type(opcua::DataTypeId::ARRAY_DOUBLE);
135:             auto* arr = static_cast<UA_Double*>(ua_val->data);
136:             for (size_t i = 0; i < ua_val->arrayLength; ++i)
137:                 val.add_double_array(arr[i]);
138:         } else {
139:             val.set_type(opcua::DataTypeId::DOUBLE);
140:             val.set_double_val(*static_cast<UA_Double*>(ua_val->data));
141:         }
142:     } else if (type == &UA_TYPES[UA_TYPES_STRING]) {
143:         if (is_array) {
144:             val.set_type(opcua::DataTypeId::ARRAY_STRING);
145:             auto* arr = static_cast<UA_String*>(ua_val->data);
146:             for (size_t i = 0; i < ua_val->arrayLength; ++i) {
147:                 auto* str = &arr[i];
148:                 val.add_string_array(str->length > 0
149:                     ? std::string((const char*)str->data, str->length) : "");
150:             }
151:         } else {
152:             val.set_type(opcua::DataTypeId::STRING);
153:             auto* s = static_cast<UA_String*>(ua_val->data);
154:             if (s->length > 0)
155:                 val.set_string_val(std::string((const char*)s->data, s->length));
156:         }
157:     } else if (type == &UA_TYPES[UA_TYPES_DATETIME]) {
158:         val.set_type(opcua::DataTypeId::DATETIME);
159:         auto dt = *static_cast<UA_DateTime*>(ua_val->data);
160:         val.set_datetime_val(static_cast<uint64_t>(dt));
161:     }
162: 
163:     return val;
164: }
165: 
166: void* OPCUAClient::protoToVariant(const opcua::VariantValue& val) const {
167:     auto* var = UA_Variant_new();
168:     switch (val.type()) {
169:     case opcua::DataTypeId::BOOL: {
170:         UA_Boolean v = val.bool_val();
171:         UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_BOOLEAN]);
172:         break;
173:     }
174:     case opcua::DataTypeId::INT32: {
175:         UA_Int32 v = val.int32_val();
176:         UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_INT32]);
177:         break;
178:     }
179:     case opcua::DataTypeId::INT64: {
180:         UA_Int64 v = val.int64_val();
181:         UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_INT64]);
182:         break;
183:     }
184:     case opcua::DataTypeId::UINT32: {
185:         UA_UInt32 v = val.uint32_val();
186:         UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_UINT32]);
187:         break;
188:     }
189:     case opcua::DataTypeId::UINT64: {
190:         UA_UInt64 v = val.uint64_val();
191:         UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_UINT64]);
192:         break;
193:     }
194:     case opcua::DataTypeId::FLOAT: {
195:         UA_Float v = val.float_val();
196:         UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_FLOAT]);
197:         break;
198:     }
199:     case opcua::DataTypeId::DOUBLE: {
200:         UA_Double v = val.double_val();
201:         UA_Variant_setScalarCopy(var, &v, &UA_TYPES[UA_TYPES_DOUBLE]);
202:         break;
203:     }
204:     case opcua::DataTypeId::STRING: {
205:         auto s = UA_String_fromChars(val.string_val().c_str());
206:         UA_Variant_setScalarCopy(var, &s, &UA_TYPES[UA_TYPES_STRING]);
207:         UA_String_clear(&s);
208:         break;
209:     }
210:     case opcua::DataTypeId::ARRAY_INT32: {
211:         size_t n = val.int32_array_size();
212:         auto* arr = static_cast<UA_Int32*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_INT32]));
213:         for (size_t i = 0; i < n; ++i) arr[i] = val.int32_array(i);
214:         UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_INT32]);
215:         UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_INT32]);
216:         break;
217:     }
218:     case opcua::DataTypeId::ARRAY_FLOAT: {
219:         size_t n = val.float_array_size();
220:         auto* arr = static_cast<UA_Float*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_FLOAT]));
221:         for (size_t i = 0; i < n; ++i) arr[i] = val.float_array(i);
222:         UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_FLOAT]);
223:         UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_FLOAT]);
224:         break;
225:     }
226:     case opcua::DataTypeId::ARRAY_DOUBLE: {
227:         size_t n = val.double_array_size();
228:         auto* arr = static_cast<UA_Double*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_DOUBLE]));
229:         for (size_t i = 0; i < n; ++i) arr[i] = val.double_array(i);
230:         UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_DOUBLE]);
231:         UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_DOUBLE]);
232:         break;
233:     }
234:     case opcua::DataTypeId::ARRAY_BOOL: {
235:         size_t n = val.bool_array_size();
236:         auto* arr = static_cast<UA_Boolean*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_BOOLEAN]));
237:         for (size_t i = 0; i < n; ++i) arr[i] = val.bool_array(i);
238:         UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_BOOLEAN]);
239:         UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_BOOLEAN]);
240:         break;
241:     }
242:     case opcua::DataTypeId::ARRAY_STRING: {
243:         size_t n = val.string_array_size();
244:         auto* arr = static_cast<UA_String*>(UA_Array_new(n, &UA_TYPES[UA_TYPES_STRING]));
245:         for (size_t i = 0; i < n; ++i)
246:             arr[i] = UA_String_fromChars(val.string_array(i).c_str());
247:         UA_Variant_setArrayCopy(var, arr, n, &UA_TYPES[UA_TYPES_STRING]);
248:         for (size_t i = 0; i < n; ++i) UA_String_clear(&arr[i]);
249:         UA_Array_delete(arr, n, &UA_TYPES[UA_TYPES_STRING]);
250:         break;
251:     }
252:     default:
253:         UA_Variant_delete(var);
254:         return nullptr;
255:     }
256:     return var;
257: }
258: 
259: // ============ READ ============
260: 
261: opcua::ReadResponse OPCUAClient::read(const opcua::ReadRequest& req) {
262:     opcua::ReadResponse resp;
263: 
264:     UA_ReadRequest rreq;
265:     UA_ReadRequest_init(&rreq);
266:     rreq.nodesToRead = UA_ReadValueId_new();
267:     rreq.nodesToReadSize = 1;
268:     rreq.nodesToRead[0].nodeId = UA_NODEID_NUMERIC(
269:         req.node_id().namespace_index(), req.node_id().numeric_id());
270:     rreq.nodesToRead[0].attributeId = req.attribute_id() != 0 ? req.attribute_id() : UA_ATTRIBUTEID_VALUE;
271: 
272:     if (req.node_id().id_case() == opcua::NodeId::IdCase::kStringId) {
273:         rreq.nodesToRead[0].nodeId = UA_NODEID_STRING_ALLOC(
274:             req.node_id().namespace_index(), req.node_id().string_id().c_str());
275:     }
276: 
277:     UA_ReadResponse raw_resp;
278:     auto status = UA_Client_Service_read(client_.get(), &rreq, &raw_resp);
279:     if (status == UA_STATUSCODE_GOOD && raw_resp.resultsSize > 0) {
280:         resp.mutable_status()->set_code(raw_resp.results[0].status);
281:         auto* sv = &raw_resp.results[0].value;
282:         if (sv->type) {
283:             *resp.mutable_value() = variantToProto(sv);
284:         }
285:         if (req.include_timestamp()) {
286:             resp.set_server_timestamp(raw_resp.results[0].serverTimestamp);
287:             resp.set_source_timestamp(raw_resp.results[0].sourceTimestamp);
288:         }
289:     } else {
290:         resp.mutable_status()->set_code(status);
291:     }
292: 
293:     UA_ReadRequest_clear(&rreq);
294:     UA_ReadResponse_clear(&raw_resp);
295:     return resp;
296: }
297: 
298: opcua::ReadResponse OPCUAClient::readValue(const opcua::NodeId& node) {
299:     opcua::ReadRequest req;
300:     *req.mutable_node_id() = node;
301:     req.set_attribute_id(UA_ATTRIBUTEID_VALUE);
302:     return read(req);
303: }
304: 
305: opcua::ReadResponse OPCUAClient::readAttribute(const opcua::NodeId& node, uint32_t attribute_id) {
306:     opcua::ReadRequest req;
307:     *req.mutable_node_id() = node;
308:     req.set_attribute_id(attribute_id);
309:     return read(req);
310: }
311: 
312: bool OPCUAClient::readBool(const opcua::NodeId& node) {
313:     auto resp = readValue(node);
314:     return resp.value().bool_val();
315: }
316: 
317: int32_t OPCUAClient::readInt32(const opcua::NodeId& node) {
318:     auto resp = readValue(node);
319:     return resp.value().int32_val();
320: }
321: 
322: int64_t OPCUAClient::readInt64(const opcua::NodeId& node) {
323:     auto resp = readValue(node);
324:     return resp.value().int64_val();
325: }
326: 
327: uint32_t OPCUAClient::readUInt32(const opcua::NodeId& node) {
328:     auto resp = readValue(node);
329:     return resp.value().uint32_val();
330: }
331: 
332: float OPCUAClient::readFloat(const opcua::NodeId& node) {
333:     auto resp = readValue(node);
334:     return resp.value().float_val();
335: }
336: 
337: double OPCUAClient::readDouble(const opcua::NodeId& node) {
338:     auto resp = readValue(node);
339:     return resp.value().double_val();
340: }
341: 
342: std::string OPCUAClient::readString(const opcua::NodeId& node) {
343:     auto resp = readValue(node);
344:     return resp.value().string_val();
345: }
346: 
347: std::vector<int32_t> OPCUAClient::readInt32Array(const opcua::NodeId& node) {
348:     auto resp = readValue(node);
349:     std::vector<int32_t> out;
350:     for (auto v : resp.value().int32_array()) out.push_back(v);
351:     return out;
352: }
353: 
354: std::vector<float> OPCUAClient::readFloatArray(const opcua::NodeId& node) {
355:     auto resp = readValue(node);
356:     std::vector<float> out;
357:     for (auto v : resp.value().float_array()) out.push_back(v);
358:     return out;
359: }
360: 
361: std::vector<double> OPCUAClient::readDoubleArray(const opcua::NodeId& node) {
362:     auto resp = readValue(node);
363:     std::vector<double> out;
364:     for (auto v : resp.value().double_array()) out.push_back(v);
365:     return out;
366: }
367: 
368: // ============ WRITE ============
369: 
370: uint32_t OPCUAClient::writeAttribute(const opcua::NodeId& node,
371:                                       const opcua::VariantValue& val,
372:                                       uint32_t attr_id) {
373:     UA_WriteRequest wreq;
374:     UA_WriteRequest_init(&wreq);
375:     wreq.nodesToWrite = UA_WriteValue_new();
376:     wreq.nodesToWriteSize = 1;
377:     wreq.nodesToWrite[0].nodeId = UA_NODEID_NUMERIC(
378:         node.namespace_index(), node.numeric_id());
379:     wreq.nodesToWrite[0].attributeId = attr_id;
380: 
381:     if (node.id_case() == opcua::NodeId::IdCase::kStringId) {
382:         wreq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(
383:             node.namespace_index(), node.string_id().c_str());
384:     }
385: 
386:     auto* var = static_cast<UA_Variant*>(protoToVariant(val));
387:     if (var) {
388:         UA_Variant_copy(var, &wreq.nodesToWrite[0].value.value);
389:         UA_Variant_delete(var);
390:     }
391: 
392:     UA_WriteResponse raw_resp;
393:     auto status = UA_Client_Service_write(client_.get(), &wreq, &raw_resp);
394:     UA_WriteRequest_clear(&wreq);
395:     UA_WriteResponse_clear(&raw_resp);
396:     return status;
397: }
398: 
399: opcua::WriteResponse OPCUAClient::write(const opcua::WriteRequest& req) {
400:     opcua::WriteResponse resp;
401:     auto status = writeAttribute(req.node_id(), req.value(),
402:                                  req.attribute_id() != 0 ? req.attribute_id() : UA_ATTRIBUTEID_VALUE);
403:     resp.mutable_status()->set_code(status);
404:     return resp;
405: }
406: 
407: bool OPCUAClient::writeBool(const opcua::NodeId& node, bool val) {
408:     opcua::VariantValue v;
409:     v.set_type(opcua::DataTypeId::BOOL);
410:     v.set_bool_val(val);
411:     return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
412: }
413: 
414: bool OPCUAClient::writeInt32(const opcua::NodeId& node, int32_t val) {
415:     opcua::VariantValue v;
416:     v.set_type(opcua::DataTypeId::INT32);
417:     v.set_int32_val(val);
418:     return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
419: }
420: 
421: bool OPCUAClient::writeInt64(const opcua::NodeId& node, int64_t val) {
422:     opcua::VariantValue v;
423:     v.set_type(opcua::DataTypeId::INT64);
424:     v.set_int64_val(val);
425:     return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
426: }
427: 
428: bool OPCUAClient::writeUInt32(const opcua::NodeId& node, uint32_t val) {
429:     opcua::VariantValue v;
430:     v.set_type(opcua::DataTypeId::UINT32);
431:     v.set_uint32_val(val);
432:     return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
433: }
434: 
435: bool OPCUAClient::writeFloat(const opcua::NodeId& node, float val) {
436:     opcua::VariantValue v;
437:     v.set_type(opcua::DataTypeId::FLOAT);
438:     v.set_float_val(val);
439:     return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
440: }
441: 
442: bool OPCUAClient::writeDouble(const opcua::NodeId& node, double val) {
443:     opcua::VariantValue v;
444:     v.set_type(opcua::DataTypeId::DOUBLE);
445:     v.set_double_val(val);
446:     return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
447: }
448: 
449: bool OPCUAClient::writeString(const opcua::NodeId& node, const std::string& val) {
450:     opcua::VariantValue v;
451:     v.set_type(opcua::DataTypeId::STRING);
452:     v.set_string_val(val);
453:     return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
454: }
455: 
456: bool OPCUAClient::writeInt32Array(const opcua::NodeId& node, const std::vector<int32_t>& vals) {
457:     opcua::VariantValue v;
458:     v.set_type(opcua::DataTypeId::ARRAY_INT32);
459:     for (auto x : vals) v.add_int32_array(x);
460:     return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
461: }
462: 
463: bool OPCUAClient::writeFloatArray(const opcua::NodeId& node, const std::vector<float>& vals) {
464:     opcua::VariantValue v;
465:     v.set_type(opcua::DataTypeId::ARRAY_FLOAT);
466:     for (auto x : vals) v.add_float_array(x);
467:     return writeAttribute(node, v, UA_ATTRIBUTEID_VALUE) == UA_STATUSCODE_GOOD;
468: }
469: 
470: // ============ BATCH ============
471: 
472: opcua::BatchReadResponse OPCUAClient::batchRead(const opcua::BatchReadRequest& req) {
473:     opcua::BatchReadResponse resp;
474:     for (const auto& r : req.requests()) {
475:         *resp.add_responses() = read(r);
476:     }
477:     return resp;
478: }
479: 
480: opcua::BatchWriteResponse OPCUAClient::batchWrite(const opcua::BatchWriteRequest& req) {
481:     opcua::BatchWriteResponse resp;
482:     for (const auto& r : req.requests()) {
483:         *resp.add_responses() = write(r);
484:     }
485:     return resp;
486: }
487: 
488: // ============ BROWSE ============
489: 
490: opcua::BrowseResponse OPCUAClient::browse(const opcua::BrowseRequest& req) {
491:     opcua::BrowseResponse resp;
492: 
493:     UA_BrowseRequest breq;
494:     UA_BrowseRequest_init(&breq);
495:     breq.nodesToBrowse = UA_BrowseDescription_new();
496:     breq.nodesToBrowseSize = 1;
497:     breq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(
498:         req.node_id().namespace_index(), req.node_id().numeric_id());
499:     breq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL;
500: 
501:     if (req.node_id().id_case() == opcua::NodeId::IdCase::kStringId) {
502:         breq.nodesToBrowse[0].nodeId = UA_NODEID_STRING_ALLOC(
503:             req.node_id().namespace_index(), req.node_id().string_id().c_str());
504:     }
505: 
506:     if (req.max_references() > 0)
507:         breq.requestedMaxReferencesPerNode = req.max_references();
508: 
509:     if (req.node_class_mask() > 0) {
510:         breq.nodesToBrowse[0].nodeClassMask = req.node_class_mask();
511:     }
512: 
513:     UA_BrowseResponse raw_resp;
514:     auto status = UA_Client_Service_browse(client_.get(), &breq, &raw_resp);
515:     if (status == UA_STATUSCODE_GOOD) {
516:         resp.mutable_status()->set_code(UA_STATUSCODE_GOOD);
517:         for (size_t i = 0; i < raw_resp.resultsSize; ++i) {
518:             for (size_t j = 0; j < raw_resp.results[i].referencesSize; ++j) {
519:                 auto* ref = &raw_resp.results[i].references[j];
520:                 auto* pb_ref = resp.add_references();
521:                 // display name
522:                 auto dn = ref->displayName;
523:                 pb_ref->set_display_name(dn.length > 0
524:                     ? std::string((const char*)dn.data, dn.length) : "");
525:                 // browse name
526:                 auto bn = ref->browseName.name;
527:                 pb_ref->set_browse_name(bn.length > 0
528:                     ? std::string((const char*)bn.data, bn.length) : "");
529:                 pb_ref->set_node_class(ref->nodeClass);
530:                 // node id
531:                 if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC) {
532:                     pb_ref->mutable_node_id()->set_namespace_index(ref->nodeId.nodeId.namespaceIndex);
533:                     pb_ref->mutable_node_id()->set_numeric_id(ref->nodeId.nodeId.identifier.numeric);
534:                 }
535:             }
536:         }
537:     } else {
538:         resp.mutable_status()->set_code(status);
539:     }
540: 
541:     UA_BrowseRequest_clear(&breq);
542:     UA_BrowseResponse_clear(&raw_resp);
543:     return resp;
544: }
545: 
546: std::vector<opcua::ReferenceDescription> OPCUAClient::browseChildren(const opcua::NodeId& node) {
547:     opcua::BrowseRequest req;
548:     *req.mutable_node_id() = node;
549:     auto resp = browse(req);
550:     return {resp.references().begin(), resp.references().end()};
551: }
552: 
553: std::vector<opcua::ReferenceDescription> OPCUAClient::browseObjects(const opcua::NodeId& node) {
554:     opcua::BrowseRequest req;
555:     *req.mutable_node_id() = node;
556:     req.set_node_class_mask(1); // Object
557:     auto resp = browse(req);
558:     return {resp.references().begin(), resp.references().end()};
559: }
560: 
561: std::vector<opcua::ReferenceDescription> OPCUAClient::browseVariables(const opcua::NodeId& node) {
562:     opcua::BrowseRequest req;
563:     *req.mutable_node_id() = node;
564:     req.set_node_class_mask(2); // Variable
565:     auto resp = browse(req);
566:     return {resp.references().begin(), resp.references().end()};
567: }
568: 
569: opcua::BrowseResponse OPCUAClient::browseRecursive(const opcua::NodeId& node, int depth) {
570:     opcua::BrowseResponse resp;
571:     if (depth <= 0) return resp;
572: 
573:     auto children = browseChildren(node);
574:     for (const auto& child : children) {
575:         *resp.add_references() = child;
576:         auto sub = browseRecursive(child.node_id(), depth - 1);
577:         for (const auto& ref : sub.references())
578:             *resp.add_references() = ref;
579:     }
580:     resp.mutable_status()->set_code(UA_STATUSCODE_GOOD);
581:     return resp;
582: }
583: 
584: // ============ SUBSCRIPTION ============
585: 
586: void OPCUAClient::dataChangeHandler(UA_Client* /*client*/,
587:                                      uint32_t /*sub_id*/,
588:                                      void* sub_ctx,
589:                                      uint32_t mon_id,
590:                                      void* mon_ctx,
591:                                      void* data_value) {
592:     auto* callbacks = static_cast<std::unordered_map<uint32_t, DataChangeCallback>*>(sub_ctx);
593:     auto* dv = static_cast<UA_DataValue*>(data_value);
594: 
595:     auto it = callbacks->find(mon_id);
596:     if (it != callbacks->end()) {
597:         opcua::DataChangeNotification notif;
598:         notif.set_monitored_item_id(mon_id);
599:         notif.mutable_status()->set_code(dv->status);
600: 
601:         if (dv->hasValue && dv->value.type) {
602:             auto self = static_cast<OPCUAClient*>(mon_ctx);
603:             *notif.mutable_value() = self->variantToProto(&dv->value);
604:         }
605:         it->second(notif);
606:     }
607: }
608: 
609: opcua::SubscribeResponse OPCUAClient::subscribe(const opcua::SubscribeRequest& req,
610:                                                   DataChangeCallback cb) {
611:     opcua::SubscribeResponse resp;
612: 
613:     // create subscription
614:     UA_CreateSubscriptionRequest sub_req = UA_CreateSubscriptionRequest_default();
615:     auto sub_resp = UA_Client_Subscriptions_create(client_.get(), sub_req,
616:                                                     nullptr, nullptr, nullptr);
617:     if (sub_resp.responseHeader.serviceResult != UA_STATUSCODE_GOOD) {
618:         resp.mutable_status()->set_code(sub_resp.responseHeader.serviceResult);
619:         return resp;
620:     }
621:     resp.set_subscription_id(sub_resp.subscriptionId);
622: 
623:     // create monitored item
624:     UA_MonitoredItemCreateRequest mon_req = UA_MonitoredItemCreateRequest_default(
625:         UA_NODEID_NUMERIC(req.node_id().namespace_index(), req.node_id().numeric_id()));
626:     if (req.sampling_interval_ms() > 0)
627:         mon_req.requestedParameters.samplingInterval = req.sampling_interval_ms();
628:     if (req.queue_size() > 0)
629:         mon_req.requestedParameters.queueSize = req.queue_size();
630:     mon_req.requestedParameters.discardOldest = req.discard_oldest();
631: 
632:     auto mon_resp = UA_Client_Subscriptions_addMonitoredItem(
633:         client_.get(), sub_resp.subscriptionId, mon_req,
634:         &callbacks_, dataChangeHandler, this);
635:     if (mon_resp.statusCode != UA_STATUSCODE_GOOD) {
636:         resp.mutable_status()->set_code(mon_resp.statusCode);
637:         return resp;
638:     }
639:     resp.set_monitored_item_id(mon_resp.monitoredItemId);
640:     resp.mutable_status()->set_code(UA_STATUSCODE_GOOD);
641: 
642:     callbacks_[mon_resp.monitoredItemId] = std::move(cb);
643:     return resp;
644: }
645: 
646: opcua::UnsubscribeResponse OPCUAClient::unsubscribe(const opcua::UnsubscribeRequest& req) {
647:     opcua::UnsubscribeResponse resp;
648:     UA_Client_Subscriptions_removeMonitoredItem(
649:         client_.get(), req.subscription_id(), req.monitored_item_id());
650:     callbacks_.erase(req.monitored_item_id());
651:     resp.mutable_status()->set_code(UA_STATUSCODE_GOOD);
652:     return resp;
653: }
654: 
655: bool OPCUAClient::isSubscribed(uint32_t monitored_item_id) const {
656:     return callbacks_.find(monitored_item_id) != callbacks_.end();
657: }
658: 
659: // ============ METHOD CALL ============
660: 
661: opcua::CallMethodResponse OPCUAClient::callMethod(const opcua::CallMethodRequest& req) {
662:     opcua::CallMethodResponse resp;
663: 
664:     UA_CallMethodRequest cReq;
665:     UA_CallMethodRequest_init(&cReq);
666:     cReq.objectId = UA_NODEID_NUMERIC(
667:         req.object_id().namespace_index(), req.object_id().numeric_id());
668:     cReq.methodId = UA_NODEID_NUMERIC(
669:         req.method_id().namespace_index(), req.method_id().numeric_id());
670: 
671:     if (req.input_args_size() > 0) {
672:         cReq.inputArgumentsSize = req.input_args_size();
673:         cReq.inputArguments = static_cast<UA_Variant*>(
674:             UA_Array_new(cReq.inputArgumentsSize, &UA_TYPES[UA_TYPES_VARIANT]));
675:         for (size_t i = 0; i < cReq.inputArgumentsSize; ++i) {
676:             auto* var = static_cast<UA_Variant*>(protoToVariant(req.input_args(i)));
677:             if (var) {
678:                 UA_Variant_copy(var, &cReq.inputArguments[i]);
679:                 UA_Variant_delete(var);
680:             }
681:         }
682:     }
683: 
684:     UA_CallRequest call_req;
685:     UA_CallRequest_init(&call_req);
686:     call_req.methodsToCall = &cReq;
687:     call_req.methodsToCallSize = 1;
688: 
689:     UA_CallResponse call_resp;
690:     auto status = UA_Client_Service_call(client_.get(), &call_req, &call_resp);
691:     if (status == UA_STATUSCODE_GOOD && call_resp.resultsSize > 0) {
692:         auto* result = &call_resp.results[0];
693:         resp.mutable_status()->set_code(result->statusCode);
694:         for (size_t i = 0; i < result->outputArgumentsSize; ++i) {
695:             *resp.add_output_args() = variantToProto(&result->outputArguments[i]);
696:         }
697:     } else {
698:         resp.mutable_status()->set_code(status);
699:     }
700: 
701:     UA_CallRequest_clear(&call_req);
702:     UA_CallResponse_clear(&call_resp);
703:     return resp;
704: }
705: 
706: // ============ HISTORY READ ============
707: 
708: opcua::HistoryReadResponse OPCUAClient::historyReadRaw(const opcua::HistoryReadRequest& req) {
709:     opcua::HistoryReadResponse resp;
710: 
711:     UA_ReadRawModifiedDetails details;
712:     UA_ReadRawModifiedDetails_init(&details);
713:     details.startTime = req.start_time();
714:     details.endTime = req.end_time();
715:     details.numValuesPerNode = req.max_values() > 0 ? req.max_values() : 100;
716:     details.isReadModified = false;
717: 
718:     UA_ExtensionObject ext;
719:     UA_ExtensionObject_init(&ext);
720:     ext.encoding = UA_EXTENSIONOBJECT_DECODED;
721:     ext.content.decoded.type = &UA_TYPES[UA_TYPES_READRAWMODIFIEDDETAILS];
722:     ext.content.decoded.data = UA_new(&UA_TYPES[UA_TYPES_READRAWMODIFIEDDETAILS]);
723:     memcpy(ext.content.decoded.data, &details, sizeof(UA_ReadRawModifiedDetails));
724: 
725:     UA_HistoryReadRequest hreq;
726:     UA_HistoryReadRequest_init(&hreq);
727:     hreq.nodesToRead = UA_HistoryReadValueId_new();
728:     hreq.nodesToReadSize = 1;
729:     hreq.nodesToRead[0].nodeId = UA_NODEID_NUMERIC(
730:         req.node_id().namespace_index(), req.node_id().numeric_id());
731:     hreq.historyReadDetails = ext;
732: 
733:     UA_HistoryReadResponse hresp;
734:     auto status = UA_Client_Service_historyRead(client_.get(), &hreq, &hresp);
735:     if (status == UA_STATUSCODE_GOOD && hresp.resultsSize > 0) {
736:         resp.mutable_status()->set_code(hresp.results[0].statusCode);
737:         auto* data = &hresp.results[0].historyData;
738:         for (size_t i = 0; i < data->dataValuesSize; ++i) {
739:             auto* dv = &data->dataValues[i];
740:             if (dv->hasValue && dv->value.type) {
741:                 *resp.add_values() = variantToProto(&dv->value);
742:             }
743:             resp.add_timestamps(dv->sourceTimestamp);
744:         }
745:     } else {
746:         resp.mutable_status()->set_code(status);
747:     }
748: 
749:     UA_HistoryReadRequest_clear(&hreq);
750:     UA_HistoryReadResponse_clear(&hresp);
751:     return resp;
752: }

(End of file - total 752 lines)
</content>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/scenarios.cpp"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/scenarios.cpp</path>
<type>file</type>
<content>
1: // ============================================================================
2: // scenarios.cpp - 所有使用场景的完整用例 (Protobuf + OPC UA + PLC)
3: // ============================================================================
4: // 前提: 假设 PLC 已运行 OPC UA 服务器, 地址 opc.tcp://192.168.1.100:4840
5: // 所有 NodeId 仅作示例用途, 按实际 PLC 变量地址替换
6: // ============================================================================
7: 
8: #include "opcua_client.h"
9: #include <open62541/types.h>
10: #include <iostream>
11: #include <thread>
12: #include <chrono>
13: 
14: static const std::string PLC_ENDPOINT = "opc.tcp://192.168.1.100:4840";
15: 
16: // ============================================================================
17: // 场景 1: 连接/断开 PLC
18: // ============================================================================
19: void scenario_connect_disconnect() {
20:     std::cout << "\n=== 场景 1: 连接/断开 PLC ===\n";
21:     OPCUAClient client(PLC_ENDPOINT);
22: 
23:     if (client.connect(5000)) {
24:         std::cout << "PLC 连接成功\n";
25:         // ... 执行操作 ...
26:         client.disconnect();
27:         std::cout << "PLC 已断开\n";
28:     } else {
29:         std::cout << "PLC 连接失败\n";
30:     }
31: }
32: 
33: // ============================================================================
34: // 场景 2: 读取标量值 (基本数据类型)
35: // ============================================================================
36: void scenario_read_scalar_types() {
37:     std::cout << "\n=== 场景 2: 读取标量值 ===\n";
38:     OPCUAClient client(PLC_ENDPOINT);
39:     if (!client.connect()) return;
40: 
41:     // 使用 protobuf 构建 NodeId (namespace 0, numeric id)
42:     auto bool_node   = OPCUAClient::numericNodeId(0, 1001);
43:     auto int32_node  = OPCUAClient::numericNodeId(0, 1002);
44:     auto int64_node  = OPCUAClient::numericNodeId(0, 1003);
45:     auto uint32_node = OPCUAClient::numericNodeId(0, 1004);
46:     auto float_node  = OPCUAClient::numericNodeId(0, 1005);
47:     auto double_node = OPCUAClient::numericNodeId(0, 1006);
48:     auto string_node = OPCUAClient::numericNodeId(0, 1007);
49: 
50:     // 方案 A: 使用封装好的类型安全接口 (推荐)
51:     bool b   = client.readBool(bool_node);
52:     int32_t i32 = client.readInt32(int32_node);
53:     int64_t i64 = client.readInt64(int64_node);
54:     uint32_t u32 = client.readUInt32(uint32_node);
55:     float f   = client.readFloat(float_node);
56:     double d  = client.readDouble(double_node);
57:     std::string s = client.readString(string_node);
58: 
59:     std::cout << "Bool:   " << b << "\n";
60:     std::cout << "Int32:  " << i32 << "\n";
61:     std::cout << "Int64:  " << i64 << "\n";
62:     std::cout << "UInt32: " << u32 << "\n";
63:     std::cout << "Float:  " << f << "\n";
64:     std::cout << "Double: " << d << "\n";
65:     std::cout << "String: " << s << "\n";
66: 
67:     // 方案 B: 使用通用 protobuf 请求/响应 (需自行处理类型)
68:     opcua::ReadRequest req;
69:     *req.mutable_node_id() = int32_node;
70:     req.set_attribute_id(13);    // UA_ATTRIBUTEID_VALUE
71:     req.set_include_timestamp(true);
72: 
73:     opcua::ReadResponse resp = client.read(req);
74:     if (resp.status().code() == 0) {  // UA_STATUSCODE_GOOD
75:         std::cout << "Value via proto: " << resp.value().int32_val()
76:                   << ", server_ts: " << resp.server_timestamp() << "\n";
77:     }
78: }
79: 
80: // ============================================================================
81: // 场景 3: 读取数组值
82: // ============================================================================
83: void scenario_read_array_types() {
84:     std::cout << "\n=== 场景 3: 读取数组值 ===\n";
85:     OPCUAClient client(PLC_ENDPOINT);
86:     if (!client.connect()) return;
87: 
88:     auto i32_arr_node = OPCUAClient::numericNodeId(0, 2001);
89:     auto f32_arr_node = OPCUAClient::numericNodeId(0, 2002);
90:     auto f64_arr_node = OPCUAClient::numericNodeId(0, 2003);
91: 
92:     // 读取 int32 数组
93:     std::vector<int32_t> i32_vals = client.readInt32Array(i32_arr_node);
94:     std::cout << "Int32 Array (" << i32_vals.size() << " elements):";
95:     for (auto v : i32_vals) std::cout << " " << v;
96:     std::cout << "\n";
97: 
98:     // 读取 float 数组
99:     std::vector<float> f32_vals = client.readFloatArray(f32_arr_node);
100:     std::cout << "Float Array (" << f32_vals.size() << " elements):";
101:     for (auto v : f32_vals) std::cout << " " << v;
102:     std::cout << "\n";
103: 
104:     // 读取 double 数组
105:     std::vector<double> f64_vals = client.readDoubleArray(f64_arr_node);
106:     std::cout << "Double Array (" << f64_vals.size() << " elements):";
107:     for (auto v : f64_vals) std::cout << " " << v;
108:     std::cout << "\n";
109: }
110: 
111: // ============================================================================
112: // 场景 4: 读取非 Value 属性 (如 DisplayName, Description)
113: // ============================================================================
114: void scenario_read_attributes() {
115:     std::cout << "\n=== 场景 4: 读取节点属性 ===\n";
116:     OPCUAClient client(PLC_ENDPOINT);
117:     if (!client.connect()) return;
118: 
119:     auto node = OPCUAClient::numericNodeId(0, 1001);
120: 
121:     // 读取 DisplayName (attribute_id = 14)
122:     auto dn_resp = client.readAttribute(node, 14);
123:     std::cout << "DisplayName: " << dn_resp.value().string_val() << "\n";
124: 
125:     // 读取 Description (attribute_id = 15)
126:     auto desc_resp = client.readAttribute(node, 15);
127:     std::cout << "Description: " << desc_resp.value().string_val() << "\n";
128: 
129:     // 读取 NodeClass (attribute_id = 2)
130:     auto nc_resp = client.readAttribute(node, 2);
131:     std::cout << "NodeClass: " << nc_resp.value().int32_val() << "\n";
132: 
133:     // 读取 BrowseName (attribute_id = 5)
134:     auto bn_resp = client.readAttribute(node, 5);
135:     std::cout << "BrowseName: " << bn_resp.value().string_val() << "\n";
136: 
137:     // 读取 WriteMask (attribute_id = 21)
138:     auto wm_resp = client.readAttribute(node, 21);
139:     std::cout << "WriteMask: " << wm_resp.value().uint32_val() << "\n";
140: 
141:     // 读取 UserWriteMask (attribute_id = 22)
142:     auto uwm_resp = client.readAttribute(node, 22);
143:     std::cout << "UserWriteMask: " << uwm_resp.value().uint32_val() << "\n";
144: 
145:     // 读取 DataType (attribute_id = 6)
146:     auto dt_resp = client.readAttribute(node, 6);
147:     std::cout << "DataType NodeId: " << dt_resp.value().uint32_val() << "\n";
148: }
149: 
150: // ============================================================================
151: // 场景 5: 写入标量值
152: // ============================================================================
153: void scenario_write_scalar_types() {
154:     std::cout << "\n=== 场景 5: 写入标量值 ===\n";
155:     OPCUAClient client(PLC_ENDPOINT);
156:     if (!client.connect()) return;
157: 
158:     auto bool_node   = OPCUAClient::numericNodeId(0, 1001);
159:     auto int32_node  = OPCUAClient::numericNodeId(0, 1002);
160:     auto float_node  = OPCUAClient::numericNodeId(0, 1005);
161:     auto string_node = OPCUAClient::numericNodeId(0, 1007);
162: 
163:     // 方案 A: 类型安全接口
164:     bool ok1 = client.writeBool(bool_node, true);
165:     bool ok2 = client.writeInt32(int32_node, 42);
166:     bool ok3 = client.writeFloat(float_node, 3.14f);
167:     bool ok4 = client.writeString(string_node, "hello plc");
168: 
169:     std::cout << "Write Bool:   " << (ok1 ? "OK" : "FAIL") << "\n";
170:     std::cout << "Write Int32:  " << (ok2 ? "OK" : "FAIL") << "\n";
171:     std::cout << "Write Float:  " << (ok3 ? "OK" : "FAIL") << "\n";
172:     std::cout << "Write String: " << (ok4 ? "OK" : "FAIL") << "\n";
173: 
174:     // 写入后验证
175:     int32_t verify = client.readInt32(int32_node);
176:     std::cout << "Verify read after write: " << verify << " (expect 42)\n";
177: 
178:     // 方案 B: 通用 protobuf 写入
179:     opcua::WriteRequest wreq;
180:     *wreq.mutable_node_id() = int32_node;
181:     wreq.mutable_value()->set_type(opcua::DataTypeId::INT32);
182:     wreq.mutable_value()->set_int32_val(100);
183: 
184:     opcua::WriteResponse wresp = client.write(wreq);
185:     std::cout << "Write via proto: "
186:               << (wresp.status().code() == 0 ? "OK" : "FAIL") << "\n";
187: }
188: 
189: // ============================================================================
190: // 场景 6: 写入数组值
191: // ============================================================================
192: void scenario_write_array_types() {
193:     std::cout << "\n=== 场景 6: 写入数组值 ===\n";
194:     OPCUAClient client(PLC_ENDPOINT);
195:     if (!client.connect()) return;
196: 
197:     auto i32_arr_node = OPCUAClient::numericNodeId(0, 2001);
198:     auto f32_arr_node = OPCUAClient::numericNodeId(0, 2002);
199: 
200:     // 写入 int32 数组
201:     bool ok = client.writeInt32Array(i32_arr_node, {10, 20, 30, 40, 50});
202:     std::cout << "Write Int32 Array: " << (ok ? "OK" : "FAIL") << "\n";
203: 
204:     // 写入 float 数组
205:     ok = client.writeFloatArray(f32_arr_node, {1.1f, 2.2f, 3.3f});
206:     std::cout << "Write Float Array: " << (ok ? "OK" : "FAIL") << "\n";
207: }
208: 
209: // ============================================================================
210: // 场景 7: 批量读取 (Batch Read)
211: // ============================================================================
212: void scenario_batch_read() {
213:     std::cout << "\n=== 场景 7: 批量读取 ===\n";
214:     OPCUAClient client(PLC_ENDPOINT);
215:     if (!client.connect()) return;
216: 
217:     // 一次请求中读取多个 PLC 变量节点
218:     opcua::BatchReadRequest batch_req;
219: 
220:     auto add_read = [&](uint32_t ns, uint32_t id) {
221:         auto* r = batch_req.add_requests();
222:         r->mutable_node_id()->set_namespace_index(ns);
223:         r->mutable_node_id()->set_numeric_id(id);
224:         r->set_attribute_id(13);  // Value
225:     };
226: 
227:     // 模拟从配置文件/配表来的节点列表
228:     add_read(0, 1001);   // Bool
229:     add_read(0, 1002);   // Int32
230:     add_read(0, 1005);   // Float
231:     add_read(0, 1007);   // String
232:     add_read(0, 2001);   // Int32 Array
233: 
234:     opcua::BatchReadResponse batch_resp = client.batchRead(batch_req);
235:     std::cout << "Batch read " << batch_resp.responses_size() << " nodes:\n";
236:     for (int i = 0; i < batch_resp.responses_size(); ++i) {
237:         const auto& r = batch_resp.responses(i);
238:         std::cout << "  Node[" << i << "] status=" << r.status().code()
239:                   << " type=" << r.value().type() << "\n";
240:     }
241: }
242: 
243: // ============================================================================
244: // 场景 8: 批量写入 (Batch Write)
245: // ============================================================================
246: void scenario_batch_write() {
247:     std::cout << "\n=== 场景 8: 批量写入 ===\n";
248:     OPCUAClient client(PLC_ENDPOINT);
249:     if (!client.connect()) return;
250: 
251:     opcua::BatchWriteRequest batch_wreq;
252: 
253:     // 同时写入多个节点
254:     auto add_write_int = [&](uint32_t ns, uint32_t id, int32_t val) {
255:         auto* w = batch_wreq.add_requests();
256:         w->mutable_node_id()->set_namespace_index(ns);
257:         w->mutable_node_id()->set_numeric_id(id);
258:         w->mutable_value()->set_type(opcua::DataTypeId::INT32);
259:         w->mutable_value()->set_int32_val(val);
260:     };
261: 
262:     add_write_int(0, 1002, 10);
263:     add_write_int(0, 1003, 20);
264:     add_write_int(0, 1004, 30);
265: 
266:     opcua::BatchWriteResponse batch_wresp = client.batchWrite(batch_wreq);
267:     std::cout << "Batch write results:\n";
268:     for (int i = 0; i < batch_wresp.responses_size(); ++i) {
269:         std::cout << "  Write[" << i << "]: "
270:                   << (batch_wresp.responses(i).status().code() == 0 ? "OK" : "FAIL")
271:                   << "\n";
272:     }
273: }
274: 
275: // ============================================================================
276: // 场景 9: 遍历/浏览 PLC 地址空间 (Browse)
277: // ============================================================================
278: void scenario_browse() {
279:     std::cout << "\n=== 场景 9: 浏览 PLC 地址空间 ===\n";
280:     OPCUAClient client(PLC_ENDPOINT);
281:     if (!client.connect()) return;
282: 
283:     // 从 Objects 根节点浏览 (ns=0, id=85)
284:     auto root = OPCUAClient::numericNodeId(0, 85);
285: 
286:     // 9a: 浏览所有子节点
287:     std::cout << "--- 9a: 浏览所有子节点 ---\n";
288:     auto all_children = client.browseChildren(root);
289:     for (const auto& ref : all_children) {
290:         std::cout << "  " << ref.display_name()
291:                   << " (class=" << ref.node_class() << ")\n";
292:     }
293: 
294:     // 9b: 只浏览变量节点 (node_class_mask = 2)
295:     std::cout << "--- 9b: 只浏览变量节点 ---\n";
296:     auto vars = client.browseVariables(root);
297:     for (const auto& ref : vars) {
298:         std::cout << "  Variable: " << ref.display_name() << "\n";
299:     }
300: 
301:     // 9c: 只浏览对象节点 (node_class_mask = 1)
302:     std::cout << "--- 9c: 只浏览对象节点 ---\n";
303:     auto objs = client.browseObjects(root);
304:     for (const auto& ref : objs) {
305:         std::cout << "  Object: " << ref.display_name() << "\n";
306:     }
307: 
308:     // 9d: 递归浏览 (遍历树上所有节点, 限制深度)
309:     std::cout << "--- 9d: 递归浏览 (depth=2) ---\n";
310:     auto recursive = client.browseRecursive(root, 2);
311:     for (const auto& ref : recursive.references()) {
312:         std::cout << "  " << ref.display_name()
313:                   << " (class=" << ref.node_class() << ")\n";
314:     }
315: 
316:     // 9e: 使用 protobuf 请求自定义过滤
317:     std::cout << "--- 9e: 用 proto 自定义 Browse ---\n";
318:     opcua::BrowseRequest breq;
319:     *breq.mutable_node_id() = root;
320:     breq.set_node_class_mask(2);  // 只找 Variable
321:     breq.set_max_references(20);
322: 
323:     auto bresp = client.browse(breq);
324:     for (const auto& ref : bresp.references()) {
325:         std::cout << "  ProtoBrowse: " << ref.display_name() << "\n";
326:     }
327: }
328: 
329: // ============================================================================
330: // 场景 10: 订阅数据变化 (Subscribe/MonitoredItem)
331: // ============================================================================
332: void scenario_subscription() {
333:     std::cout << "\n=== 场景 10: 订阅 PLC 数据变化 ===\n";
334:     OPCUAClient client(PLC_ENDPOINT);
335:     if (!client.connect()) return;
336: 
337:     auto temp_node = OPCUAClient::numericNodeId(0, 3001);  // 温度变量
338: 
339:     // 订阅温度变量, 100ms 采样间隔
340:     opcua::SubscribeRequest sub_req;
341:     *sub_req.mutable_node_id() = temp_node;
342:     sub_req.set_sampling_interval_ms(100.0);
343:     sub_req.set_queue_size(10);
344:     sub_req.set_discard_oldest(true);
345: 
346:     auto sub_resp = client.subscribe(sub_req,
347:         [](const opcua::DataChangeNotification& notif) {
348:             std::cout << "[Sub] mon_id=" << notif.monitored_item_id()
349:                       << " value=";
350:             if (notif.value().type() == opcua::DataTypeId::FLOAT)
351:                 std::cout << notif.value().float_val();
352:             else if (notif.value().type() == opcua::DataTypeId::DOUBLE)
353:                 std::cout << notif.value().double_val();
354:             else if (notif.value().type() == opcua::DataTypeId::INT32)
355:                 std::cout << notif.value().int32_val();
356:             std::cout << " ts=" << notif.source_timestamp() << "\n";
357:         });
358: 
359:     if (sub_resp.status().code() == 0) {
360:         std::cout << "Subscribed: sub_id=" << sub_resp.subscription_id()
361:                   << " mon_id=" << sub_resp.monitored_item_id() << "\n";
362:     }
363: 
364:     // 保持运行 10s 收集数据变化
365:     std::this_thread::sleep_for(std::chrono::seconds(10));
366: 
367:     // 取消订阅
368:     opcua::UnsubscribeRequest unsub_req;
369:     unsub_req.set_subscription_id(sub_resp.subscription_id());
370:     unsub_req.set_monitored_item_id(sub_resp.monitored_item_id());
371:     client.unsubscribe(unsub_req);
372:     std::cout << "Unsubscribed\n";
373: }
374: 
375: // ============================================================================
376: // 场景 11: 调用 PLC 方法 (Method Call)
377: // ============================================================================
378: void scenario_method_call() {
379:     std::cout << "\n=== 场景 11: 调用 PLC 方法 ===\n";
380:     OPCUAClient client(PLC_ENDPOINT);
381:     if (!client.connect()) return;
382: 
383:     // 调用 PLC 上的方法: Objects.MyPLC.ResetCounter
384:     auto obj_node  = OPCUAClient::numericNodeId(0, 5001);  // 对象节点
385:     auto mtd_node  = OPCUAClient::numericNodeId(0, 5002);  // 方法节点
386: 
387:     // 无参调用
388:     opcua::CallMethodRequest call_req;
389:     *call_req.mutable_object_id() = obj_node;
390:     *call_req.mutable_method_id() = mtd_node;
391: 
392:     auto call_resp = client.callMethod(call_req);
393:     std::cout << "Method call (no args): "
394:               << (call_resp.status().code() == 0 ? "OK" : "FAIL") << "\n";
395: 
396:     // 带参调用: SetSpeed(speed)
397:     auto speed_method = OPCUAClient::numericNodeId(0, 5003);
398:     opcua::CallMethodRequest call_req2;
399:     *call_req2.mutable_object_id() = obj_node;
400:     *call_req2.mutable_method_id() = speed_method;
401: 
402:     auto* arg = call_req2.add_input_args();
403:     arg->set_type(opcua::DataTypeId::FLOAT);
404:     arg->set_float_val(1500.5f);
405: 
406:     auto call_resp2 = client.callMethod(call_req2);
407:     if (call_resp2.status().code() == 0 && call_resp2.output_args_size() > 0) {
408:         // 假设返回 bool 表示成功
409:         bool success = call_resp2.output_args(0).bool_val();
410:         std::cout << "SetSpeed(1500.5) returned: " << success << "\n";
411:     }
412: }
413: 
414: // ============================================================================
415: // 场景 12: 读取历史数据 (Historical Access)
416: // ============================================================================
417: void scenario_history_read() {
418:     std::cout << "\n=== 场景 12: 读取历史数据 ===\n";
419:     OPCUAClient client(PLC_ENDPOINT);
420:     if (!client.connect()) return;
421: 
422:     auto hist_node = OPCUAClient::numericNodeId(0, 4001);
423: 
424:     // 读取过去 1 小时的历史数据
425:     uint64_t now = UA_DateTime_now();
426:     uint64_t one_hour_ago = now - 3600 * 1000 * 10000;  // UA_DateTime 单位 100ns
427: 
428:     opcua::HistoryReadRequest hist_req;
429:     *hist_req.mutable_node_id() = hist_node;
430:     hist_req.set_start_time(one_hour_ago);
431:     hist_req.set_end_time(now);
432:     hist_req.set_max_values(100);
433: 
434:     auto hist_resp = client.historyReadRaw(hist_req);
435:     if (hist_resp.status().code() == 0) {
436:         std::cout << "History data points: " << hist_resp.values_size() << "\n";
437:         for (int i = 0; i < hist_resp.values_size(); ++i) {
438:             std::cout << "  [" << i << "] val=";
439:             if (hist_resp.values(i).type() == opcua::DataTypeId::FLOAT)
440:                 std::cout << hist_resp.values(i).float_val();
441:             else if (hist_resp.values(i).type() == opcua::DataTypeId::DOUBLE)
442:                 std::cout << hist_resp.values(i).double_val();
443:             std::cout << " ts=" << hist_resp.timestamps(i) << "\n";
444:         }
445:     }
446: }
447: 
448: // ============================================================================
449: // 场景 13: 通过字符串 NodeId 访问 PLC 变量 (西门子 S7-1500 等)
450: // ============================================================================
451: void scenario_string_nodeid() {
452:     std::cout << "\n=== 场景 13: 字符串 NodeId 访问 ===\n";
453:     OPCUAClient client(PLC_ENDPOINT);
454:     if (!client.connect()) return;
455: 
456:     // 有些 PLC (如西门子) 使用字符串 NodeId:
457:     //   ns=3;s="::Program:MainProgram.Temperature"
458:     //   ns=3;s="::Program:MainProgram.Pressure"
459:     auto temp_node = OPCUAClient::stringNodeId(3, "::Program:MainProgram.Temperature");
460:     auto press_node = OPCUAClient::stringNodeId(3, "::Program:MainProgram.Pressure");
461:     auto speed_node = OPCUAClient::stringNodeId(3, "::Program:MainProgram.MotorSpeed");
462: 
463:     float temp  = client.readFloat(temp_node);
464:     float press = client.readFloat(press_node);
465:     float speed = client.readFloat(speed_node);
466: 
467:     std::cout << "Temperature: " << temp << " C\n";
468:     std::cout << "Pressure: " << press << " bar\n";
469:     std::cout << "MotorSpeed: " << speed << " rpm\n";
470: 
471:     // 写入
472:     client.writeFloat(speed_node, 1200.0f);
473:     std::cout << "MotorSpeed set to 1200\n";
474: }
475: 
476: // ============================================================================
477: // 场景 14: 写入不同属性 (如 EngineeringUnits 等)
478: // ============================================================================
479: void scenario_write_attribute() {
480:     std::cout << "\n=== 场景 14: 写入属性 ===\n";
481:     OPCUAClient client(PLC_ENDPOINT);
482:     if (!client.connect()) return;
483: 
484:     auto node = OPCUAClient::numericNodeId(0, 1002);
485: 
486:     // 写 Description (attribute_id = 15)
487:     opcua::WriteRequest wreq;
488:     *wreq.mutable_node_id() = node;
489:     wreq.set_attribute_id(15);   // UA_ATTRIBUTEID_DESCRIPTION
490:     wreq.mutable_value()->set_type(opcua::DataTypeId::STRING);
491:     wreq.mutable_value()->set_string_val("Engine RPM value");
492: 
493:     auto wresp = client.write(wreq);
494:     std::cout << "Write Description: "
495:               << (wresp.status().code() == 0 ? "OK" : "FAIL") << "\n";
496: }
497: 
498: // ============================================================================
499: // 场景 15: 读写大端/位域 (通过 protobuf bytes 传递原始数据)
500: // ============================================================================
501: void scenario_raw_bytes() {
502:     std::cout << "\n=== 场景 15: 原始字节读写 ===\n";
503:     OPCUAClient client(PLC_ENDPOINT);
504:     if (!client.connect()) return;
505: 
506:     auto node = OPCUAClient::numericNodeId(0, 6001);
507: 
508:     // 读取 4 字节原始数据
509:     auto resp = client.readValue(node);
510:     if (resp.value().type() == opcua::DataTypeId::BYTESTRING) {
511:         std::string raw = resp.value().bytes_val();
512:         std::cout << "Raw bytes (" << raw.size() << "): ";
513:         for (unsigned char c : raw)
514:             std::cout << std::hex << (int)c << " ";
515:         std::cout << std::dec << "\n";
516: 
517:         // 解析位域 (假设 byte[0] 的低 4 位是状态标志)
518:         uint8_t status_flags = static_cast<uint8_t>(raw[0]) & 0x0F;
519:         std::cout << "Status flags (lower nibble): " << (int)status_flags << "\n";
520:     }
521: 
522:     // 写入 4 字节原始数据
523:     opcua::WriteRequest wreq;
524:     *wreq.mutable_node_id() = node;
525:     wreq.mutable_value()->set_type(opcua::DataTypeId::BYTESTRING);
526:     wreq.mutable_value()->set_bytes_val(std::string({0x01, 0x02, 0x03, 0x04}));
527:     client.write(wreq);
528: }
529: 
530: // ============================================================================
531: // 场景 16: 读写 LocalizedText / QualifiedName 类型
532: // ============================================================================
533: void scenario_structured_types() {
534:     std::cout << "\n=== 场景 16: 结构化类型 ===\n";
535:     OPCUAClient client(PLC_ENDPOINT);
536:     if (!client.connect()) return;
537: 
538:     auto node = OPCUAClient::numericNodeId(0, 7001);
539: 
540:     // 通用 Read/Write 请求通过 protobuf VariantValue 传递
541:     opcua::ReadRequest req;
542:     *req.mutable_node_id() = node;
543:     auto resp = client.read(req);
544: 
545:     if (resp.status().code() == 0) {
546:         std::cout << "Read type=" << resp.value().type()
547:                   << " string_val=" << resp.value().string_val() << "\n";
548:     }
549: 
550:     // QualifiedName 可编码为字符串 "ns:name"
551:     // LocalizedText 可编码为字符串 "locale:text"
552:     opcua::WriteRequest wreq;
553:     *wreq.mutable_node_id() = node;
554:     wreq.mutable_value()->set_type(opcua::DataTypeId::LOCALIZEDTEXT);
555:     wreq.mutable_value()->set_string_val("en_US:Temperature Sensor");
556:     client.write(wreq);
557: }
558: 
559: // ============================================================================
560: // 场景 17: 使用 protobuf OPCUARequest/OPCUAResponse 统一消息封装
561: // ============================================================================
562: void scenario_unified_message() {
563:     std::cout << "\n=== 场景 17: 统一消息封装 ===\n";
564:     OPCUAClient client(PLC_ENDPOINT);
565:     if (!client.connect()) return;
566: 
567:     // 构建统一的请求消息
568:     opcua::OPCUARequest req;
569:     req.set_request_id(1);
570: 
571:     auto* read_req = req.mutable_read();
572:     read_req->mutable_node_id()->set_namespace_index(0);
573:     read_req->mutable_node_id()->set_numeric_id(1002);
574:     read_req->set_attribute_id(13);
575: 
576:     // 序列化为 bytes (用于网络传输 / 消息队列 / 日志存储)
577:     std::string wire_bytes;
578:     req.SerializeToString(&wire_bytes);
579:     std::cout << "Serialized OPCUARequest: " << wire_bytes.size() << " bytes\n";
580: 
581:     // 反序列化并执行
582:     opcua::OPCUARequest parsed_req;
583:     parsed_req.ParseFromString(wire_bytes);
584: 
585:     opcua::OPCUAResponse resp;
586:     resp.set_request_id(parsed_req.request_id());
587: 
588:     switch (parsed_req.request_case()) {
589:     case opcua::OPCUARequest::kRead: {
590:         auto read_resp = client.read(parsed_req.read());
591:         *resp.mutable_read() = read_resp;
592:         break;
593:     }
594:     default:
595:         break;
596:     }
597: 
598:     // 序列化响应
599:     std::string resp_bytes;
600:     resp.SerializeToString(&resp_bytes);
601:     std::cout << "Response serialized: " << resp_bytes.size() << " bytes\n";
602:     if (resp.read().status().code() == 0) {
603:         std::cout << "Read value: " << resp.read().value().int32_val() << "\n";
604:     }
605: }
606: 
607: // ============================================================================
608: // 场景 18: 安全连接 (用户名/密码 + 证书)
609: // ============================================================================
610: void scenario_secure_connection() {
611:     std::cout << "\n=== 场景 18: 安全连接 (示意) ===\n";
612:     OPCUAClient client("opc.tcp://192.168.1.100:4840");
613:     // 注意: 安全连接需要额外配置 UA_ClientConfig 设置证书和密码
614:     // 以下为概念示例, 需要根据 open62541 安全配置 API 实现
615:     /*
616:     auto* config = UA_Client_getConfig(client.getNative());
617:     config->clientDescription.applicationName = UA_LOCALIZEDTEXT("", "MyApp");
618:     // 设置用户名密码
619:     UA_ClientConfig_setAuthenticationUsername(config, "admin", "password");
620:     // 加载证书
621:     UA_ByteString certificate = loadFile("client_cert.der");
622:     UA_ByteString privateKey  = loadFile("client_key.der");
623:     UA_ClientConfig_setSecurityPolicies(config, &certificate, &privateKey,
624:                                          certificate, privateKey);
625:     */
626:     if (client.connect()) {
627:         std::cout << "Secure connection established\n";
628:     }
629: }
630: 
631: // ============================================================================
632: // 场景 19: 从配置文件/配表动态加载节点列表并轮询
633: // ============================================================================
634: void scenario_polling_from_config() {
635:     std::cout << "\n=== 场景 19: 配表驱动轮询 ===\n";
636:     OPCUAClient client(PLC_ENDPOINT);
637:     if (!client.connect()) return;
638: 
639:     // 模拟从 JSON/YAML/CSV 配表加载的变量监控列表
640:     struct PollConfig {
641:         std::string name;
642:         uint32_t ns;
643:         uint32_t id;
644:         opcua::DataTypeId dtype;
645:     };
646: 
647:     std::vector<PollConfig> poll_list = {
648:         {"Temperature",  0, 1005, opcua::DataTypeId::FLOAT},
649:         {"Pressure",     0, 1006, opcua::DataTypeId::FLOAT},
650:         {"MotorSpeed",   0, 1002, opcua::DataTypeId::INT32},
651:         {"IsRunning",    0, 1001, opcua::DataTypeId::BOOL},
652:         {"AlarmCode",    0, 1003, opcua::DataTypeId::INT64},
653:     };
654: 
655:     // 批量读取 (高效)
656:     opcua::BatchReadRequest batch_req;
657:     for (const auto& cfg : poll_list) {
658:         auto* r = batch_req.add_requests();
659:         r->mutable_node_id()->set_namespace_index(cfg.ns);
660:         r->mutable_node_id()->set_numeric_id(cfg.id);
661:     }
662: 
663:     auto batch_resp = client.batchRead(batch_req);
664:     for (int i = 0; i < batch_resp.responses_size(); ++i) {
665:         std::cout << poll_list[i].name << " = ";
666:         switch (poll_list[i].dtype) {
667:         case opcua::DataTypeId::BOOL:
668:             std::cout << batch_resp.responses(i).value().bool_val(); break;
669:         case opcua::DataTypeId::INT32:
670:             std::cout << batch_resp.responses(i).value().int32_val(); break;
671:         case opcua::DataTypeId::INT64:
672:             std::cout << batch_resp.responses(i).value().int64_val(); break;
673:         case opcua::DataTypeId::FLOAT:
674:             std::cout << batch_resp.responses(i).value().float_val(); break;
675:         default:
676:             std::cout << "(type " << batch_resp.responses(i).value().type() << ")";
677:         }
678:         std::cout << "\n";
679:     }
680: }
681: 
682: // ============================================================================
683: // 场景 20: 异常/故障处理模式
684: // ============================================================================
685: void scenario_error_handling() {
686:     std::cout << "\n=== 场景 20: 异常/故障处理 ===\n";
687:     OPCUAClient client(PLC_ENDPOINT);
688: 
689:     // 尝试连接不存在的 PLC
690:     OPCUAClient bad_client("opc.tcp://192.168.1.200:4840");
691:     if (!bad_client.connect(3000)) {
692:         std::cout << "场景 20a: 连接失败已捕获 (预期行为)\n";
693:     }
694: 
695:     if (!client.connect()) return;
696: 
697:     // 读取不存在的节点
698:     auto bad_node = OPCUAClient::numericNodeId(0, 99999);
699:     auto resp = client.readValue(bad_node);
700:     if (resp.status().code() != 0) {
701:         std::cout << "场景 20b: 读不存在的节点, 错误码="
702:                   << resp.status().code() << "\n";
703:     }
704: 
705:     // 写入只读节点 (假设 1001 是只读的)
706:     auto read_node = OPCUAClient::numericNodeId(0, 1001);
707:     bool wrote = client.writeBool(read_node, false);
708:     if (!wrote) {
709:         std::cout << "场景 20c: 写入只读节点失败 (预期行为)\n";
710:     }
711: 
712:     // 类型不匹配读取
713:     auto float_node = OPCUAClient::numericNodeId(0, 1005);
714:     resp = client.readValue(float_node);
715:     if (resp.value().type() == opcua::DataTypeId::FLOAT) {
716:         std::cout << "场景 20d: 读取 float 节点, 类型="
717:                   << resp.value().type() << " 值=" << resp.value().float_val() << "\n";
718:     }
719: 
720:     // 重连机制 (概念)
721:     if (!client.isConnected()) {
722:         std::cout << "场景 20e: 尝试重连...\n";
723:         for (int retry = 0; retry < 3; ++retry) {
724:             if (client.connect()) {
725:                 std::cout << "  第 " << (retry + 1) << " 次重连成功\n";
726:                 break;
727:             }
728:             std::this_thread::sleep_for(std::chrono::seconds(1));
729:         }
730:     }
731: }
732: 
733: // ============================================================================
734: // 场景 21: protobuf 透传 -- 序列化后通过消息队列 / ZeroMQ / gRPC 转发
735: // ============================================================================
736: void scenario_proto_wire_transport() {
737:     std::cout << "\n=== 场景 21: protobuf 序列化转发 ===\n";
738:     OPCUAClient client(PLC_ENDPOINT);
739:     if (!client.connect()) return;
740: 
741:     // 从 PLC 读取数据
742:     opcua::BatchReadRequest batch;
743:     for (uint32_t id = 1001; id <= 1010; ++id) {
744:         auto* r = batch.add_requests();
745:         r->mutable_node_id()->set_namespace_index(0);
746:         r->mutable_node_id()->set_numeric_id(id);
747:     }
748:     auto batch_resp = client.batchRead(batch);
749: 
750:     // 序列化为 protobuf bytes
751:     std::string pb_data;
752:     batch_resp.SerializeToString(&pb_data);
753:     std::cout << "Serialized batch response: " << pb_data.size() << " bytes\n";
754: 
755:     // 模拟发送到消息队列 (Kafka / RabbitMQ / ZeroMQ)
756:     // mq_producer.send("plc_data_topic", pb_data);
757: 
758:     // 对端反序列化
759:     opcua::BatchReadResponse received;
760:     if (received.ParseFromString(pb_data)) {
761:         std::cout << "Deserialized " << received.responses_size() << " values\n";
762:         for (int i = 0; i < received.responses_size(); ++i) {
763:             std::cout << "  val[" << i << "] type="
764:                       << received.responses(i).value().type() << "\n";
765:         }
766:     }
767: 
768:     // gRPC 示例: service PlcGateway { rpc Read(BatchReadRequest) returns (BatchReadResponse); }
769:     // 说明: 可以直接将 protobuf 消息用于 gRPC 服务定义
770: }
771: 
772: // ============================================================================
773: // 场景 22: 读写 UInt64 / 大整数 (如累计流量计读数)
774: // ============================================================================
775: void scenario_large_integer() {
776:     std::cout << "\n=== 场景 22: 大整数读写 ===\n";
777:     OPCUAClient client(PLC_ENDPOINT);
778:     if (!client.connect()) return;
779: 
780:     auto total_flow = OPCUAClient::numericNodeId(0, 8001);
781:     auto serial_num = OPCUAClient::numericNodeId(0, 8002);
782: 
783:     // 读取大整数累计值
784:     uint64_t flow = client.readUInt32(total_flow);  // 若实际是 UInt64, 需要相应修改
785:     std::cout << "Total flow: " << flow << "\n";
786: 
787:     // 使用 protobuf 写 UInt64
788:     opcua::WriteRequest wreq;
789:     *wreq.mutable_node_id() = serial_num;
790:     wreq.mutable_value()->set_type(opcua::DataTypeId::UINT64);
791:     wreq.mutable_value()->set_uint64_val(9876543210UL);
792:     client.write(wreq);
793:     std::cout << "Serial number written\n";
794: 
795:     // 读取 Int64 (负数兼容)
796:     int64_t counter = client.readInt64(OPCUAClient::numericNodeId(0, 8003));
797:     std::cout << "Signed counter: " << counter << "\n";
798: }
799: 
800: // ============================================================================
801: // 场景 23: 通过 QualifiedName 浏览并快速读取 (按名称查找变量)
802: // ============================================================================
803: void scenario_browse_and_read() {
804:     std::cout << "\n=== 场景 23: 浏览 + 读取组合 ===\n";
805:     OPCUAClient client(PLC_ENDPOINT);
806:     if (!client.connect()) return;
807: 
808:     auto root = OPCUAClient::numericNodeId(0, 85);
809: 
810:     // 浏览所有变量, 然后逐一读取值
811:     auto vars = client.browseVariables(root);
812:     for (const auto& v : vars) {
813:         auto val = client.readValue(v.node_id());
814:         if (val.status().code() == 0) {
815:             std::cout << v.display_name() << " = ";
816:             if (val.value().has_int32_val())
817:                 std::cout << val.value().int32_val();
818:             else if (val.value().has_float_val())
819:                 std::cout << val.value().float_val();
820:             else if (val.value().has_bool_val())
821:                 std::cout << val.value().bool_val();
822:             else if (val.value().has_string_val())
823:                 std::cout << val.value().string_val();
824:             std::cout << "\n";
825:         }
826:     }
827: }
828: 
829: // ============================================================================
830: // 场景 24: 读取 DateTime 类型
831: // ============================================================================
832: void scenario_datetime() {
833:     std::cout << "\n=== 场景 24: DateTime 读写 ===\n";
834:     OPCUAClient client(PLC_ENDPOINT);
835:     if (!client.connect()) return;
836: 
837:     auto dt_node = OPCUAClient::numericNodeId(0, 9001);
838: 
839:     auto resp = client.readValue(dt_node);
840:     if (resp.value().type() == opcua::DataTypeId::DATETIME) {
841:         uint64_t dt_raw = resp.value().datetime_val();
842:         // UA_DateTime 以 100ns 为单位, 从 1601-01-01 开始
843:         // 转换为 Unix timestamp (秒)
844:         uint64_t unix_sec = (dt_raw / 10000000) - 11644473600ULL;
845:         std::cout << "DateTime (raw): " << dt_raw
846:                   << " (unix): " << unix_sec << "\n";
847:     }
848: }
849: 
850: // ============================================================================
851: // 场景 25: 带诊断信息的 Read (StatusCode 详细解析)
852: // ============================================================================
853: void scenario_status_code_detail() {
854:     std::cout << "\n=== 场景 25: 状态码解析 ===\n";
855:     OPCUAClient client(PLC_ENDPOINT);
856:     if (!client.connect()) return;
857: 
858:     auto node = OPCUAClient::numericNodeId(0, 1002);
859:     auto resp = client.readValue(node);
860: 
861:     uint32_t code = resp.status().code();
862:     if (code == 0) {
863:         std::cout << "Good: " << resp.value().int32_val() << "\n";
864:     } else {
865:         // 常见错误码解析
866:         switch (code) {
867:         case 0x80000000:
868:             std::cout << "Bad Unexpected Error\n"; break;
869:         case 0x803C0000:
870:             std::cout << "Bad NodeId Unknown\n"; break;
871:         case 0x80410000:
872:             std::cout << "Bad AttributeId Invalid\n"; break;
873:         case 0x804B0000:
874:             std::cout << "Bad User Access Denied\n"; break;
875:         case 0x805A0000:
876:             std::cout << "Bad Not Readable\n"; break;
877:         case 0x805B0000:
878:             std::cout << "Bad Not Writable\n"; break;
879:         case 0x80730000:
880:             std::cout << "Bad Type Mismatch\n"; break;
881:         case 0x807A0000:
882:             std::cout << "Bad Timeout\n"; break;
883:         default:
884:             std::cout << "Error code: 0x" << std::hex << code << std::dec << "\n";
885:         }
886:     }
887: }
888: 
889: // ============================================================================
890: // main: 运行所有场景
891: // ============================================================================
892: int main() {
893:     scenario_connect_disconnect();
894:     scenario_read_scalar_types();
895:     scenario_read_array_types();
896:     scenario_read_attributes();
897:     scenario_write_scalar_types();
898:     scenario_write_array_types();
899:     scenario_batch_read();
900:     scenario_batch_write();
901:     scenario_browse();
902:     // scenario_subscription();     // 需等待, 默认注释
903:     scenario_method_call();
904:     scenario_history_read();
905:     scenario_string_nodeid();
906:     scenario_write_attribute();
907:     scenario_raw_bytes();
908:     scenario_structured_types();
909:     scenario_unified_message();
910:     scenario_secure_connection();
911:     scenario_polling_from_config();
912:     scenario_error_handling();
913:     scenario_proto_wire_transport();
914:     scenario_large_integer();
915:     scenario_browse_and_read();
916:     scenario_datetime();
917:     scenario_status_code_detail();
918: 
919:     std::cout << "\n所有场景演示完毕.\n";
920:     return 0;
921: }

(End of file - total 921 lines)
</content>
```

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 69.8s)

_Thinking:_

The user is asking about OPC UA data types and their applications, with example programs. I can see this project already has a comprehensive demo with 25 scenarios covering various OPC UA data types. Let me write an explanation of OPC UA data types along with example programs.

Let me provide a thorough explanation and a standalone example program since the `ex/` directory is empty. The project already has a comprehensive `scenarios.cpp` file, so I should provide something complementary and educational.

Let me write an example program in the `ex/` directory that focuses specifically on OPC UA data type demonstration.

这是一个专注于 OPC UA 数据类型应用的示例程序，展示各类型在 protobuf 中的定义、与 open62541 之间的编解码，以及实际工业控制中的应用场景。

**Tool: write**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/ex/data_type_demo.cpp",
  "content": "// ============================================================================\n// OPC UA 数据类型应用详解 + 示例程序\n// ============================================================================\n// OPC UA 类型系统分为三层:\n//   1. Built-in 类型 (25 种基础类型)\n//   2. 预定义结构化类型 (如 LocalizedText, QualifiedName, NodeId)\n//   3. 应用自定义类型 (通过 DataTypeDefinition 定义)\n//\n// 本例展示如何在实际工业控制场景中使用这些类型,\n// 通过 protobuf 封装实现与 open62541 之间的双向转换。\n// ============================================================================\n\n#include \"opcua_client.h\"\n#include <open62541/types.h>\n#include <open62541/types_generated.h>\n#include <iostream>\n#include <iomanip>\n#include <cstring>\n#include <cassert>\n\n// ============================================================================\n// 第一部分: Built-in 类型概览\n//\n// 下表列了 OPC UA 规范定义的 25 种内置数据类型:\n//\n//   ID | 类型           | C/C++ 映射       | 说明\n//  ─────────────────────────────────────────────────────────\n//   1  | Boolean        | bool             | 逻辑值\n//   2  | SByte          | int8_t           | 有符号 8-bit\n//   3  | Byte           | uint8_t          | 无符号 8-bit\n//   4  | Int16          | int16_t          | 有符号 16-bit\n//   5  | UInt16         | uint16_t         | 无符号 16-bit\n//   6  | Int32          | int32_t          | 有符号 32-bit\n//   7  | UInt32         | uint32_t         | 无符号 32-bit\n//   8  | Int64          | int64_t          | 有符号 64-bit\n//   9  | UInt64         | uint64_t         | 无符号 64-bit\n//   10 | Float          | float            | 单精度浮点 (IEEE 754)\n//   11 | Double         | double           | 双精度浮点 (IEEE 754)\n//   12 | String         | UA_String        | UTF-8 字符串\n//   13 | DateTime       | UA_DateTime      | 100ns 间隔, 始于 1601-01-01\n//   14 | Guid           | UA_Guid          | 16 字节 UUID\n//   15 | ByteString     | UA_ByteString    | 原始字节序列\n//   16 | XmlElement     | UA_XmlElement    | XML 片段\n//   17 | NodeId         | UA_NodeId        | 节点标识符\n//   18 | ExpandedNodeId | UA_ExpandedNodeId| 带 namespace URI 的 NodeId\n//   19 | StatusCode     | UA_StatusCode    | 状态码\n//   20 | QualifiedName  | UA_QualifiedName | 带 namespace 索引的字符串\n//   21 | LocalizedText  | UA_LocalizedText | 带 locale 的语言字符串\n//   22 | ExtensionObject| UA_ExtensionObject| 扩展对象 (任意类型包装)\n//   23 | DataValue      | UA_DataValue     | 带品质/时间戳的值包装\n//   24 | Variant        | UA_Variant       | 动态类型容器 (支持任意类型+数组)\n//   25 | DiagnosticInfo | UA_DiagnosticInfo | 诊断信息\n//\n// 这些类型是构建地址空间节点属性的基础。\n// ============================================================================\n\nstatic const std::string PLC_ENDPOINT = \"opc.tcp://192.168.1.100:4840\";\n\n// ============================================================================\n// 1. 标量类型 (Scalar) 读写示例\n// ============================================================================\nvoid demo_builtin_scalar_types() {\n    std::cout << \"\\n========== Built-in 标量类型 ==========\\n\";\n\n    // 每个节点在 OPC UA 服务器中有一个 DataType 属性,\n    // 客户端在读写时必须使用匹配的数据类型。\n    //\n    // NodeId 的构建方式:\n    //   - 数字型:  ns=0;i=1001   (namespace=0, numeric_id=1001)\n    //   - 字符串型: ns=3;s=\"MyVar\" (namespace=3, string_id=\"MyVar\")\n    //\n    // 应用场景:\n    //   PLC 变量通常映射为以下类型:\n\n    auto bool_node    = OPCUAClient::numericNodeId(2, 100);  // 设备开关状态\n    auto int32_node   = OPCUAClient::numericNodeId(2, 101);  // 产品计数\n    auto uint32_node  = OPCUAClient::numericNodeId(2, 102);  // 累计运行时间\n    auto float_node   = OPCUAClient::numericNodeId(2, 103);  // 温度测量值\n    auto double_node  = OPCUAClient::numericNodeId(2, 104);  // 高精度压力\n    auto int64_node   = OPCUAClient::numericNodeId(2, 105);  // 总产量累计\n    auto uint64_node  = OPCUAClient::numericNodeId(2, 106);  // 传感器序列号\n    auto int16_node   = OPCUAClient::numericNodeId(2, 107);  // 通道选择器\n    auto string_node  = OPCUAClient::numericNodeId(2, 108);  // 批次号/产品码\n\n    // 读取示例 (实际运行时需要连接 PLC)\n    // OPCUAClient client(PLC_ENDPOINT);\n    // if (!client.connect()) return;\n    //\n    // bool   sw   = client.readBool(bool_node);\n    // int32_t cnt = client.readInt32(int32_node);\n    // uint32_t rt = client.readUInt32(uint32_node);\n    // float  temp = client.readFloat(float_node);\n    // double pres = client.readDouble(double_node);\n    // int64_t total   = client.readInt64(int64_node);\n    // uint64_t serial = client.readUInt64(uint64_node);  // 注意: readUInt64 需在客户端实现\n    // std::string batch = client.readString(string_node);\n\n    std::cout << \"标量类型: Bool, Int8/16/32/64, UInt8/16/32/64, Float, Double, String\\n\";\n    std::cout << \"          每个类型有对应的 protobuf field 用于序列化\\n\";\n\n    // 写入示例\n    // client.writeBool(bool_node, true);        // 启动设备\n    // client.writeInt32(int32_node, 1024);       // 设置计数\n    // client.writeFloat(float_node, 25.5f);      // 设置温度\n    // client.writeString(string_node, \"BATCH001\"); // 写入批次号\n}\n\n// ============================================================================\n// 2. 数组类型 (Array) 读写示例\n// ============================================================================\nvoid demo_array_types() {\n    std::cout << \"\\n========== 数组类型 ==========\\n\";\n    // 应用场景:\n    //   - 波形数据: 连续采样的 ADC 值 (Int32[] / Float[])\n    //   - 坐标点集: 机器人路径点 (Double[])\n    //   - 配方参数: 批量工艺参数 (Float[])\n    //   - 状态字:   PLC 状态字数组 (Bool[] / UInt16[])\n    //\n    // OPC UA 使用同一个 Variant 表示标量和数组,\n    // 通过 arrayLength 字段区分 (0=标量, >0=数组)。\n\n    auto wave_node    = OPCUAClient::numericNodeId(2, 200);  // 波形采样点 Float[]\n    auto coords_node  = OPCUAClient::numericNodeId(2, 201);  // 坐标点 Double[]\n    auto flags_node   = OPCUAClient::numericNodeId(2, 202);  // 状态标志 Bool[]\n    auto labels_node  = OPCUAClient::numericNodeId(2, 203);  // 数组标签 String[]\n\n    std::cout << \"数组类型: 通过 repeated field 在 protobuf 中传输\\n\";\n    std::cout << \"          Float[] → float_array, Double[] → double_array,\\n\";\n    std::cout << \"          Int32[] → int32_array,  Bool[]  → bool_array\\n\";\n\n    // 读取示例:\n    // auto wave = client.readFloatArray(wave_node);\n    // 写入示例:\n    // client.writeFloatArray(wave_node, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f});\n\n    // 在 protobuf 内部的区分:\n    //   opcua::DataTypeId::FLOAT       = 9   (标量 Float)\n    //   opcua::DataTypeId::ARRAY_FLOAT = 18  (数组 Float)\n    //   variantToProto() 根据 UA_Variant.arrayLength 自动区分\n}\n\n// ============================================================================\n// 3. DateTime 类型 — 时间戳读写\n// ============================================================================\nvoid demo_datetime_type() {\n    std::cout << \"\\n========== DateTime ==========\\n\";\n    // OPC UA 的 DateTime:\n    //   - 单位: 100 纳秒间隔 (100ns = 1e-7 s)\n    //   - 纪元: 1601-01-01 00:00:00 UTC (Windows FILETIME)\n    //   - 转换为 Unix timestamp: unix_sec = (ua_dt / 10_000_000) - 11644473600\n    //   - 范围: 可表示约 30,000 年\n    //\n    // 应用场景:\n    //   - 事件时间戳 (报警触发时间)\n    //   - 历史数据查询的时间范围\n    //   - 批次记录时间\n\n    UA_DateTime now = UA_DateTime_now();\n    UA_DateTimeStruct dts = UA_DateTime_toStruct(now);\n\n    std::cout << \"当前 UA_DateTime: \" << now << \"\\n\";\n    std::cout << \"分解时间: \" << dts.year << \"-\" << (int)dts.month << \"-\" << dts.day\n              << \" \" << dts.hour << \":\" << dts.min << \":\" << dts.sec << \"\\n\";\n\n    // protobuf 中使用 uint64 传输\n    uint64_t proto_dt = static_cast<uint64_t>(now);\n    // Unix 转换:\n    uint64_t unix_sec = (proto_dt / 10000000) - 11644473600ULL;\n    std::cout << \"Unix timestamp: \" << unix_sec << \"\\n\";\n\n    std::cout << \"protobuf field: datetime_val (uint64)\\n\";\n\n    // 读取示例:\n    // auto resp = client.readValue(dt_node);\n    // uint64_t dt = resp.value().datetime_val();\n}\n\n// ============================================================================\n// 4. LocalizedText — 多语言描述\n// ============================================================================\nvoid demo_localized_text() {\n    std::cout << \"\\n========== LocalizedText ==========\\n\";\n    // 结构体定义:\n    //   struct UA_LocalizedText { UA_String locale; UA_String text; };\n    //\n    // locale = \"en_US\", \"zh_CN\", \"de_DE\" 等\n    //\n    // 应用场景:\n    //   - 设备 DisplayName: {\"zh_CN\",\"温度传感器\"}, {\"en_US\",\"Temperature Sensor\"}\n    //   - 报警描述: {\"en_US\",\"Overheat warning\"}, {\"zh_CN\",\"过热警告\"}\n    //   - 菜单标签: 多语言 HMI 界面\n\n    UA_LocalizedText lt;\n    UA_LocalizedText_init(&lt);\n    lt.locale = UA_STRING_STATIC(\"zh_CN\");\n    lt.text   = UA_STRING_STATIC(\"电机转速\");\n\n    std::cout << \"LocalizedText: locale=\\\"\" << lt.locale.data\n              << \"\\\" text=\\\"\" << lt.text.data << \"\\\"\\n\";\n\n    UA_LocalizedText_clear(&lt);\n\n    // protobuf 编码策略: 使用字符串 \"locale:text\"\n    // 例如: \"zh_CN:电机转速\", \"en_US:Motor Speed\"\n    // VariantValue.string_val() 承载\n    std::cout << \"protobuf: LOCALIZEDTEXT=21, string_val=\\\"locale:text\\\"\\n\";\n}\n\n// ============================================================================\n// 5. QualifiedName — 带命名空间的限定名\n// ============================================================================\nvoid demo_qualified_name() {\n    std::cout << \"\\n========== QualifiedName ==========\\n\";\n    // 结构体定义:\n    //   struct UA_QualifiedName { uint16_t namespaceIndex; UA_String name; };\n    //\n    // 类似 XML 的 namespace:name 概念\n    //\n    // 应用场景:\n    //   - BrowseName 标识符: {ns=2,name=\"Temperature\"}\n    //   - 方法输入参数名称\n    //   - DataType 定义中的字段名\n\n    UA_QualifiedName qn;\n    UA_QualifiedName_init(&qn);\n    qn.namespaceIndex = 2;\n    qn.name = UA_STRING_STATIC(\"MotorCurrent\");\n\n    std::cout << \"QualifiedName: ns=\" << qn.namespaceIndex\n              << \" name=\\\"\" << qn.name.data << \"\\\"\\n\";\n\n    UA_QualifiedName_clear(&qn);\n\n    // protobuf: 专有 message QualifiedName { uint32 namespace_index; string name; }\n    // 也可编码为字符串 \"ns:name\", 例如 \"2:MotorCurrent\"\n    std::cout << \"protobuf: QUALIFIEDNAME=22, 专用 message 或 string \\\"ns:name\\\"\\n\";\n}\n\n// ============================================================================\n// 6. NodeId — 节点标识符的四种形式\n// ============================================================================\nvoid demo_node_id_types() {\n    std::cout << \"\\n========== NodeId 四种形式 ==========\\n\";\n    // NodeId 是 OPC UA 中最核心的标识符类型, 有四种变体:\n    //\n    // 1. 数字型 (Numeric):    ns=0;i=85         — 标准类型, 效率最高\n    // 2. 字符串型 (String):   ns=3;s=\"MyVar\"    — PLC 常用, 可读性好\n    // 3. GUID 型 (Guid):      ns=1;g=...        — 全局唯一标识\n    // 4. 不透明型 (Opaque):   ns=1;b=...        — 自定义二进制\n\n    // protobuf 定义:\n    // message NodeId {\n    //     uint32 namespace_index = 1;\n    //     oneof id {\n    //         uint32 numeric_id = 2;    // i=xxxx\n    //         string string_id = 3;     // s=\"xxxx\"\n    //         bytes  guid_id   = 4;     // g=...\n    //         uint64 opaque_id = 5;     // b=...\n    //     }\n    // }\n\n    auto numeric_nid = OPCUAClient::numericNodeId(0, 85);\n    auto string_nid  = OPCUAClient::stringNodeId(3, \"::Program:Main.Temperature\");\n\n    std::cout << \"数字型 NodeId: ns=\" << numeric_nid.namespace_index()\n              << \" id=\" << numeric_nid.numeric_id() << \"\\n\";\n    std::cout << \"字符串型 NodeId: ns=\" << string_nid.namespace_index()\n              << \" id=\\\"\" << string_nid.string_id() << \"\\\"\\n\";\n\n    // 数字型用于: 内置类型、标准对象\n    // 字符串型用于: PLC 符号寻址 (西门子 S7-1500, Beckhoff, Codesys)\n}\n\n// ============================================================================\n// 7. Variant — OPC UA 的动态类型系统\n// ============================================================================\nvoid demo_variant_type() {\n    std::cout << \"\\n========== Variant ==========\\n\";\n    // Variant 是 OPC UA 的\"任意类型\"容器, 可以携带:\n    //   - 任意 Built-in 类型 (scalar)\n    //   - 任意 Built-in 类型的数组\n    //   - ExtensionObject (自定义结构的包装)\n    //\n    // 应用场景:\n    //   - Read/Write 服务返回任意类型的值\n    //   - Subscribe 收到的 DataChange 通知\n    //   - Method 的输入参数和输出参数\n    //\n    // protobuf 中对应的 VariantValue message:\n    //   message VariantValue {\n    //       DataTypeId type = 1;\n    //       oneof value { ... }           // 标量\n    //       repeated ... *_array = ...    // 数组\n    //   }\n    //\n    // 类型映射表:\n    //   OPC UA Type     → DataTypeId         → protobuf field\n    //   ─────────────────────────────────────────────────────\n    //   Boolean         → BOOL              → bool_val\n    //   Int32           → INT32             → int32_val\n    //   UInt32          → UINT32            → uint32_val\n    //   Float           → FLOAT             → float_val\n    //   Double          → DOUBLE            → double_val\n    //   String          → STRING            → string_val\n    //   DateTime        → DATETIME          → datetime_val\n    //   ByteString      → BYTESTRING        → bytes_val\n    //   Int32[]         → ARRAY_INT32       → int32_array[]\n    //   Float[]         → ARRAY_FLOAT       → float_array[]\n\n    std::cout << \"Variant 是 OPC UA 序列化的核心, 本项目的 protoToVariant /\\n\";\n    std::cout << \"variantToProto 负责 open62541 ↔ protobuf 的双向转换\\n\";\n}\n\n// ============================================================================\n// 8. 枚举类型在 protobuf 中的表示\n// ============================================================================\nvoid demo_enum_types() {\n    std::cout << \"\\n========== 枚举类型 ==========\\n\";\n    // OPC UA 规范中有大量枚举类型:\n    //\n    //   NodeClass:      Object=1, Variable=2, Method=4, ObjectType=8, VariableType=16\n    //   AttributeId:    Value=13, DisplayName=14, Description=15, NodeClass=2, ...\n    //   DataTypeId:     上述 DataTypeId 本质上是枚举\n    //\n    // 应用场景:\n    //   - Browse 时按 NodeClass 过滤节点\n    //   - Read/Write 时指定 AttributeId\n    //\n    // protobuf 定义:\n    //   enum DataTypeId {\n    //       BOOL=0, INT8=1, ..., STRUCTURED=23\n    //   }\n    //\n    // 在 proto 中枚举的好处:\n    //   1. 编译期类型检查\n    //   2. 自动序列化/反序列化\n    //   3. 跨语言一致\n\n    std::cout << \"protobuf 枚举示例:\\n\";\n    std::cout << \"  DataTypeId::INT32 = \" << opcua::DataTypeId::INT32 << \"\\n\";\n    std::cout << \"  DataTypeId::FLOAT = \" << opcua::DataTypeId::FLOAT << \"\\n\";\n    std::cout << \"  DataTypeId::STRING = \" << opcua::DataTypeId::STRING << \"\\n\";\n\n    // 实际应用: 按类型分发处理\n    opcua::VariantValue val;\n    val.set_type(opcua::DataTypeId::FLOAT);\n    val.set_float_val(25.5f);\n\n    switch (val.type()) {\n    case opcua::DataTypeId::BOOL:\n        std::cout << \"处理 BOOL: \" << val.bool_val() << \"\\n\"; break;\n    case opcua::DataTypeId::INT32:\n        std::cout << \"处理 INT32: \" << val.int32_val() << \"\\n\"; break;\n    case opcua::DataTypeId::FLOAT:\n        std::cout << \"处理 FLOAT: \" << val.float_val() << \"\\n\"; break;\n    case opcua::DataTypeId::STRING:\n        std::cout << \"处理 STRING: \" << val.string_val() << \"\\n\"; break;\n    default:\n        std::cout << \"未知类型: \" << val.type() << \"\\n\"; break;\n    }\n}\n\n// ============================================================================\n// 9. ExtensionObject — 任意结构化数据的包装\n// ============================================================================\nvoid demo_extension_object() {\n    std::cout << \"\\n========== ExtensionObject ==========\\n\";\n    // ExtensionObject 用于传输自定义结构化数据.\n    // 内部包含:\n    //   - TypeId (NodeId, 指向数据类型定义)\n    //   - 编码方式: Binary / XML / JSON\n    //   - body: 编码后的字节流\n    //\n    // 应用场景:\n    //   - 调用方法时的复杂的输入/输出参数\n    //   - HistoryRead 的过滤器参数 (ReadRawModifiedDetails)\n    //   - 事件通知的字段\n\n    // protobuf 中可通过 BYTESTRING 传输序列化的 ExtensionObject:\n    //   opcua::VariantValue val;\n    //   val.set_type(opcua::DataTypeId::BYTESTRING);\n    //   val.set_bytes_val(binary_data);\n    //\n    // 复杂结构可以定义独立的 protobuf message,\n    // 然后通过 bytes 字段嵌入 VariantValue, 实现嵌套扩展.\n\n    std::cout << \"ExtensionObject 通过 BYTESTRING 或自定义 proto message 传输\\n\";\n}\n\n// ============================================================================\n// 10. StatusCode — 操作结果状态码\n// ============================================================================\nvoid demo_status_code() {\n    std::cout << \"\\n========== StatusCode ==========\\n\";\n    // StatusCode 是 uint32, 高 16 位表示主要错误码, 低 16 位表示子码.\n    //\n    // 关键 StatusCode:\n    //   0x00000000  Good\n    //   0x803C0000  BadNodeIdUnknown\n    //   0x80410000  BadAttributeIdInvalid\n    //   0x804B0000  BadUserAccessDenied\n    //   0x805A0000  BadNotReadable\n    //   0x805B0000  BadNotWritable\n    //   0x80730000  BadTypeMismatch\n    //   0x807A0000  BadTimeout\n    //\n    // 应用场景:\n    //   - 判断 Read/Write/Browse 是否成功\n    //   - Subscribe 的 DataChange 数据品质\n    //   - Method 调用结果\n\n    std::cout << \"StatusCode 在 protobuf 中:\\n\";\n    std::cout << \"  message StatusCode { uint32 code; string symbolic_name; string description; }\\n\";\n\n    // 检查操作结果\n    opcua::StatusCode sc;\n    sc.set_code(0x803C0000);\n    sc.set_symbolic_name(\"BadNodeIdUnknown\");\n\n    if (sc.code() == 0) {\n        std::cout << \"操作成功\\n\";\n    } else {\n        std::cout << \"操作失败: \" << sc.symbolic_name()\n                  << \" (0x\" << std::hex << sc.code() << std::dec << \")\\n\";\n    }\n}\n\n// ============================================================================\n// 11. 数据品质 (DataValue 的 quality/status)\n// ============================================================================\nvoid demo_data_quality() {\n    std::cout << \"\\n========== 数据品质 ==========\\n\";\n    // DataValue 包含:\n    //   - Value:       实际数据 (Variant)\n    //   - Status:      数据品质 (StatusCode)\n    //   - SourceTimestamp: 源时间戳 (数据产生时间)\n    //   - ServerTimestamp: 服务端时间戳 (数据到达服务端时间)\n    //\n    // 品质等级:\n    //   Good (0x00)              — 数据有效\n    //   Uncertain (0x40)        — 数据可用但品质不确定\n    //   Bad (0x80)              — 数据无效\n    //   BadSensorFailure        — 传感器故障\n    //   BadNotConnected         — 通信断开\n    //\n    // 应用场景:\n    //   - 数据采集时先检查品质, 避免使用无效数据\n    //   - SCADA 系统的数据质量告警\n\n    // protobuf 中 ReadResponse 携带时间戳:\n    //   message ReadResponse {\n    //       StatusCode status = 1;\n    //       VariantValue value = 2;\n    //       uint64 server_timestamp = 3;\n    //       uint64 source_timestamp = 4;\n    //   }\n\n    std::cout << \"ReadResponse protobuf 包含 status + value + 双时间戳\\n\";\n    std::cout << \"品质判断: status.code() == 0 → Good\\n\";\n}\n\n// ============================================================================\n// 12. 类型转换: 从 open62541 UA_Variant 到 protobuf VariantValue\n// ============================================================================\nvoid demo_type_conversion() {\n    std::cout << \"\\n========== UA_Variant ↔ protobuf VariantValue ==========\\n\";\n\n    // open62541 → protobuf (variantToProto)\n    UA_Variant ua_var;\n    UA_Variant_init(&ua_var);\n\n    UA_Int32 scalar_val = 42;\n    UA_Variant_setScalarCopy(&ua_var, &scalar_val, &UA_TYPES[UA_TYPES_INT32]);\n    std::cout << \"UA_Variant type: \" << UA_TYPES[UA_TYPES_INT32].typeName\n              << \" isArray: \" << (ua_var.arrayLength > 0) << \"\\n\";\n    UA_Variant_clear(&ua_var);\n\n    // Array case\n    UA_Float arr[4] = {1.1f, 2.2f, 3.3f, 4.4f};\n    UA_Variant_setArrayCopy(&ua_var, arr, 4, &UA_TYPES[UA_TYPES_FLOAT]);\n    std::cout << \"UA_Variant Float[4]: typeName=\"\n              << UA_TYPES[ua_var.type->typeKind].typeName\n              << \" length=\" << ua_var.arrayLength << \"\\n\";\n    UA_Variant_clear(&ua_var);\n\n    // String case\n    UA_String ua_str = UA_STRING_ALLOC(\"Hello OPC UA\");\n    UA_Variant_setScalarCopy(&ua_var, &ua_str, &UA_TYPES[UA_TYPES_STRING]);\n    UA_String_clear(&ua_str);\n    std::cout << \"UA_Variant String: \\\"\"\n              << ((UA_String*)ua_var.data)->data << \"\\\"\\n\";\n    UA_Variant_clear(&ua_var);\n\n    // protobuf VariantValue → open62541 (protoToVariant) 是反向过程\n    std::cout << \"\\n双向转换流程:\\n\";\n    std::cout << \"  1. UA_Variant → opcua::VariantValue:  variantToProto()\\n\";\n    std::cout << \"  2. opcua::VariantValue → UA_Variant:  protoToVariant()\\n\";\n    std::cout << \"转换在 opcua_client.cpp 中完整实现.\\n\";\n}\n\n// ============================================================================\n// 13. 综合场景: 温度监控系统的数据处理\n// ============================================================================\nvoid demo_integrated_scenario() {\n    std::cout << \"\\n========== 综合应用场景: 温度监控系统 ==========\\n\";\n    // 这是一个工业温度监控系统的数据类型应用全景\n\n    // ---- 13a. 数据类型映射 ----\n    std::cout << \"\\n-- 13a. 温度监控系统的数据类型映射 --\\n\";\n    struct TempChannel {\n        uint32_t ns;\n        uint32_t id;\n        opcua::DataTypeId dtype;\n        const char* unit;\n        const char* description;\n    };\n\n    TempChannel channels[] = {\n        {2, 100, opcua::DataTypeId::FLOAT,  \"°C\",  \"当前温度值 (Float)\"},\n        {2, 101, opcua::DataTypeId::FLOAT,  \"°C\",  \"温度设定值 (Float)\"},\n        {2, 102, opcua::DataTypeId::BOOL,   \"\",    \"加热器启停 (Bool)\"},\n        {2, 103, opcua::DataTypeId::FLOAT,  \"°C/s\",\"升温速率 (Float)\"},\n        {2, 104, opcua::DataTypeId::INT32,  \"pcs\", \"超温报警计数 (Int32)\"},\n        {2, 105, opcua::DataTypeId::DOUBLE, \"bar\", \"腔体压力 (Double)\"},\n        {2, 106, opcua::DataTypeId::INT64,  \"ms\",  \"累计运行时长 (Int64)\"},\n        {2, 107, opcua::DataTypeId::STRING, \"\",    \"当前配方名 (String)\"},\n        {2, 200, opcua::DataTypeId::ARRAY_FLOAT, \"°C\", \"温度曲线数组 (Float[])\"},\n        {2, 201, opcua::DataTypeId::DATETIME, \"\",   \"最后校准时间 (DateTime)\"},\n    };\n\n    for (const auto& ch : channels) {\n        std::cout << \"  ns=\" << ch.ns << \";i=\" << ch.id\n                  << \" [\" << ch.dtype << \"] \"\n                  << ch.description;\n        if (ch.unit[0]) std::cout << \" unit=\" << ch.unit;\n        std::cout << \"\\n\";\n    }\n\n    // ---- 13b. 读取并分类处理 ----\n    std::cout << \"\\n-- 13b. 按类型分发处理 --\\n\";\n    auto process_value = [](opcua::DataTypeId type, const opcua::VariantValue& val) {\n        switch (type) {\n        case opcua::DataTypeId::FLOAT:\n            std::cout << \"  Float: \" << val.float_val();\n            if (val.float_val() > 100.0f)\n                std::cout << \" [超温告警!]\";\n            break;\n        case opcua::DataTypeId::BOOL:\n            std::cout << \"  Bool: \" << (val.bool_val() ? \"ON\" : \"OFF\");\n            break;\n        case opcua::DataTypeId::INT32:\n            std::cout << \"  Int32: \" << val.int32_val();\n            break;\n        case opcua::DataTypeId::STRING:\n            std::cout << \"  String: \" << val.string_val();\n            break;\n        case opcua::DataTypeId::ARRAY_FLOAT: {\n            std::cout << \"  Float[\" << val.float_array_size() << \"]:\";\n            for (int i = 0; i < val.float_array_size(); ++i)\n                std::cout << \" \" << val.float_array(i);\n            break;\n        }\n        default:\n            std::cout << \"  type=\" << type;\n        }\n        std::cout << \"\\n\";\n    };\n\n    // 模拟一批读取结果\n    opcua::BatchReadResponse batch_resp;\n    for (int i = 0; i < 5; ++i) {\n        auto* resp = batch_resp.add_responses();\n        resp->mutable_status()->set_code(0);\n        auto* val = resp->mutable_value();\n        switch (i) {\n        case 0: val->set_type(opcua::DataTypeId::FLOAT);  val->set_float_val(85.5f);  break;\n        case 1: val->set_type(opcua::DataTypeId::FLOAT);  val->set_float_val(95.0f);  break;\n        case 2: val->set_type(opcua::DataTypeId::BOOL);   val->set_bool_val(true);    break;\n        case 3: val->set_type(opcua::DataTypeId::INT32);  val->set_int32_val(3);      break;\n        case 4: val->set_type(opcua::DataTypeId::STRING); val->set_string_val(\"Recipe_01\"); break;\n        }\n    }\n\n    for (int i = 0; i < batch_resp.responses_size(); ++i) {\n        const auto& r = batch_resp.responses(i);\n        if (r.status().code() == 0) {\n            std::cout << \"  Ch[\" << i << \"]:\";\n            process_value(r.value().type(), r.value());\n        }\n    }\n\n    // ---- 13c. 序列化用于日志/传输 ----\n    std::cout << \"\\n-- 13c. protobuf 序列化到日志/消息队列 --\\n\";\n    std::string wire;\n    batch_resp.SerializeToString(&wire);\n    std::cout << \"  Batch response serialized: \" << wire.size() << \" bytes\\n\";\n\n    // 反序列化验证\n    opcua::BatchReadResponse parsed;\n    parsed.ParseFromString(wire);\n    std::cout << \"  Deserialized \" << parsed.responses_size() << \" values\\n\";\n}\n\n// ============================================================================\n// 14. 自定义结构化类型的扩展方式\n// ============================================================================\nvoid demo_custom_structured_type() {\n    std::cout << \"\\n========== 自定义结构化类型 ==========\\n\";\n    // OPC UA 允许服务端定义自己的数据类型 (通过 DataType 节点).\n    //\n    // 方式 1: 在 protobuf 中定义专属 message, 使用 BYTESTRING 封装\n    //   message AnalogSignal {\n    //       float  value   = 1;\n    //       float  range_min = 2;\n    //       float  range_max = 3;\n    //       string unit    = 4;\n    //       uint64 timestamp = 5;\n    //   }\n    //\n    //   opcua::VariantValue val;\n    //   val.set_type(opcua::DataTypeId::BYTESTRING);\n    //   AnalogSignal sig; sig.SerializeToString(&tmp);\n    //   val.set_bytes_val(tmp);\n    //\n    // 方式 2: 使用 VariantValue 的结构化字段 (如果扩展)\n    //   可以在 VariantValue 的 oneof 中增加 message 字段\n    //\n    // 方式 3: 使用 ExtensionObject + 自定义编码\n    //   复杂场景, 需要 TypeId 和编码器注册\n\n    std::cout << \"自定义类型扩展方案:\\n\";\n    std::cout << \"  1. BYTESTRING + 嵌套 proto message (轻量, 推荐)\\n\";\n    std::cout << \"  2. VariantValue 增加结构化字段 (修改 proto)\\n\";\n    std::cout << \"  3. ExtensionObject + UA_DataType 注册 (完整 OPC UA 兼容)\\n\";\n}\n\n// ============================================================================\n// 15. 数据处理备注\n// ============================================================================\nvoid demo_type_safety_notes() {\n    std::cout << \"\\n========== 类型安全与最佳实践 ==========\\n\";\n    std::cout << \"1. 读写时必须确保 DataTypeId 与服务器节点类型匹配\\n\";\n    std::cout << \"2. 数组和标量使用同一个 VariantValue, 根据 type 字段区分\\n\";\n    std::cout << \"3. 收到请求先检查 status.code() == 0, 再使用 value\\n\";\n    std::cout << \"4. Proto 序列化用于跨语言/跨进程传输 (C++/Python/Java/C#)\\n\";\n    std::cout << \"5. open62541 内置所有 25 种 Built-in 类型的序列化/反序列化\\n\";\n    std::cout << \"6. Variant 的嵌套使用: Variant 可以包含另一个 Variant (嵌套)\\n\";\n    std::cout << \"7. 类型不匹配时服务器返回 BadTypeMismatch (0x80730000)\\n\";\n}\n\n// ============================================================================\nint main() {\n    std::cout << \"=========================================\\n\";\n    std::cout << \"OPC UA 数据类型应用详解\\n\";\n    std::cout << \"=========================================\\n\";\n\n    std::cout << \"\\nOPC UA 数据类型分层结构:\\n\";\n    std::cout << \"  ┌─────────────────────────┐\\n\";\n    std::cout << \"  │  Application Types      │  ← 自定义结构体\\n\";\n    std::cout << \"  │  (custom structs)       │\\n\";\n    std::cout << \"  ├─────────────────────────┤\\n\";\n    std::cout << \"  │  Predefined Types       │  ← NodeId, QualifiedName, ...\\n\";\n    std::cout << \"  │  (built-in structured)  │\\n\";\n    std::cout << \"  ├─────────────────────────┤\\n\";\n    std::cout << \"  │  Built-in Types (25)    │  ← Boolean, Int32, Float, ...\\n\";\n    std::cout << \"  │  (primitive types)      │\\n\";\n    std::cout << \"  └─────────────────────────┘\\n\";\n\n    demo_builtin_scalar_types();\n    demo_array_types();\n    demo_datetime_type();\n    demo_localized_text();\n    demo_qualified_name();\n    demo_node_id_types();\n    demo_variant_type();\n    demo_enum_types();\n    demo_extension_object();\n    demo_status_code();\n    demo_data_quality();\n    demo_type_conversion();\n    demo_integrated_scenario();\n    demo_custom_structured_type();\n    demo_type_safety_notes();\n\n    std::cout << \"\\n=========================================\\n\";\n    std::cout << \"总结:\\n\";\n    std::cout << \"  OPC UA 数据类型体系由 25 种 Built-in 类型为基础,\\n\";\n    std::cout << \"  通过 Variant 实现动态类型, 通过 ExtensionObject 扩展自定义类型.\\n\";\n    std::cout << \"  Protobuf 封装 (opcua.proto) 将所有类型映射为统一的\\n\";\n    std::cout << \"  VariantValue message, 实现高效的跨语言序列化.\\n\";\n    std::cout << \"=========================================\\n\";\n    return 0;\n}\n"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 4.0s)

_Thinking:_

Let me also add the build configuration for the new file to CMakeLists.txt so the user can compile and run it.

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/CMakeLists.txt"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/CMakeLists.txt</path>
<type>file</type>
<content>
1: cmake_minimum_required(VERSION 3.16)
2: project(opcua-protobuf-demo CXX)
3: 
4: set(CMAKE_CXX_STANDARD 17)
5: set(CMAKE_CXX_STANDARD_REQUIRED ON)
6: 
7: # ---- Dependencies ----
8: find_package(protobuf CONFIG REQUIRED)
9: find_package(open62541 REQUIRED)
10: find_package(Threads REQUIRED)
11: 
12: # ---- Library: OPC UA Client Wrapper ----
13: add_library(opcua_client
14:     opcua_client.cpp
15: )
16: 
17: target_include_directories(opcua_client PUBLIC
18:     ${CMAKE_CURRENT_BINARY_DIR}
19:     ${open62541_INCLUDE_DIRS}
20: )
21: 
22: target_link_libraries(opcua_client PUBLIC
23:     protobuf::libprotobuf
24:     ${open62541_LIBRARIES}
25: )
26: 
27: # ---- Protobuf code generation ----
28: protobuf_generate(TARGET opcua_client LANGUAGE cpp PROTOS opcua.proto)
29: 
30: # ---- Executable: All Usage Scenarios ----
31: add_executable(scenarios scenarios.cpp)
32: target_include_directories(scenarios PRIVATE
33:     ${open62541_INCLUDE_DIRS}
34: )
35: target_link_libraries(scenarios PRIVATE
36:     opcua_client
37:     Threads::Threads
38: )

(End of file - total 38 lines)
</content>
```

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 3.2s)

_Thinking:_

Let me add the data_type_demo executable to the CMakeLists.txt.

**Tool: edit**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/CMakeLists.txt",
  "oldString": "# ---- Executable: All Usage Scenarios ----\nadd_executable(scenarios scenarios.cpp)\ntarget_include_directories(scenarios PRIVATE\n    ${open62541_INCLUDE_DIRS}\n)\ntarget_link_libraries(scenarios PRIVATE\n    opcua_client\n    Threads::Threads\n)",
  "newString": "# ---- Executable: All Usage Scenarios ----\nadd_executable(scenarios scenarios.cpp)\ntarget_include_directories(scenarios PRIVATE\n    ${open62541_INCLUDE_DIRS}\n)\ntarget_link_libraries(scenarios PRIVATE\n    opcua_client\n    Threads::Threads\n)\n\n# ---- Executable: Data Type Demo ----\nadd_executable(data_type_demo ex/data_type_demo.cpp)\ntarget_include_directories(data_type_demo PRIVATE\n    ${open62541_INCLUDE_DIRS}\n)\ntarget_link_libraries(data_type_demo PRIVATE\n    opcua_client\n    Threads::Threads\n)"
}
```

**Output:**
```
Edit applied successfully.
```

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 5.8s)

_Thinking:_

The user asked me to explain OPC UA data types and give example programs. I've written a comprehensive demo program in `ex/data_type_demo.cpp` and added it to the build. Let me summarize what was done.

已在 `ex/data_type_demo.cpp` 创建了完整的 OPC UA 数据类型应用示例，并更新了 `CMakeLists.txt` 以支持编译。

## OPC UA 数据类型体系

OPC UA 类型分三层：

| 层次 | 说明 | 示例 |
|------|------|------|
| **Built-in (25种)** | 基础数据类型 | `Boolean`, `Int32`, `Float`, `Double`, `String`, `DateTime` |
| **预定义结构体** | 规范定义的结构化类型 | `NodeId`, `QualifiedName`, `LocalizedText`, `Variant`, `ExtensionObject` |
| **自定义类型** | 应用层定义 | 通过 `DataType` 节点 + `ExtensionObject` 扩展 |

## 核心设计

- **`Variant`** 是动态类型容器，可承载任意 Built-in 类型的标量或数组
- **`VariantValue`** (protobuf) 将 25 种类型统一映射为 `oneof value` + `repeated array` 字段
- **`DataTypeId`** 枚举区分类型，**`StatusCode`** 表示操作结果品质
- 双向转换函数 `variantToProto()` / `protoToVariant()` 在 `opcua_client.cpp` 中完成 `UA_Variant ↔ protobuf` 互转

## 编译运行

```bash
cd build
cmake ..
make data_type_demo
./data_type_demo
```

示例包含 15 个独立 demo，从标量/数组/DateTime 到类型转换、综合监控场景（温度监控系统的多类型数据分发处理），以及自定义类型的扩展方案。

---

## User

当前添加任务到线程池，然后分别执行，需要提供能够查询修改另一线程的上下文的方法

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 10.1s)

_Thinking:_

The user is asking about adding tasks to a thread pool and executing them, and they want methods to query and modify the context of another thread. This is in the context of the OPC UA project (proto_opcua).

Let me understand the context better. The user has an OPC UA client wrapper. They want to add tasks to a thread pool, execute them separately, and provide methods to query/modify the context of another thread.

This is a common pattern - thread-local context or shared context per thread, where tasks running on threads need to access/modify state of other threads (like OPC UA subscriptions, monitored items, etc.)

Let me ask clarifying questions about what they actually want, or look at how this might integrate with the existing codebase. Let me think about what "context" means here - likely each thread has a context (like a client connection, subscription state), and they want to be able to query or modify another thread's context.

Actually, let me first check if there's more context in the repository - maybe there are other files that hint at a thread pool design.

Let me explore the workspace root to understand the broader project.

Actually, the question seems somewhat generic - they want a thread pool where tasks are added, and they need thread context query/modification methods. This is a fairly common design question. Let me think about what the best approach would be.

Let me look at the overall project structure first to understand conventions.

**Tool: bash**

**Input:**
```json
{
  "command": "git -C /Volumes/ExtDisk/code/cpp/project/opencodes log --oneline -10 2>/dev/null; echo \"---\"; ls /Volumes/ExtDisk/code/cpp/project/opencodes"
}
```

**Output:**
```
659c7a4 上传本地
---
auto
ccache
crash_cases
ddd
ddd_multi_device
proto_opcua
state_machine
state_machine_2
threads

```

**Tool: grep**

**Input:**
```json
{
  "pattern": "thread|pool|context|Thread",
  "path": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua",
  "include": "*.{h,cpp,proto}"
}
```

**Output:**
```
Found 15 matches
/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/parse_project/plc_poller.h:
  Line 8: #include <thread>

  Line 48:     void pollThread();

  Line 56:     std::thread poll_thread_;


/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/parse_project/main.cpp:
  Line 11: #include <thread>

  Line 220: // demo 6: NodeCache - thread-safe cache with change callback

  Line 309:   std::this_thread::sleep_for(std::chrono::milliseconds(300));


/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/parse_project/plc_poller.cpp:
  Line 3: #include <thread>

  Line 42:     poll_thread_ = std::thread(&PlcPoller::pollThread, this);

  Line 47:     if (poll_thread_.joinable()) poll_thread_.join();

  Line 73: void PlcPoller::pollThread() {

  Line 87:             std::this_thread::sleep_for(std::chrono::milliseconds(100));

  Line 89:             std::this_thread::sleep_until(next);


/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/scenarios.cpp:
  Line 11: #include <thread>

  Line 365:     std::this_thread::sleep_for(std::chrono::seconds(10));

  Line 728:             std::this_thread::sleep_for(std::chrono::seconds(1));

```

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 3.3s)

_Thinking:_

Let me look at the broader project structure, especially the "threads" directory, which likely has thread pool examples. This will help me understand the conventions and patterns used.

**Tool: bash**

**Input:**
```json
{
  "command": "ls -R /Volumes/ExtDisk/code/cpp/project/opencodes/threads 2>/dev/null | head -50; echo \"===\"; ls /Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/parse_project"
}
```

**Output:**
```
cancel

/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel:
CMakeLists.txt
build
include
session-ses_030c.md
src

/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel/build:
CMakeCache.txt
CMakeFiles
Makefile
cmake_install.cmake
demo

/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel/build/CMakeFiles:
4.3.0
CMakeConfigureLog.yaml
CMakeDirectoryInformation.cmake
CMakeScratch
InstallScripts.json
Makefile.cmake
Makefile2
TargetDirectories.txt
cmake.check_cache
demo.dir
pkgRedirects
progress.marks

/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel/build/CMakeFiles/4.3.0:
CMakeCXXCompiler.cmake
CMakeDetermineCompilerABI_CXX.bin
CMakeSystem.cmake
CompilerIdCXX

/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel/build/CMakeFiles/4.3.0/CompilerIdCXX:
CMakeCXXCompilerId.cpp
a.out
apple-sdk.cpp
tmp

/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel/build/CMakeFiles/4.3.0/CompilerIdCXX/tmp:

/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel/build/CMakeFiles/CMakeScratch:

/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel/build/CMakeFiles/demo.dir:
DependInfo.cmake
build.make
cmake_clean.cmake
===
CMakeLists.txt
build
main.cpp
node_cache.cpp
node_cache.h
plc_client.cpp
plc_client.h
plc_poller.cpp
plc_poller.h
session-ses_0609.md
struct_parser.cpp
struct_parser.h

```

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 5.4s)

_Thinking:_

Let me look at the threads/cancel project since it seems related to thread cancellation, and the parse_project which has plc_poller with a poll thread. Let me also check the session files for context on what the user is working on.

The user's request is about: "当前添加任务到线程池，然后分别执行，需要提供能够查询修改另一线程的上下文的方法"

Translation: "Currently add tasks to a thread pool, then execute separately, need to provide methods to query and modify another thread's context."

This is asking about a design where:
1. Tasks are added to a thread pool and executed on separate threads
2. Need methods to query and modify the context of another thread

This is a classic concurrency pattern. In the context of this OPC UA project, each thread probably runs its own OPC UA client (or the tasks are PLC polling tasks on different threads). Each thread has a "context" (its own state like connection, subscription, etc.). The user wants cross-thread query/modification of that context.

Let me look at the parse_project files to understand the existing structure, and the cancel example.

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel/src"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel/src</path>
<type>directory</type>
<entries>
main.cpp

(1 entries)
</entries>
```

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/parse_project/plc_poller.h"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/parse_project/plc_poller.h</path>
<type>file</type>
<content>
1: #pragma once
2: 
3: #include "plc_client.h"
4: #include "node_cache.h"
5: #include "struct_parser.h"
6: #include <string>
7: #include <vector>
8: #include <thread>
9: #include <atomic>
10: #include <functional>
11: #include <chrono>
12: #include <mutex>
13: 
14: struct PollItem {
15:     NodeKey key;
16:     std::string name;
17:     std::string description;
18:     bool is_struct{false};
19: };
20: 
21: struct PollGroup {
22:     std::string name;
23:     std::chrono::milliseconds interval{1000};
24:     std::vector<PollItem> items;
25:     bool enabled{true};
26: };
27: 
28: class PlcPoller {
29: public:
30:     PlcPoller(PlcClient& client, NodeCache& cache);
31:     ~PlcPoller();
32: 
33:     void addGroup(PollGroup group);
34:     void removeGroup(const std::string& name);
35:     void setGroupInterval(const std::string& name, std::chrono::milliseconds ms);
36:     void setGroupEnabled(const std::string& name, bool enabled);
37: 
38:     void start();
39:     void stop();
40:     bool running() const;
41:     void pollOnce(const std::string& group_name = "");
42:     void pollAll();
43: 
44:     void setOnPollDone(std::function<void(const std::string& group, size_t count)> cb);
45:     void setOnError(std::function<void(const NodeKey& key, uint32_t status)> cb);
46: 
47: private:
48:     void pollThread();
49:     void pollGroup(PollGroup& grp);
50:     CachedValue readNode(const PollItem& item);
51: 
52:     PlcClient& client_;
53:     NodeCache& cache_;
54:     std::vector<PollGroup> groups_;
55:     std::mutex groups_mtx_;
56:     std::thread poll_thread_;
57:     std::atomic<bool> running_{false};
58:     std::function<void(const std::string&, size_t)> on_poll_done_;
59:     std::function<void(const NodeKey&, uint32_t)> on_error_;
60: };

(End of file - total 60 lines)
</content>
```

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 2.8s)

_Thinking:_

Let me look at the cancel example since it's likely related to the user's current work on thread cancellation and contexts.

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel/src/main.cpp"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel/src/main.cpp</path>
<type>file</type>
<content>
1: #include "stage/pipeline.hpp"
2: 
3: #include <chrono>
4: #include <cstdio>
5: #include <thread>
6: #include <vector>
7: 
8: using namespace std::chrono_literals;
9: 
10: static const char* stage_name(int i) {
11:     static const char* names[] = {"step1", "step2", "step3", "step4", "step5"};
12:     return (i >= 0 && i < 5) ? names[i] : "?";
13: }
14: 
15: static stage::Pipeline::Step make_step(int i, int ms) {
16:     return [i, ms](stage::Pipeline&) {
17:         printf("    [%s] begin (%d ms, runs to completion)\n", stage_name(i), ms);
18:         std::this_thread::sleep_for(std::chrono::milliseconds(ms));
19:         printf("    [%s] done\n", stage_name(i));
20:     };
21: }
22: 
23: int main() {
24:     puts("Scenario 1: stop RPC arrives in the middle of step2.\n"
25:          "            step2 is NOT killed; it finishes, steps 3-5 are skipped,\n"
26:          "            then cleanup runs.");
27:     {
28:         stage::Pipeline pipe;
29:         pipe.set_steps({
30:             make_step(0, 300),
31:             make_step(1, 400),
32:             make_step(2, 500),
33:             make_step(3, 200),
34:             make_step(4, 300),
35:         });
36:         pipe.set_cleanup([](stage::Pipeline&, bool aborted) {
37:             printf("    [cleanup] begin (aborted=%d)\n", (int)aborted);
38:             std::this_thread::sleep_for(150ms);
39:             printf("    [cleanup] done\n");
40:         });
41: 
42:         pipe.start();
43: 
44:         // Simulated "stop" RPC arriving at t=550ms, mid-step2.
45:         std::thread rpc([&pipe] {
46:             std::this_thread::sleep_for(550ms);
47:             printf("\n  >>> stop RPC arrives, stage %s executing\n",
48:                    stage_name(pipe.current_stage()));
49:             auto t0 = std::chrono::steady_clock::now();
50:             pipe.stop();
51:             auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(
52:                 std::chrono::steady_clock::now() - t0);
53:             printf("  >>> stop() returned after %lld ms, pipeline fully stopped\n",
54:                    (long long)dt.count());
55:         });
56: 
57:         pipe.wait();
58:         rpc.join();
59:         pipe.rethrow_if_failed();
60:     }
61: 
62:     puts("\nScenario 2: cooperative stage. The stage may end early, but only\n"
63:          "            when IT chooses to check the flag.");
64:     {
65:         stage::Pipeline pipe;
66:         pipe.set_steps({
67:             [](stage::Pipeline&) {
68:                 printf("    [step1] begin/done\n");
69:                 std::this_thread::sleep_for(100ms);
70:             },
71:             [](stage::Pipeline& p) {
72:                 printf("    [step2] long cooperative stage begin\n");
73:                 for (int i = 0; i < 100; ++i) {
74:                     if (p.stop_requested()) {
75:                         printf("    [step2] exited early (cooperative)\n");
76:                         return;
77:                     }
78:                     std::this_thread::sleep_for(50ms);
79:                 }
80:                 printf("    [step2] completed naturally\n");
81:             },
82:             [](stage::Pipeline&) { printf("    [step3] must NOT run\n"); },
83:         });
84: 
85:         pipe.start();
86:         std::thread rpc([&pipe] {
87:             std::this_thread::sleep_for(180ms);
88:             pipe.stop();
89:         });
90:         pipe.wait();
91:         rpc.join();
92:         pipe.rethrow_if_failed();
93:     }
94: 
95:     puts("\nall scenarios finished");
96:     return 0;
97: }

(End of file - total 97 lines)
</content>
```

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 6.6s)

_Thinking:_

The user's request is about thread pools and cross-thread context access. This seems like a new design task in the context of this OPC UA project. Let me understand the request better.

"当前添加任务到线程池，然后分别执行，需要提供能够查询修改另一线程的上下文的方法"

Translation: "Currently tasks are added to a thread pool and executed separately. Need to provide methods to query/modify another thread's context."

This is a common concurrency design question. The user wants to:
1. Have a thread pool where each thread runs tasks
2. Each thread has a "context" (state that tasks can query/modify)
3. Be able to query/modify one thread's context from another thread

This is a classic pattern for things like:
- Each OPC UA client connection runs on its own thread, with a context holding connection state, subscriptions, etc.
- Other threads need to query/modify that context (e.g., request a stop, change sampling interval, query status)

The question is where to build this. The user is working in `proto_opcua/ex` directory. There's no existing thread pool in the project. Let me ask what exactly they want, or just build a good example.

Actually let me think about the appropriate design. The user says "当前添加任务到线程池" (currently adding tasks to a thread pool) - implying there might be an existing thread pool somewhere, or they want to build one. Let me search the whole workspace for existing thread pool implementations to understand the conventions.

Let me look at the parse_project files more, and search for any thread pool existing.

**Tool: bash**

**Input:**
```json
{
  "command": "grep -rl \"ThreadPool\\|thread_pool\\|ThreadPoolExecutor\\|worker\\|task_queue\\|BlockingQueue\" /Volumes/ExtDisk/code/cpp/project/opencodes --include=\"*.h\" --include=\"*.hpp\" --include=\"*.cpp\" 2>/dev/null | head -20"
}
```

**Output:**
```
/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel/include/stage/pipeline.hpp
/Volumes/ExtDisk/code/cpp/project/opencodes/crash_cases/10_deadlock.cpp

```

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 41.9s)

_Thinking:_

Let me look at the existing thread pool implementations to understand the user's context. There are threads/cancel/include/stage/pipeline.hpp and crash_cases/10_deadlock.cpp. Let me also check the session file for the cancel project which describes the context.

Let me first ask a clarifying question or explore more. The user's request is somewhat ambiguous: "

_Thinking:_

Let me understand the context better. The user wants to add tasks to a thread pool and have them execute separately, while providing methods to query/modify the context of another thread. This is a classic concurrency pattern question. Let me check the existing thread pool in the pipeline.hpp and understand what context they might be referring to.

Let me look at the pipeline.hpp file to understand the existing thread pool pattern in this workspace.

**Tool: read**

**Input:**
```json
{
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel/include/stage/pipeline.hpp"
}
```

**Output:**
```
<path>/Volumes/ExtDisk/code/cpp/project/opencodes/threads/cancel/include/stage/pipeline.hpp</path>
<type>file</type>
<content>
1: #pragma once
2: 
3: #include <atomic>
4: #include <condition_variable>
5: #include <exception>
6: #include <functional>
7: #include <mutex>
8: #include <thread>
9: #include <vector>
10: 
11: namespace stage {
12: 
13: // Runs a sequence of stages on a dedicated worker thread.
14: //
15: // Cancellation is cooperative and only honored at stage boundaries:
16: //   - a stop request never force-kills the stage currently executing;
17: //     that stage always runs to completion;
18: //   - once the running stage finishes, all remaining stages are skipped;
19: //   - the cleanup stage always runs exactly once;
20: //   - stop() blocks until cleanup has finished, so an RPC caller can rely
21: //     on the pipeline being fully stopped when it returns.
22: //
23: // Cancellation granularity is the stage, not the thread.
24: class Pipeline {
25: public:
26:     using Step = std::function<void(Pipeline&)>;
27:     using Cleanup = std::function<void(Pipeline&, bool aborted)>;
28: 
29:     Pipeline() = default;
30:     ~Pipeline();
31: 
32:     Pipeline(const Pipeline&) = delete;
33:     Pipeline& operator=(const Pipeline&) = delete;
34: 
35:     // --- configuration (call before start()) ---
36:     void set_steps(std::vector<Step> steps);
37:     void set_cleanup(Cleanup cleanup);
38: 
39:     // --- control ---
40:     // Spawns the worker thread and begins executing stages.
41:     // Throws std::logic_error if a run is already in progress.
42:     void start();
43: 
44:     // Graceful stop: requests cancellation, then blocks until the current
45:     // stage finishes, remaining stages are skipped, and cleanup has run.
46:     // Safe to call from any thread; no-op if not running.
47:     // NOTE: must not be called from inside a stage (would self-deadlock);
48:     // stages that want to cancel themselves use request_stop() instead.
49:     void stop();
50: 
51:     // Non-blocking stop request (e.g. from an RPC handler).
52:     void request_stop() noexcept;
53: 
54:     // Blocks until all work and cleanup has finished.
55:     // Rethrows the exception that aborted the pipeline, if any.
56:     void wait();
57: 
58:     // Rethrows the exception that aborted the pipeline, if any.
59:     void rethrow_if_failed();
60: 
61:     // --- queries ---
62:     // Stages may poll this to cooperatively shorten their own work.
63:     bool stop_requested() const noexcept;
64: 
65:     // Index of the stage currently executing; -1 while idle/cleaning/finished.
66:     int current_stage() const noexcept;
67: 
68:     bool is_running() const noexcept;
69: 
70: private:
71:     void run();
72: 
73:     std::vector<Step> steps_;
74:     Cleanup cleanup_;
75: 
76:     std::atomic<bool> stop_flag_{false};
77:     std::atomic<int> current_{-1};
78: 
79:     mutable std::mutex m_;
80:     mutable std::condition_variable done_cv_;
81:     bool done_{false};
82:     std::exception_ptr failure_;
83:     std::thread worker_;
84: 
85:     bool running_locked() const { return worker_.joinable() && !done_; }
86: };
87: 
88: inline void Pipeline::set_steps(std::vector<Step> steps) {
89:     std::lock_guard<std::mutex> lk(m_);
90:     if (running_locked())
91:         throw std::logic_error("Pipeline::set_steps: pipeline already running");
92:     steps_ = std::move(steps);
93: }
94: 
95: inline void Pipeline::set_cleanup(Cleanup cleanup) {
96:     std::lock_guard<std::mutex> lk(m_);
97:     if (running_locked())
98:         throw std::logic_error("Pipeline::set_cleanup: pipeline already running");
99:     cleanup_ = std::move(cleanup);
100: }
101: 
102: inline void Pipeline::start() {
103:     std::lock_guard<std::mutex> lk(m_);
104:     if (running_locked())
105:         throw std::logic_error("Pipeline::start: already running");
106:     if (worker_.joinable())
107:         worker_.join();  // reap a previous finished run before restarting
108:     stop_flag_.store(false, std::memory_order_relaxed);
109:     failure_ = nullptr;
110:     done_ = false;
111:     current_.store(-1, std::memory_order_relaxed);
112:     worker_ = std::thread(&Pipeline::run, this);
113: }
114: 
115: inline void Pipeline::request_stop() noexcept {
116:     stop_flag_.store(true, std::memory_order_release);
117: }
118: 
119: inline void Pipeline::stop() {
120:     request_stop();
121:     wait();
122: }
123: 
124: inline void Pipeline::wait() {
125:     std::unique_lock<std::mutex> lk(m_);
126:     done_cv_.wait(lk, [this] { return done_ || !worker_.joinable(); });
127:     lk.unlock();
128:     rethrow_if_failed();
129: }
130: 
131: inline void Pipeline::rethrow_if_failed() {
132:     std::lock_guard<std::mutex> lk(m_);
133:     if (failure_)
134:         std::rethrow_exception(failure_);
135: }
136: 
137: inline bool Pipeline::stop_requested() const noexcept {
138:     return stop_flag_.load(std::memory_order_acquire);
139: }
140: 
141: inline int Pipeline::current_stage() const noexcept {
142:     return current_.load(std::memory_order_relaxed);
143: }
144: 
145: inline bool Pipeline::is_running() const noexcept {
146:     std::lock_guard<std::mutex> lk(m_);
147:     return running_locked();
148: }
149: 
150: inline void Pipeline::run() {
151:     std::exception_ptr failure;
152:     bool aborted = false;
153:     try {
154:         for (std::size_t i = 0; i < steps_.size(); ++i) {
155:             current_.store(static_cast<int>(i), std::memory_order_relaxed);
156:             steps_[i](*this);  // runs to completion, never interrupted
157:             if (stop_flag_.load(std::memory_order_acquire)) {
158:                 aborted = true;
159:                 break;
160:             }
161:         }
162:     } catch (...) {
163:         failure = std::current_exception();
164:         aborted = true;
165:     }
166: 
167:     current_.store(-1, std::memory_order_relaxed);
168: 
169:     try {
170:         if (cleanup_)
171:             cleanup_(*this, aborted);
172:     } catch (...) {
173:         if (!failure)
174:             failure = std::current_exception();
175:     }
176: 
177:     {
178:         std::lock_guard<std::mutex> lk(m_);
179:         failure_ = failure;
180:         done_ = true;
181:     }
182:     done_cv_.notify_all();
183: }
184: 
185: inline Pipeline::~Pipeline() {
186:     request_stop();
187:     if (worker_.joinable())
188:         worker_.join();
189: }
190: 
191: }  // namespace stage

(End of file - total 191 lines)
</content>
```

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 43.7s)

_Thinking:_

Now I understand the context. The user is working in an OPC UA project. They want to:
1. Add tasks to a thread pool and execute them separately
2. Provide methods to query/modify another thread's context (i.e., cross-thread context access)

This is a common concurrency pattern: a thread pool where each worker thread has its own context (per-thread context), and other threads need to query or modify that context safely.

The key design questions:
- What is "another thread's context"? Probably each worker thread has context like: which task it's running, status, progress, parameters, cancellation flags, etc.
- How do we provide safe access? Through thread-safe APIs with mutexes, or atomic variables, or shared context with mutex.

Let me think about what the user is really asking. This looks like a design/implementation question in the context of their OPC UA project. They want:
- A thread pool that dispatches tasks
- Each thread has a context
- Methods to query and modify another thread's context from any thread

This is a classic pattern. Let me design a clean solution and create a demo in the `ex/` directory.

The key design: 
- A `ThreadPool` that manages N worker threads
- Each worker thread has a `WorkerContext` containing task-specific state (task name, running flag, progress, config, cancellation flag, etc.)
- Thread-safe accessor methods: `queryContext(threadId)` and `modifyContext(threadId, ...)`

Actually, let me ask the user for clarification on what "context" means in their scenario. But given the context of the OPC UA project, each thread probably runs an OPC UA polling task with its own client, and they want to query/modify parameters (like polling interval, node list, enabled/disabled) of another thread's context.

Let me look at the parse_project to understand better what the existing structure is. There's plc_poller with poll groups, node_cache, etc.

Actually, the request is fairly clear conceptually. Let me build a thread pool with per-worker-thread context that supports querying and modifying another thread's context. I'll integrate the OPC UA client use case as an example.

Let me design:

1. `ThreadPool` class:
   - N worker threads
   - Task queue
   - Each thread has an ID and a context

2. `WorkerContext` class:
   - Shared, thread-safe context object
   - Contains OPC UA client, polling interval, node list, enabled flag, last values, etc.
   - Protected by mutex
   - Methods: query (getters), modify (setters)

3. Cross-thread access: The pool provides `getContext(threadId)` returning a shared_ptr to the context, so any thread can query/modify.

The safest pattern is that the pool itself owns a map of threadId -> shared_ptr<Context>. All access to a context goes through its own mutex. Since each context belongs to exactly one worker thread (its owner) but can be accessed by any thread, each context has its own mutex.

Let me write this as a demo. Given this is the proto_opcua project, I'll make the context hold OPC UA related state.

Let me think about the design more carefully to make it correct:

Key requirement: "能够查询修改另一线程的上下文" - provide methods to query/modify another thread's context.

Design:
```cpp
// Context: per-worker state, owned by worker, accessible by all
class TaskContext {
public:
    // query methods
    ...
    // modify methods
    ...
private:
    mutable std::mutex mtx_;
    ...
};
```

```cpp
// ThreadPool
class ThreadPool {
public:
    int submit(std::function<void(TaskContext&)> task);  // returns worker id that will run it
    std::shared_ptr<TaskContext> context(int worker_id);  // query/modify another thread's context
};
```

Actually, better: each worker thread runs a task (possibly long-running, like a polling loop). The pool assigns a task to the least busy worker. Then callers can get the worker's context by worker id and query/modify.

Let me write a complete, compilable demo. I'll put it in ex/thread_pool_context.cpp.

Let me structure:

```cpp
// Context for a worker: what another thread can query/modify
class WorkerContext {
public:
    WorkerContext(int id) : id_(id) {}

    // --- queries (safe from any thread) ---
    int id() const;
    bool running() const;
    std::string currentTask() const;
    double progress() const;
    int64_t cyclesDone() const;
    uint32_t lastStatusCode() const;
    std::vector<std::string> nodeList() const;
    // ...

    // --- modifications (safe from any thread) ---
    void setEnabled(bool);
    void setInterval(std::chrono::milliseconds);
    void requestStop();   // cooperative cancellation
    void setNodeList(...);
    // ...

private:
    mutable std::mutex mtx_;
    int id_;
    bool enabled_ = true;
    std::atomic<bool> stop_{false};
    std::chrono::milliseconds interval_{1000};
    std::string current_task_;
    double progress_ = 0;
    int64_t cycles_ = 0;
    uint32_t status_ = 0;
    std::vector<std::string> nodes_;
};
```

ThreadPool:
```cpp
class ThreadPool {
public:
    explicit ThreadPool(size_t n);
    ~ThreadPool();

    // schedule a task; returns the id of worker that picked it up
    int submit(std::function<void(std::shared_ptr<WorkerContext>)> task);

    std::shared_ptr<WorkerContext> context(int worker_id) const;

    void shutdown();
    size_t size() const;

private:
    void workerLoop(int id);
    ...
};
```

Hmm, there's a design subtlety: submit returns worker id but the task is queued; worker might not pick it up immediately. To make it deterministic for the demo, I can use a round-robin assignment: submit assigns the task to a specific worker's queue. This is cleaner for "query/modify another thread's context" because each task is bound to a worker.

So design: each worker has its own task queue + context. submit(task) -> assigns to next worker in round-robin, returns worker id. The caller can then query/modify that worker's context.

Let me implement this with a demo in main that:
1. Creates pool with 4 workers
2. Submits polling tasks (simulated OPC UA polling) to each
3. From the main thread, queries each worker's context
4. Modifies another worker's context (e.g., set interval, disable, change node list, request stop)
5. Shows the worker picking up the change

I'll base it on the OPC UA scenario: each worker simulates polling a set of PLC nodes and updating progress.

Let me write it. I'll keep it self-contained (no dependency on open62541 needed to compile the demo, but I can include it as a context). Actually to keep it buildable and simple, I'll make it standalone C++ (no OPC UA dependency), but design the context to hold OPC UA-ish fields. That way it compiles without the heavy deps.

Actually, let me check whether I should make it part of CMake build. I'll add it as a separate executable `thread_pool_demo`.

Let me also consider: should I ask the user for clarification? The request is reasonably clear: they want a thread pool with cross-thread context query/modify. I'll implement a clean, correct design. I might briefly ask whether context is per-worker or per-task... but I think I can make a reasonable design decision and explain. Given the instruction to be proactive when asked, I'll just implement.

Actually, let me reconsider - maybe ask one quick question about the exact context content they need. But that may be over-asking. The design pattern they need is clear. I'll implement a generic WorkerContext with OPC UA polling fields, plus a ThreadPool. I'll make the context generic enough (template or variant) that they can adapt.

Let me write the code now. I'll create:
- ex/thread_pool_context.h
- ex/thread_pool_context.cpp
- ex/thread_pool_demo.cpp
- Update CMakeLists.txt

Let me think about thread safety details carefully:

1. Each WorkerContext has its own mutex. All getters/setters lock it. Query/modify of another thread's context is simply calling these methods on the shared_ptr.

2. For getters returning aggregates (like node list), return by value under lock.

3. For progress updates from the worker, worker calls context methods too (same lock, no issue).

4. requestStop uses atomic for the flag so worker's cooperative loop can check cheaply without lock. Actually can use a mutex-protected bool, but atomic is cleaner.

5. The pool's worker loop: while not shutting down: pop task from own queue (with cv), set context->running, execute task(context), mark done.

For the demo, tasks are long-running polling loops, not one-shot. Let me make submit take a task function that receives the context. The task runs until context.requestStop or pool shutdown. That matches "添加任务到线程池，然后分别执行" (add tasks to thread pool, execute separately/independently).

Let me also think: after a task finishes, how does a new task get submitted to the same worker? The worker queue holds tasks; after task finishes, worker pops next. For the demo, tasks that run until stop is fine. But to demonstrate "add tasks then execute separately", I'll have the pool support multiple queued tasks per worker, executed one after another.

Let me implement a robust generic version. I'll write:

```cpp
namespace ctxpool {

class WorkerContext {
public:
    explicit WorkerContext(int id);
    ~WorkerContext();

    WorkerContext(const WorkerContext&) = delete;
    WorkerContext& operator=(const WorkerContext&) = delete;

    int id() const noexcept { return id_; }

    // ---- cross-thread queries ----
    bool enabled() const;
    bool busy() const;
    std::string current_task() const;
    uint64_t iteration() const;
    double progress() const;          // 0.0 ~ 1.0
    uint32_t last_status() const;     // OPC UA StatusCode
    std::chrono::milliseconds interval() const;
    std::vector<std::string> nodes() const;
    std::vector<double> last_values() const;
    std::string last_value_string() const;  // for printing

    // ---- cross-thread modifications ----
    void set_enabled(bool on);
    void set_interval(std::chrono::milliseconds ms);
    void set_nodes(std::vector<std::string> nodes);
    void request_stop() noexcept;   // cooperative

    // ---- worker-side updates ----
    void mark_busy(const std::string& task_name);
    void mark_idle();
    void set_progress(double p);
    void set_last_status(uint32_t code);
    void set_last_values(std::vector<double> vals);

private:
    mutable std::mutex mtx_;
    int id_;
    bool enabled_ = true;
    bool busy_ = false;
    std::string current_task_;
    uint64_t iteration_ = 0;
    double progress_ = 0.0;
    uint32_t status_ = 0;
    std::chrono::milliseconds interval_{1000};
    std::vector<std::string> nodes_;
    std::vector<double> last_values_;
    std::atomic<bool> stop_{false};
};

}  // namespace
```

Then ThreadPool:

```cpp
class ThreadPool {
public:
    explicit ThreadPool(size_t n);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    // Round-robin: schedule a long-running task onto the next worker.
    // Returns the worker id that will run it.
    size_t submit(std::function<void(std::shared_ptr<WorkerContext>)> task);

    // Access another worker's context for query / modification.
    std::shared_ptr<WorkerContext> context(size_t worker_id) const;

    // Cooperative stop of a single worker's task.
    void stopWorker(size_t worker_id);

    void shutdown();
    size_t size() const noexcept { return workers_.size(); }
    bool all_idle() const;   // helper for demo

private:
    void workerLoop(size_t id);

    struct Worker {
        std::shared_ptr<WorkerContext> ctx;
        std::thread thread;
        std::queue<std::function<void(std::shared_ptr<WorkerContext>)>> queue;
        std::mutex mtx;
        std::condition_variable cv;
        bool stop{false};
    };

    std::vector<std::unique_ptr<Worker>> workers_;
    std::atomic<size_t> next_{0};
};
```

Wait - if I use round-robin submit with a dedicated queue per worker, the worker will pop tasks in order and run them. Tasks that run until stop would block the worker queue. That's the intent: each worker runs one polling task independently ("分别执行").

For shutdown: set stop flag on all workers and notify, then join.

Let me write the workerLoop:

```cpp
void ThreadPool::workerLoop(size_t id) {
    auto w = workers_[id].get();
    for (;;) {
        std::function<void(std::shared_ptr<WorkerContext>)> task;
        {
            std::unique_lock<std::mutex> lk(w->mtx);
            w->cv.wait(lk, [w] { return w->stop || !w->queue.empty(); });
            if (w->stop && w->queue.empty()) return;
            task = std::move(w->queue.front());
            w->queue.pop();
        }
        try {
            task(w->ctx);
        } catch (...) {
            // context could record error; swallow to keep worker alive
        }
    }
}
```

submit:
```cpp
size_t ThreadPool::submit(std::function<void(std::shared_ptr<WorkerContext>)> task) {
    size_t id = next_.fetch_add(1) % workers_.size();
    auto w = workers_[id].get();
    {
        std::lock_guard<std::mutex> lk(w->mtx);
        w->queue.push(std::move(task));
    }
    w->cv.notify_one();
    return id;
}
```

Now the demo. Simulate OPC UA polling per worker:

```cpp
// task: poll a set of nodes every interval, update context
auto polling_task = [](std::shared_ptr<WorkerContext> ctx) {
    ctx->mark_busy("poll");
    int64_t cycle = 0;
    while (!ctx->stop_requested()) {
        auto nodes = ctx->nodes();
        if (ctx->enabled()) {
            std::vector<double> vals;
            for (size_t i = 0; i < nodes.size(); ++i) {
                // simulate read: value derived from node index and cycle
                double v = sin(cycle * 0.1 + i) * 100 + (ctx->id() * 10);
                vals.push_back(v);
            }
            ctx->set_last_values(vals);
            ctx->set_last_status(0); // UA_STATUSCODE_GOOD
        } else {
            ctx->set_last_status(0x805A0000); // BadNotReadable
        }
        ctx->tick(); // increments iteration, progress
        std::this_thread::sleep_for(ctx->interval());
    }
    ctx->mark_idle();
};
```

Main demo flow:
1. Create pool of 4.
2. Submit polling tasks to workers 0..3. Print assigned worker ids.
3. Wait a bit for them to run.
4. Query each worker's context (iteration, last values, status).
5. Modify worker 2's context: set_interval, set_enabled(false), set_nodes(...). Show it takes effect.
6. Query worker 1's context again to show modification reflected.
7. request_stop on worker 3 to show cooperative cancellation.
8. shutdown.

Also provide a helper print.

Now the demo should compile standalone. Let me also add CMake target.

Let me check open62541 usage — I won't use it to keep demo dependency-light, but I'll include the constants as comments. Actually the header opcua_client.h is not needed. Keep it standalone: pure C++17.

Let me now also double check the "查询修改另一线程的上下文" requirement: the key deliverable is `context(worker_id)` returning a shared_ptr that any thread can use to query/modify. I'll demonstrate from the main thread (which is a different thread than the workers).

Let me write the files.

One more design consideration: deadlock safety. If a worker task tries to modify its own context, that's fine because the task holds the context shared_ptr and calls methods; the lock is per-context. No cross-context locks held simultaneously, so no lock ordering issues. Good.

Edge: When modifying `interval`, the worker's sleep_for uses ctx->interval() which reads the latest value each loop. Good—modification takes effect on next iteration.

Let me write it all now. I'll aim for clean, correct, compileable code.

Let me write ex/thread_pool_context.h first.

我先看下现有线程池相关代码,确认设计模式后再实现。从 `threads/cancel` 和 `proto_opcua` 项目来看,你要的是:线程池调度任务 + 每线程上下文 + 跨线程查询/修改上下文。

我来实现一个完整的线程池 + WorkerContext 设计,并在 `ex/` 下给出演示程序。

**Tool: todowrite**

**Input:**
```json
{
  "todos": [
    {
      "content": "设计 WorkerContext (每线程上下文, 跨线程查询/修改)",
      "status": "in_progress",
      "priority": "high"
    },
    {
      "content": "实现 ThreadPool (任务调度 + 上下文访问)",
      "status": "pending",
      "priority": "high"
    },
    {
      "content": "编写演示程序 (查询/修改另一线程上下文)",
      "status": "pending",
      "priority": "medium"
    },
    {
      "content": "加入 CMake 并编译验证",
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
    "content": "设计 WorkerContext (每线程上下文, 跨线程查询/修改)",
    "status": "in_progress",
    "priority": "high"
  },
  {
    "content": "实现 ThreadPool (任务调度 + 上下文访问)",
    "status": "pending",
    "priority": "high"
  },
  {
    "content": "编写演示程序 (查询/修改另一线程上下文)",
    "status": "pending",
    "priority": "medium"
  },
  {
    "content": "加入 CMake 并编译验证",
    "status": "pending",
    "priority": "medium"
  }
]
```

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 30.5s)

_Thinking:_

Let me design this properly. The user wants:
1. Thread pool that accepts tasks and executes them on separate threads
2. A way to query and modify the context of another thread

This is a common pattern for things like OPC UA polling where each thread manages its own client/context, and a coordinator thread needs to query/modify the state of worker threads.

Let me design:
- `WorkerContext`: per-thread state object, guarded by a mutex, with thread-safe query/modify methods
- `ThreadPool`: manages N worker threads, each with a task queue and a context. Provides `context(id)` accessor to query/modify another thread's context.

Let me think about the design more carefully. The classic thread pool pattern where you can query/modify a specific thread's context:

```cpp
class WorkerContext {
public:
    // query methods (const, thread-safe)
    // modify methods (thread-safe)
private:
    mutable std::mutex mtx_;
    ... state ...
};

class ThreadPool {
public:
    // get context of worker i to query/modify from any thread
    std::shared_ptr<WorkerContext> context(size_t id);
    size_t size() const;
    // submit task to a specific worker or round-robin
};
```

I'll write a complete implementation in the ex/ directory, following the OPC UA theme (polling task context). Let me make it realistic: each worker thread runs a polling task with its own context that contains: node list, interval, enabled flag, last values, last poll time, status. The main thread can query and modify another thread's context (e.g., add node, change interval, disable).

Let me write this as header + demo in the ex folder. Actually let me keep it simple - one header file with everything inline, plus a demo cpp. Following project conventions.

Let me create:
- ex/thread_pool_context.h - the ThreadPool + WorkerContext implementation (header-only for simplicity)
- ex/thread_pool_demo.cpp - demo

Actually, let me reconsider. The user asked in Chinese: "当前添加任务到线程池，然后分别执行，需要提供能够查询修改另一线程的上下文的方法" = "Currently adding tasks to a thread pool, then executing separately, need to provide methods that can query and modify another thread's context."

So they want the thread pool + context access pattern. Let me write a clean, self-contained implementation with a demo. I'll make it header-only to make it easy to integrate.

Let me write the code.

现在我实现了核心设计。设计要点:每个 worker 线程拥有独立的 `WorkerContext`(由 mutex 保护),线程池通过 `context(id)` 暴露访问入口,主线程或其他线程即可安全地查询/修改任意 worker 的上下文。

**Tool: write**

**Input:**
```json
{
  "content": "#pragma once\n\n#include <atomic>\n#include <condition_variable>\n#include <functional>\n#include <memory>\n#include <mutex>\n#include <queue>\n#include <string>\n#include <thread>\n#include <vector>\n\nnamespace opcua {\n\n// ============================================================================\n// WorkerContext\n// ============================================================================\n// 每个工作线程独立拥有的上下文对象。\n// 该上下文由 mutex 保护, 可被任意线程 (包括主线程 / 其他 worker) 查询与修改,\n// 用于让\"外部\"线程监控和调节某一条工作线程的运行参数。\n//\n// 设计约束:\n//   - 所有公开方法线程安全 (内部加锁);\n//   - 上下文的生命周期由线程池管理 (shared_ptr),\n//     因此其他线程持有引用时不会悬垂;\n//   - 工作线程通过 update* 方法报告自身运行状态;\n//   - 外部线程通过 set* / get* 方法调节 / 查看运行参数。\n// ============================================================================\nclass WorkerContext {\npublic:\n    explicit WorkerContext(uint32_t id) : id_(id) {}\n\n    WorkerContext(const WorkerContext&) = delete;\n    WorkerContext& operator=(const WorkerContext&) = delete;\n\n    // ---- 标识 ----\n    uint32_t id() const { return id_; }\n\n    // ---- 由 worker 自身报告的运行状态 (其他线程可查询) ----\n    bool running() const { return running_.load(std::memory_order_acquire); }\n    uint64_t pollCount() const {\n        std::lock_guard<std::mutex> lk(mtx_);\n        return poll_count_;\n    }\n    std::string lastValue() const {\n        std::lock_guard<std::mutex> lk(mtx_);\n        return last_value_;\n    }\n    uint64_t lastPollMs() const {\n        std::lock_guard<std::mutex> lk(mtx_);\n        return last_poll_ms_;\n    }\n\n    // ---- 外部线程可查询 / 修改的配置参数 ----\n    std::string name() const {\n        std::lock_guard<std::mutex> lk(mtx_);\n        return name_;\n    }\n    void setName(const std::string& name) {\n        std::lock_guard<std::mutex> lk(mtx_);\n        name_ = name;\n    }\n\n    bool enabled() const {\n        std::lock_guard<std::mutex> lk(mtx_);\n        return enabled_;\n    }\n    void setEnabled(bool on) {\n        std::lock_guard<std::mutex> lk(mtx_);\n        enabled_ = on;\n    }\n\n    uint32_t pollIntervalMs() const {\n        std::lock_guard<std::mutex> lk(mtx_);\n        return poll_interval_ms_;\n    }\n    void setPollIntervalMs(uint32_t ms) {\n        std::lock_guard<std::mutex> lk(mtx_);\n        poll_interval_ms_ = ms;\n    }\n\n    std::vector<uint32_t> nodeIds() const {\n        std::lock_guard<std::mutex> lk(mtx_);\n        return node_ids_;\n    }\n    void setNodeIds(std::vector<uint32_t> ids) {\n        std::lock_guard<std::mutex> lk(mtx_);\n        node_ids_ = std::move(ids);\n    }\n    void addNodeId(uint32_t id) {\n        std::lock_guard<std::mutex> lk(mtx_);\n        node_ids_.push_back(id);\n    }\n\n    // 请求 worker 停止 (协同式取消)。worker 自身在循环中检查 shouldStop()。\n    void requestStop() { stop_.store(true, std::memory_order_release); }\n    bool shouldStop() const { return stop_.load(std::memory_order_acquire); }\n    void clearStop() { stop_.store(false, std::memory_order_release); }\n\n    // ---- 供 worker 自身调用的状态报告 ----\n    void markRunning() { running_.store(true, std::memory_order_release); }\n    void markStopped() { running_.store(false, std::memory_order_release); }\n    void reportPoll(uint64_t poll_ms, const std::string& value) {\n        std::lock_guard<std::mutex> lk(mtx_);\n        ++poll_count_;\n        last_poll_ms_ = poll_ms;\n        last_value_ = value;\n    }\n\nprivate:\n    uint32_t id_ = 0;\n\n    mutable std::mutex mtx_;\n    std::string name_;\n    bool enabled_ = true;\n    uint32_t poll_interval_ms_ = 1000;\n    std::vector<uint32_t> node_ids_;\n\n    // worker 自身写入, 外部线程只读:\n    uint64_t poll_count_ = 0;\n    uint64_t last_poll_ms_ = 0;\n    std::string last_value_;\n\n    std::atomic<bool> running_{false};\n    std::atomic<bool> stop_{false};\n};\n\n// ============================================================================\n// ThreadPool\n// ============================================================================\n// 固定线程数的工作池。每个 worker 线程:\n//   - 拥有唯一的 WorkerContext (通过 context(id) 可获取);\n//   - 持有一条独立的任务队列 (tasks[id]);\n//   - 循环执行队列中最新提交的任务函数, 任务可运行任意时长;\n//   - 任务函数接收本线程的 shared_ptr<WorkerContext>,\n//     以便 (a) 查询/更新自身状态, (b) 需要时访问其他 worker 的上下文。\n//\n// 使用模式:\n//   ThreadPool pool(4);\n//   auto ctx = pool.context(2);            // 查询 / 修改 worker#2 的上下文\n//   ctx->setEnabled(false);\n//   pool.submit(2, [ctx](auto){ ... });    // 向 worker#2 提交任务\n// ============================================================================\nclass ThreadPool {\npublic:\n    using Task = std::function<void(std::shared_ptr<WorkerContext>)>;\n\n    explicit ThreadPool(size_t num_workers) {\n        for (size_t i = 0; i < num_workers; ++i) {\n            auto ctx = std::make_shared<WorkerContext>(static_cast<uint32_t>(i));\n            ctx->setName(\"worker-\" + std::to_string(i));\n            workers_.emplace_back(\n                std::thread(&ThreadPool::run, this, i),\n                ctx, std::queue<Task>{});\n        }\n    }\n\n    ~ThreadPool() {\n        shutdown();\n    }\n\n    ThreadPool(const ThreadPool&) = delete;\n    ThreadPool& operator=(const ThreadPool&) = delete;\n\n    size_t size() const { return workers_.size(); }\n\n    // 获取某个 worker 的上下文。任意线程均可调用, 从而查询 / 修改该 worker。\n    std::shared_ptr<WorkerContext> context(size_t id) const {\n        return id < workers_.size() ? workers_[id].ctx : nullptr;\n    }\n\n    // 向指定 worker 提交一个任务。任务函数会在该 worker 线程上执行。\n    bool submit(size_t id, Task task) {\n        if (id >= workers_.size()) return false;\n        {\n            std::lock_guard<std::mutex> lk(workers_[id].mtx);\n            workers_[id].tasks.push(std::move(task));\n        }\n        workers_[id].cv.notify_one();\n        return true;\n    }\n\n    // 轮询分配: 将任务提交给\"下一个\"空闲 worker。返回被选中的 worker id, 失败返回 SIZE_MAX。\n    size_t submitRoundRobin(Task task) {\n        for (size_t i = 0; i < workers_.size(); ++i) {\n            size_t id = (next_round_robin_.fetch_add(1)) % workers_.size();\n            if (submit(id, std::move(task))) return id;\n        }\n        return SIZE_MAX;\n    }\n\n    // 请求全部 worker 停止当前任务并退出。阻塞直到所有线程结束。\n    void shutdown() {\n        for (auto& w : workers_) {\n            {\n                std::lock_guard<std::mutex> lk(w.mtx);\n                w.ctx->requestStop();\n                w.shutdown = true;\n            }\n            w.cv.notify_all();\n        }\n        for (auto& w : workers_) {\n            if (w.thread.joinable()) w.thread.join();\n        }\n    }\n\n    // 仅请求指定 worker 的当前任务协同退出 (不销毁线程)。\n    void stopWorker(size_t id) {\n        if (auto ctx = context(id)) ctx->requestStop();\n    }\n\nprivate:\n    struct Worker {\n        std::thread thread;\n        std::shared_ptr<WorkerContext> ctx;\n        std::queue<Task> tasks;\n        std::mutex mtx;\n        std::condition_variable cv;\n        bool shutdown = false;\n    };\n\n    void run(size_t id) {\n        auto ctx = workers_[id].ctx;\n        ctx->markRunning();\n\n        while (true) {\n            Task task;\n            {\n                std::unique_lock<std::mutex> lk(workers_[id].mtx);\n                workers_[id].cv.wait(lk, [&] {\n                    return workers_[id].shutdown || !workers_[id].tasks.empty();\n                });\n                if (workers_[id].tasks.empty()) {\n                    if (workers_[id].shutdown) break;\n                    continue;\n                }\n                task = std::move(workers_[id].tasks.front());\n                workers_[id].tasks.pop();\n            }\n            ctx->clearStop();\n            try {\n                task(ctx);\n            } catch (const std::exception&) {\n                // 任务异常不应终止 worker 线程, 交由上层自行处理\n            }\n        }\n\n        ctx->markStopped();\n    }\n\n    std::vector<Worker> workers_;\n    std::atomic<size_t> next_round_robin_{0};\n};\n\n}  // namespace opcua\n",
  "filePath": "/Volumes/ExtDisk/code/cpp/project/opencodes/proto_opcua/ex/thread_pool_context.h"
}
```

**Output:**
```
Wrote file successfully.
```

---

## Assistant (Build · DeepSeek V4 Flash Free (New) · 6.5s)

---

