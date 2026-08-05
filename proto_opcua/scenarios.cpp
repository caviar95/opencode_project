// ============================================================================
// scenarios.cpp - 所有使用场景的完整用例 (Protobuf + OPC UA + PLC)
// ============================================================================
// 前提: 假设 PLC 已运行 OPC UA 服务器, 地址 opc.tcp://192.168.1.100:4840
// 所有 NodeId 仅作示例用途, 按实际 PLC 变量地址替换
// ============================================================================

#include "opcua_client.h"
#include <open62541/types.h>
#include <iostream>
#include <thread>
#include <chrono>

static const std::string PLC_ENDPOINT = "opc.tcp://192.168.1.100:4840";

// ============================================================================
// 场景 1: 连接/断开 PLC
// ============================================================================
void scenario_connect_disconnect() {
    std::cout << "\n=== 场景 1: 连接/断开 PLC ===\n";
    OPCUAClient client(PLC_ENDPOINT);

    if (client.connect(5000)) {
        std::cout << "PLC 连接成功\n";
        // ... 执行操作 ...
        client.disconnect();
        std::cout << "PLC 已断开\n";
    } else {
        std::cout << "PLC 连接失败\n";
    }
}

// ============================================================================
// 场景 2: 读取标量值 (基本数据类型)
// ============================================================================
void scenario_read_scalar_types() {
    std::cout << "\n=== 场景 2: 读取标量值 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    // 使用 protobuf 构建 NodeId (namespace 0, numeric id)
    auto bool_node   = OPCUAClient::numericNodeId(0, 1001);
    auto int32_node  = OPCUAClient::numericNodeId(0, 1002);
    auto int64_node  = OPCUAClient::numericNodeId(0, 1003);
    auto uint32_node = OPCUAClient::numericNodeId(0, 1004);
    auto float_node  = OPCUAClient::numericNodeId(0, 1005);
    auto double_node = OPCUAClient::numericNodeId(0, 1006);
    auto string_node = OPCUAClient::numericNodeId(0, 1007);

    // 方案 A: 使用封装好的类型安全接口 (推荐)
    bool b   = client.readBool(bool_node);
    int32_t i32 = client.readInt32(int32_node);
    int64_t i64 = client.readInt64(int64_node);
    uint32_t u32 = client.readUInt32(uint32_node);
    float f   = client.readFloat(float_node);
    double d  = client.readDouble(double_node);
    std::string s = client.readString(string_node);

    std::cout << "Bool:   " << b << "\n";
    std::cout << "Int32:  " << i32 << "\n";
    std::cout << "Int64:  " << i64 << "\n";
    std::cout << "UInt32: " << u32 << "\n";
    std::cout << "Float:  " << f << "\n";
    std::cout << "Double: " << d << "\n";
    std::cout << "String: " << s << "\n";

    // 方案 B: 使用通用 protobuf 请求/响应 (需自行处理类型)
    opcua::ReadRequest req;
    *req.mutable_node_id() = int32_node;
    req.set_attribute_id(13);    // UA_ATTRIBUTEID_VALUE
    req.set_include_timestamp(true);

    opcua::ReadResponse resp = client.read(req);
    if (resp.status().code() == 0) {  // UA_STATUSCODE_GOOD
        std::cout << "Value via proto: " << resp.value().int32_val()
                  << ", server_ts: " << resp.server_timestamp() << "\n";
    }
}

// ============================================================================
// 场景 3: 读取数组值
// ============================================================================
void scenario_read_array_types() {
    std::cout << "\n=== 场景 3: 读取数组值 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    auto i32_arr_node = OPCUAClient::numericNodeId(0, 2001);
    auto f32_arr_node = OPCUAClient::numericNodeId(0, 2002);
    auto f64_arr_node = OPCUAClient::numericNodeId(0, 2003);

    // 读取 int32 数组
    std::vector<int32_t> i32_vals = client.readInt32Array(i32_arr_node);
    std::cout << "Int32 Array (" << i32_vals.size() << " elements):";
    for (auto v : i32_vals) std::cout << " " << v;
    std::cout << "\n";

    // 读取 float 数组
    std::vector<float> f32_vals = client.readFloatArray(f32_arr_node);
    std::cout << "Float Array (" << f32_vals.size() << " elements):";
    for (auto v : f32_vals) std::cout << " " << v;
    std::cout << "\n";

    // 读取 double 数组
    std::vector<double> f64_vals = client.readDoubleArray(f64_arr_node);
    std::cout << "Double Array (" << f64_vals.size() << " elements):";
    for (auto v : f64_vals) std::cout << " " << v;
    std::cout << "\n";
}

// ============================================================================
// 场景 4: 读取非 Value 属性 (如 DisplayName, Description)
// ============================================================================
void scenario_read_attributes() {
    std::cout << "\n=== 场景 4: 读取节点属性 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    auto node = OPCUAClient::numericNodeId(0, 1001);

    // 读取 DisplayName (attribute_id = 14)
    auto dn_resp = client.readAttribute(node, 14);
    std::cout << "DisplayName: " << dn_resp.value().string_val() << "\n";

    // 读取 Description (attribute_id = 15)
    auto desc_resp = client.readAttribute(node, 15);
    std::cout << "Description: " << desc_resp.value().string_val() << "\n";

    // 读取 NodeClass (attribute_id = 2)
    auto nc_resp = client.readAttribute(node, 2);
    std::cout << "NodeClass: " << nc_resp.value().int32_val() << "\n";

    // 读取 BrowseName (attribute_id = 5)
    auto bn_resp = client.readAttribute(node, 5);
    std::cout << "BrowseName: " << bn_resp.value().string_val() << "\n";

    // 读取 WriteMask (attribute_id = 21)
    auto wm_resp = client.readAttribute(node, 21);
    std::cout << "WriteMask: " << wm_resp.value().uint32_val() << "\n";

    // 读取 UserWriteMask (attribute_id = 22)
    auto uwm_resp = client.readAttribute(node, 22);
    std::cout << "UserWriteMask: " << uwm_resp.value().uint32_val() << "\n";

    // 读取 DataType (attribute_id = 6)
    auto dt_resp = client.readAttribute(node, 6);
    std::cout << "DataType NodeId: " << dt_resp.value().uint32_val() << "\n";
}

// ============================================================================
// 场景 5: 写入标量值
// ============================================================================
void scenario_write_scalar_types() {
    std::cout << "\n=== 场景 5: 写入标量值 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    auto bool_node   = OPCUAClient::numericNodeId(0, 1001);
    auto int32_node  = OPCUAClient::numericNodeId(0, 1002);
    auto float_node  = OPCUAClient::numericNodeId(0, 1005);
    auto string_node = OPCUAClient::numericNodeId(0, 1007);

    // 方案 A: 类型安全接口
    bool ok1 = client.writeBool(bool_node, true);
    bool ok2 = client.writeInt32(int32_node, 42);
    bool ok3 = client.writeFloat(float_node, 3.14f);
    bool ok4 = client.writeString(string_node, "hello plc");

    std::cout << "Write Bool:   " << (ok1 ? "OK" : "FAIL") << "\n";
    std::cout << "Write Int32:  " << (ok2 ? "OK" : "FAIL") << "\n";
    std::cout << "Write Float:  " << (ok3 ? "OK" : "FAIL") << "\n";
    std::cout << "Write String: " << (ok4 ? "OK" : "FAIL") << "\n";

    // 写入后验证
    int32_t verify = client.readInt32(int32_node);
    std::cout << "Verify read after write: " << verify << " (expect 42)\n";

    // 方案 B: 通用 protobuf 写入
    opcua::WriteRequest wreq;
    *wreq.mutable_node_id() = int32_node;
    wreq.mutable_value()->set_type(opcua::DataTypeId::INT32);
    wreq.mutable_value()->set_int32_val(100);

    opcua::WriteResponse wresp = client.write(wreq);
    std::cout << "Write via proto: "
              << (wresp.status().code() == 0 ? "OK" : "FAIL") << "\n";
}

// ============================================================================
// 场景 6: 写入数组值
// ============================================================================
void scenario_write_array_types() {
    std::cout << "\n=== 场景 6: 写入数组值 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    auto i32_arr_node = OPCUAClient::numericNodeId(0, 2001);
    auto f32_arr_node = OPCUAClient::numericNodeId(0, 2002);

    // 写入 int32 数组
    bool ok = client.writeInt32Array(i32_arr_node, {10, 20, 30, 40, 50});
    std::cout << "Write Int32 Array: " << (ok ? "OK" : "FAIL") << "\n";

    // 写入 float 数组
    ok = client.writeFloatArray(f32_arr_node, {1.1f, 2.2f, 3.3f});
    std::cout << "Write Float Array: " << (ok ? "OK" : "FAIL") << "\n";
}

// ============================================================================
// 场景 7: 批量读取 (Batch Read)
// ============================================================================
void scenario_batch_read() {
    std::cout << "\n=== 场景 7: 批量读取 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    // 一次请求中读取多个 PLC 变量节点
    opcua::BatchReadRequest batch_req;

    auto add_read = [&](uint32_t ns, uint32_t id) {
        auto* r = batch_req.add_requests();
        r->mutable_node_id()->set_namespace_index(ns);
        r->mutable_node_id()->set_numeric_id(id);
        r->set_attribute_id(13);  // Value
    };

    // 模拟从配置文件/配表来的节点列表
    add_read(0, 1001);   // Bool
    add_read(0, 1002);   // Int32
    add_read(0, 1005);   // Float
    add_read(0, 1007);   // String
    add_read(0, 2001);   // Int32 Array

    opcua::BatchReadResponse batch_resp = client.batchRead(batch_req);
    std::cout << "Batch read " << batch_resp.responses_size() << " nodes:\n";
    for (int i = 0; i < batch_resp.responses_size(); ++i) {
        const auto& r = batch_resp.responses(i);
        std::cout << "  Node[" << i << "] status=" << r.status().code()
                  << " type=" << r.value().type() << "\n";
    }
}

// ============================================================================
// 场景 8: 批量写入 (Batch Write)
// ============================================================================
void scenario_batch_write() {
    std::cout << "\n=== 场景 8: 批量写入 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    opcua::BatchWriteRequest batch_wreq;

    // 同时写入多个节点
    auto add_write_int = [&](uint32_t ns, uint32_t id, int32_t val) {
        auto* w = batch_wreq.add_requests();
        w->mutable_node_id()->set_namespace_index(ns);
        w->mutable_node_id()->set_numeric_id(id);
        w->mutable_value()->set_type(opcua::DataTypeId::INT32);
        w->mutable_value()->set_int32_val(val);
    };

    add_write_int(0, 1002, 10);
    add_write_int(0, 1003, 20);
    add_write_int(0, 1004, 30);

    opcua::BatchWriteResponse batch_wresp = client.batchWrite(batch_wreq);
    std::cout << "Batch write results:\n";
    for (int i = 0; i < batch_wresp.responses_size(); ++i) {
        std::cout << "  Write[" << i << "]: "
                  << (batch_wresp.responses(i).status().code() == 0 ? "OK" : "FAIL")
                  << "\n";
    }
}

// ============================================================================
// 场景 9: 遍历/浏览 PLC 地址空间 (Browse)
// ============================================================================
void scenario_browse() {
    std::cout << "\n=== 场景 9: 浏览 PLC 地址空间 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    // 从 Objects 根节点浏览 (ns=0, id=85)
    auto root = OPCUAClient::numericNodeId(0, 85);

    // 9a: 浏览所有子节点
    std::cout << "--- 9a: 浏览所有子节点 ---\n";
    auto all_children = client.browseChildren(root);
    for (const auto& ref : all_children) {
        std::cout << "  " << ref.display_name()
                  << " (class=" << ref.node_class() << ")\n";
    }

    // 9b: 只浏览变量节点 (node_class_mask = 2)
    std::cout << "--- 9b: 只浏览变量节点 ---\n";
    auto vars = client.browseVariables(root);
    for (const auto& ref : vars) {
        std::cout << "  Variable: " << ref.display_name() << "\n";
    }

    // 9c: 只浏览对象节点 (node_class_mask = 1)
    std::cout << "--- 9c: 只浏览对象节点 ---\n";
    auto objs = client.browseObjects(root);
    for (const auto& ref : objs) {
        std::cout << "  Object: " << ref.display_name() << "\n";
    }

    // 9d: 递归浏览 (遍历树上所有节点, 限制深度)
    std::cout << "--- 9d: 递归浏览 (depth=2) ---\n";
    auto recursive = client.browseRecursive(root, 2);
    for (const auto& ref : recursive.references()) {
        std::cout << "  " << ref.display_name()
                  << " (class=" << ref.node_class() << ")\n";
    }

    // 9e: 使用 protobuf 请求自定义过滤
    std::cout << "--- 9e: 用 proto 自定义 Browse ---\n";
    opcua::BrowseRequest breq;
    *breq.mutable_node_id() = root;
    breq.set_node_class_mask(2);  // 只找 Variable
    breq.set_max_references(20);

    auto bresp = client.browse(breq);
    for (const auto& ref : bresp.references()) {
        std::cout << "  ProtoBrowse: " << ref.display_name() << "\n";
    }
}

// ============================================================================
// 场景 10: 订阅数据变化 (Subscribe/MonitoredItem)
// ============================================================================
void scenario_subscription() {
    std::cout << "\n=== 场景 10: 订阅 PLC 数据变化 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    auto temp_node = OPCUAClient::numericNodeId(0, 3001);  // 温度变量

    // 订阅温度变量, 100ms 采样间隔
    opcua::SubscribeRequest sub_req;
    *sub_req.mutable_node_id() = temp_node;
    sub_req.set_sampling_interval_ms(100.0);
    sub_req.set_queue_size(10);
    sub_req.set_discard_oldest(true);

    auto sub_resp = client.subscribe(sub_req,
        [](const opcua::DataChangeNotification& notif) {
            std::cout << "[Sub] mon_id=" << notif.monitored_item_id()
                      << " value=";
            if (notif.value().type() == opcua::DataTypeId::FLOAT)
                std::cout << notif.value().float_val();
            else if (notif.value().type() == opcua::DataTypeId::DOUBLE)
                std::cout << notif.value().double_val();
            else if (notif.value().type() == opcua::DataTypeId::INT32)
                std::cout << notif.value().int32_val();
            std::cout << " ts=" << notif.source_timestamp() << "\n";
        });

    if (sub_resp.status().code() == 0) {
        std::cout << "Subscribed: sub_id=" << sub_resp.subscription_id()
                  << " mon_id=" << sub_resp.monitored_item_id() << "\n";
    }

    // 保持运行 10s 收集数据变化
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // 取消订阅
    opcua::UnsubscribeRequest unsub_req;
    unsub_req.set_subscription_id(sub_resp.subscription_id());
    unsub_req.set_monitored_item_id(sub_resp.monitored_item_id());
    client.unsubscribe(unsub_req);
    std::cout << "Unsubscribed\n";
}

// ============================================================================
// 场景 11: 调用 PLC 方法 (Method Call)
// ============================================================================
void scenario_method_call() {
    std::cout << "\n=== 场景 11: 调用 PLC 方法 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    // 调用 PLC 上的方法: Objects.MyPLC.ResetCounter
    auto obj_node  = OPCUAClient::numericNodeId(0, 5001);  // 对象节点
    auto mtd_node  = OPCUAClient::numericNodeId(0, 5002);  // 方法节点

    // 无参调用
    opcua::CallMethodRequest call_req;
    *call_req.mutable_object_id() = obj_node;
    *call_req.mutable_method_id() = mtd_node;

    auto call_resp = client.callMethod(call_req);
    std::cout << "Method call (no args): "
              << (call_resp.status().code() == 0 ? "OK" : "FAIL") << "\n";

    // 带参调用: SetSpeed(speed)
    auto speed_method = OPCUAClient::numericNodeId(0, 5003);
    opcua::CallMethodRequest call_req2;
    *call_req2.mutable_object_id() = obj_node;
    *call_req2.mutable_method_id() = speed_method;

    auto* arg = call_req2.add_input_args();
    arg->set_type(opcua::DataTypeId::FLOAT);
    arg->set_float_val(1500.5f);

    auto call_resp2 = client.callMethod(call_req2);
    if (call_resp2.status().code() == 0 && call_resp2.output_args_size() > 0) {
        // 假设返回 bool 表示成功
        bool success = call_resp2.output_args(0).bool_val();
        std::cout << "SetSpeed(1500.5) returned: " << success << "\n";
    }
}

// ============================================================================
// 场景 12: 读取历史数据 (Historical Access)
// ============================================================================
void scenario_history_read() {
    std::cout << "\n=== 场景 12: 读取历史数据 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    auto hist_node = OPCUAClient::numericNodeId(0, 4001);

    // 读取过去 1 小时的历史数据
    uint64_t now = UA_DateTime_now();
    uint64_t one_hour_ago = now - 3600 * 1000 * 10000;  // UA_DateTime 单位 100ns

    opcua::HistoryReadRequest hist_req;
    *hist_req.mutable_node_id() = hist_node;
    hist_req.set_start_time(one_hour_ago);
    hist_req.set_end_time(now);
    hist_req.set_max_values(100);

    auto hist_resp = client.historyReadRaw(hist_req);
    if (hist_resp.status().code() == 0) {
        std::cout << "History data points: " << hist_resp.values_size() << "\n";
        for (int i = 0; i < hist_resp.values_size(); ++i) {
            std::cout << "  [" << i << "] val=";
            if (hist_resp.values(i).type() == opcua::DataTypeId::FLOAT)
                std::cout << hist_resp.values(i).float_val();
            else if (hist_resp.values(i).type() == opcua::DataTypeId::DOUBLE)
                std::cout << hist_resp.values(i).double_val();
            std::cout << " ts=" << hist_resp.timestamps(i) << "\n";
        }
    }
}

// ============================================================================
// 场景 13: 通过字符串 NodeId 访问 PLC 变量 (西门子 S7-1500 等)
// ============================================================================
void scenario_string_nodeid() {
    std::cout << "\n=== 场景 13: 字符串 NodeId 访问 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    // 有些 PLC (如西门子) 使用字符串 NodeId:
    //   ns=3;s="::Program:MainProgram.Temperature"
    //   ns=3;s="::Program:MainProgram.Pressure"
    auto temp_node = OPCUAClient::stringNodeId(3, "::Program:MainProgram.Temperature");
    auto press_node = OPCUAClient::stringNodeId(3, "::Program:MainProgram.Pressure");
    auto speed_node = OPCUAClient::stringNodeId(3, "::Program:MainProgram.MotorSpeed");

    float temp  = client.readFloat(temp_node);
    float press = client.readFloat(press_node);
    float speed = client.readFloat(speed_node);

    std::cout << "Temperature: " << temp << " C\n";
    std::cout << "Pressure: " << press << " bar\n";
    std::cout << "MotorSpeed: " << speed << " rpm\n";

    // 写入
    client.writeFloat(speed_node, 1200.0f);
    std::cout << "MotorSpeed set to 1200\n";
}

// ============================================================================
// 场景 14: 写入不同属性 (如 EngineeringUnits 等)
// ============================================================================
void scenario_write_attribute() {
    std::cout << "\n=== 场景 14: 写入属性 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    auto node = OPCUAClient::numericNodeId(0, 1002);

    // 写 Description (attribute_id = 15)
    opcua::WriteRequest wreq;
    *wreq.mutable_node_id() = node;
    wreq.set_attribute_id(15);   // UA_ATTRIBUTEID_DESCRIPTION
    wreq.mutable_value()->set_type(opcua::DataTypeId::STRING);
    wreq.mutable_value()->set_string_val("Engine RPM value");

    auto wresp = client.write(wreq);
    std::cout << "Write Description: "
              << (wresp.status().code() == 0 ? "OK" : "FAIL") << "\n";
}

// ============================================================================
// 场景 15: 读写大端/位域 (通过 protobuf bytes 传递原始数据)
// ============================================================================
void scenario_raw_bytes() {
    std::cout << "\n=== 场景 15: 原始字节读写 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    auto node = OPCUAClient::numericNodeId(0, 6001);

    // 读取 4 字节原始数据
    auto resp = client.readValue(node);
    if (resp.value().type() == opcua::DataTypeId::BYTESTRING) {
        std::string raw = resp.value().bytes_val();
        std::cout << "Raw bytes (" << raw.size() << "): ";
        for (unsigned char c : raw)
            std::cout << std::hex << (int)c << " ";
        std::cout << std::dec << "\n";

        // 解析位域 (假设 byte[0] 的低 4 位是状态标志)
        uint8_t status_flags = static_cast<uint8_t>(raw[0]) & 0x0F;
        std::cout << "Status flags (lower nibble): " << (int)status_flags << "\n";
    }

    // 写入 4 字节原始数据
    opcua::WriteRequest wreq;
    *wreq.mutable_node_id() = node;
    wreq.mutable_value()->set_type(opcua::DataTypeId::BYTESTRING);
    wreq.mutable_value()->set_bytes_val(std::string({0x01, 0x02, 0x03, 0x04}));
    client.write(wreq);
}

// ============================================================================
// 场景 16: 读写 LocalizedText / QualifiedName 类型
// ============================================================================
void scenario_structured_types() {
    std::cout << "\n=== 场景 16: 结构化类型 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    auto node = OPCUAClient::numericNodeId(0, 7001);

    // 通用 Read/Write 请求通过 protobuf VariantValue 传递
    opcua::ReadRequest req;
    *req.mutable_node_id() = node;
    auto resp = client.read(req);

    if (resp.status().code() == 0) {
        std::cout << "Read type=" << resp.value().type()
                  << " string_val=" << resp.value().string_val() << "\n";
    }

    // QualifiedName 可编码为字符串 "ns:name"
    // LocalizedText 可编码为字符串 "locale:text"
    opcua::WriteRequest wreq;
    *wreq.mutable_node_id() = node;
    wreq.mutable_value()->set_type(opcua::DataTypeId::LOCALIZEDTEXT);
    wreq.mutable_value()->set_string_val("en_US:Temperature Sensor");
    client.write(wreq);
}

// ============================================================================
// 场景 17: 使用 protobuf OPCUARequest/OPCUAResponse 统一消息封装
// ============================================================================
void scenario_unified_message() {
    std::cout << "\n=== 场景 17: 统一消息封装 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    // 构建统一的请求消息
    opcua::OPCUARequest req;
    req.set_request_id(1);

    auto* read_req = req.mutable_read();
    read_req->mutable_node_id()->set_namespace_index(0);
    read_req->mutable_node_id()->set_numeric_id(1002);
    read_req->set_attribute_id(13);

    // 序列化为 bytes (用于网络传输 / 消息队列 / 日志存储)
    std::string wire_bytes;
    req.SerializeToString(&wire_bytes);
    std::cout << "Serialized OPCUARequest: " << wire_bytes.size() << " bytes\n";

    // 反序列化并执行
    opcua::OPCUARequest parsed_req;
    parsed_req.ParseFromString(wire_bytes);

    opcua::OPCUAResponse resp;
    resp.set_request_id(parsed_req.request_id());

    switch (parsed_req.request_case()) {
    case opcua::OPCUARequest::kRead: {
        auto read_resp = client.read(parsed_req.read());
        *resp.mutable_read() = read_resp;
        break;
    }
    default:
        break;
    }

    // 序列化响应
    std::string resp_bytes;
    resp.SerializeToString(&resp_bytes);
    std::cout << "Response serialized: " << resp_bytes.size() << " bytes\n";
    if (resp.read().status().code() == 0) {
        std::cout << "Read value: " << resp.read().value().int32_val() << "\n";
    }
}

// ============================================================================
// 场景 18: 安全连接 (用户名/密码 + 证书)
// ============================================================================
void scenario_secure_connection() {
    std::cout << "\n=== 场景 18: 安全连接 (示意) ===\n";
    OPCUAClient client("opc.tcp://192.168.1.100:4840");
    // 注意: 安全连接需要额外配置 UA_ClientConfig 设置证书和密码
    // 以下为概念示例, 需要根据 open62541 安全配置 API 实现
    /*
    auto* config = UA_Client_getConfig(client.getNative());
    config->clientDescription.applicationName = UA_LOCALIZEDTEXT("", "MyApp");
    // 设置用户名密码
    UA_ClientConfig_setAuthenticationUsername(config, "admin", "password");
    // 加载证书
    UA_ByteString certificate = loadFile("client_cert.der");
    UA_ByteString privateKey  = loadFile("client_key.der");
    UA_ClientConfig_setSecurityPolicies(config, &certificate, &privateKey,
                                         certificate, privateKey);
    */
    if (client.connect()) {
        std::cout << "Secure connection established\n";
    }
}

// ============================================================================
// 场景 19: 从配置文件/配表动态加载节点列表并轮询
// ============================================================================
void scenario_polling_from_config() {
    std::cout << "\n=== 场景 19: 配表驱动轮询 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    // 模拟从 JSON/YAML/CSV 配表加载的变量监控列表
    struct PollConfig {
        std::string name;
        uint32_t ns;
        uint32_t id;
        opcua::DataTypeId dtype;
    };

    std::vector<PollConfig> poll_list = {
        {"Temperature",  0, 1005, opcua::DataTypeId::FLOAT},
        {"Pressure",     0, 1006, opcua::DataTypeId::FLOAT},
        {"MotorSpeed",   0, 1002, opcua::DataTypeId::INT32},
        {"IsRunning",    0, 1001, opcua::DataTypeId::BOOL},
        {"AlarmCode",    0, 1003, opcua::DataTypeId::INT64},
    };

    // 批量读取 (高效)
    opcua::BatchReadRequest batch_req;
    for (const auto& cfg : poll_list) {
        auto* r = batch_req.add_requests();
        r->mutable_node_id()->set_namespace_index(cfg.ns);
        r->mutable_node_id()->set_numeric_id(cfg.id);
    }

    auto batch_resp = client.batchRead(batch_req);
    for (int i = 0; i < batch_resp.responses_size(); ++i) {
        std::cout << poll_list[i].name << " = ";
        switch (poll_list[i].dtype) {
        case opcua::DataTypeId::BOOL:
            std::cout << batch_resp.responses(i).value().bool_val(); break;
        case opcua::DataTypeId::INT32:
            std::cout << batch_resp.responses(i).value().int32_val(); break;
        case opcua::DataTypeId::INT64:
            std::cout << batch_resp.responses(i).value().int64_val(); break;
        case opcua::DataTypeId::FLOAT:
            std::cout << batch_resp.responses(i).value().float_val(); break;
        default:
            std::cout << "(type " << batch_resp.responses(i).value().type() << ")";
        }
        std::cout << "\n";
    }
}

// ============================================================================
// 场景 20: 异常/故障处理模式
// ============================================================================
void scenario_error_handling() {
    std::cout << "\n=== 场景 20: 异常/故障处理 ===\n";
    OPCUAClient client(PLC_ENDPOINT);

    // 尝试连接不存在的 PLC
    OPCUAClient bad_client("opc.tcp://192.168.1.200:4840");
    if (!bad_client.connect(3000)) {
        std::cout << "场景 20a: 连接失败已捕获 (预期行为)\n";
    }

    if (!client.connect()) return;

    // 读取不存在的节点
    auto bad_node = OPCUAClient::numericNodeId(0, 99999);
    auto resp = client.readValue(bad_node);
    if (resp.status().code() != 0) {
        std::cout << "场景 20b: 读不存在的节点, 错误码="
                  << resp.status().code() << "\n";
    }

    // 写入只读节点 (假设 1001 是只读的)
    auto read_node = OPCUAClient::numericNodeId(0, 1001);
    bool wrote = client.writeBool(read_node, false);
    if (!wrote) {
        std::cout << "场景 20c: 写入只读节点失败 (预期行为)\n";
    }

    // 类型不匹配读取
    auto float_node = OPCUAClient::numericNodeId(0, 1005);
    resp = client.readValue(float_node);
    if (resp.value().type() == opcua::DataTypeId::FLOAT) {
        std::cout << "场景 20d: 读取 float 节点, 类型="
                  << resp.value().type() << " 值=" << resp.value().float_val() << "\n";
    }

    // 重连机制 (概念)
    if (!client.isConnected()) {
        std::cout << "场景 20e: 尝试重连...\n";
        for (int retry = 0; retry < 3; ++retry) {
            if (client.connect()) {
                std::cout << "  第 " << (retry + 1) << " 次重连成功\n";
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

// ============================================================================
// 场景 21: protobuf 透传 -- 序列化后通过消息队列 / ZeroMQ / gRPC 转发
// ============================================================================
void scenario_proto_wire_transport() {
    std::cout << "\n=== 场景 21: protobuf 序列化转发 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    // 从 PLC 读取数据
    opcua::BatchReadRequest batch;
    for (uint32_t id = 1001; id <= 1010; ++id) {
        auto* r = batch.add_requests();
        r->mutable_node_id()->set_namespace_index(0);
        r->mutable_node_id()->set_numeric_id(id);
    }
    auto batch_resp = client.batchRead(batch);

    // 序列化为 protobuf bytes
    std::string pb_data;
    batch_resp.SerializeToString(&pb_data);
    std::cout << "Serialized batch response: " << pb_data.size() << " bytes\n";

    // 模拟发送到消息队列 (Kafka / RabbitMQ / ZeroMQ)
    // mq_producer.send("plc_data_topic", pb_data);

    // 对端反序列化
    opcua::BatchReadResponse received;
    if (received.ParseFromString(pb_data)) {
        std::cout << "Deserialized " << received.responses_size() << " values\n";
        for (int i = 0; i < received.responses_size(); ++i) {
            std::cout << "  val[" << i << "] type="
                      << received.responses(i).value().type() << "\n";
        }
    }

    // gRPC 示例: service PlcGateway { rpc Read(BatchReadRequest) returns (BatchReadResponse); }
    // 说明: 可以直接将 protobuf 消息用于 gRPC 服务定义
}

// ============================================================================
// 场景 22: 读写 UInt64 / 大整数 (如累计流量计读数)
// ============================================================================
void scenario_large_integer() {
    std::cout << "\n=== 场景 22: 大整数读写 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    auto total_flow = OPCUAClient::numericNodeId(0, 8001);
    auto serial_num = OPCUAClient::numericNodeId(0, 8002);

    // 读取大整数累计值
    uint64_t flow = client.readUInt32(total_flow);  // 若实际是 UInt64, 需要相应修改
    std::cout << "Total flow: " << flow << "\n";

    // 使用 protobuf 写 UInt64
    opcua::WriteRequest wreq;
    *wreq.mutable_node_id() = serial_num;
    wreq.mutable_value()->set_type(opcua::DataTypeId::UINT64);
    wreq.mutable_value()->set_uint64_val(9876543210UL);
    client.write(wreq);
    std::cout << "Serial number written\n";

    // 读取 Int64 (负数兼容)
    int64_t counter = client.readInt64(OPCUAClient::numericNodeId(0, 8003));
    std::cout << "Signed counter: " << counter << "\n";
}

// ============================================================================
// 场景 23: 通过 QualifiedName 浏览并快速读取 (按名称查找变量)
// ============================================================================
void scenario_browse_and_read() {
    std::cout << "\n=== 场景 23: 浏览 + 读取组合 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    auto root = OPCUAClient::numericNodeId(0, 85);

    // 浏览所有变量, 然后逐一读取值
    auto vars = client.browseVariables(root);
    for (const auto& v : vars) {
        auto val = client.readValue(v.node_id());
        if (val.status().code() == 0) {
            std::cout << v.display_name() << " = ";
            if (val.value().has_int32_val())
                std::cout << val.value().int32_val();
            else if (val.value().has_float_val())
                std::cout << val.value().float_val();
            else if (val.value().has_bool_val())
                std::cout << val.value().bool_val();
            else if (val.value().has_string_val())
                std::cout << val.value().string_val();
            std::cout << "\n";
        }
    }
}

// ============================================================================
// 场景 24: 读取 DateTime 类型
// ============================================================================
void scenario_datetime() {
    std::cout << "\n=== 场景 24: DateTime 读写 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    auto dt_node = OPCUAClient::numericNodeId(0, 9001);

    auto resp = client.readValue(dt_node);
    if (resp.value().type() == opcua::DataTypeId::DATETIME) {
        uint64_t dt_raw = resp.value().datetime_val();
        // UA_DateTime 以 100ns 为单位, 从 1601-01-01 开始
        // 转换为 Unix timestamp (秒)
        uint64_t unix_sec = (dt_raw / 10000000) - 11644473600ULL;
        std::cout << "DateTime (raw): " << dt_raw
                  << " (unix): " << unix_sec << "\n";
    }
}

// ============================================================================
// 场景 25: 带诊断信息的 Read (StatusCode 详细解析)
// ============================================================================
void scenario_status_code_detail() {
    std::cout << "\n=== 场景 25: 状态码解析 ===\n";
    OPCUAClient client(PLC_ENDPOINT);
    if (!client.connect()) return;

    auto node = OPCUAClient::numericNodeId(0, 1002);
    auto resp = client.readValue(node);

    uint32_t code = resp.status().code();
    if (code == 0) {
        std::cout << "Good: " << resp.value().int32_val() << "\n";
    } else {
        // 常见错误码解析
        switch (code) {
        case 0x80000000:
            std::cout << "Bad Unexpected Error\n"; break;
        case 0x803C0000:
            std::cout << "Bad NodeId Unknown\n"; break;
        case 0x80410000:
            std::cout << "Bad AttributeId Invalid\n"; break;
        case 0x804B0000:
            std::cout << "Bad User Access Denied\n"; break;
        case 0x805A0000:
            std::cout << "Bad Not Readable\n"; break;
        case 0x805B0000:
            std::cout << "Bad Not Writable\n"; break;
        case 0x80730000:
            std::cout << "Bad Type Mismatch\n"; break;
        case 0x807A0000:
            std::cout << "Bad Timeout\n"; break;
        default:
            std::cout << "Error code: 0x" << std::hex << code << std::dec << "\n";
        }
    }
}

// ============================================================================
// main: 运行所有场景
// ============================================================================
int main() {
    scenario_connect_disconnect();
    scenario_read_scalar_types();
    scenario_read_array_types();
    scenario_read_attributes();
    scenario_write_scalar_types();
    scenario_write_array_types();
    scenario_batch_read();
    scenario_batch_write();
    scenario_browse();
    // scenario_subscription();     // 需等待, 默认注释
    scenario_method_call();
    scenario_history_read();
    scenario_string_nodeid();
    scenario_write_attribute();
    scenario_raw_bytes();
    scenario_structured_types();
    scenario_unified_message();
    scenario_secure_connection();
    scenario_polling_from_config();
    scenario_error_handling();
    scenario_proto_wire_transport();
    scenario_large_integer();
    scenario_browse_and_read();
    scenario_datetime();
    scenario_status_code_detail();

    std::cout << "\n所有场景演示完毕.\n";
    return 0;
}
