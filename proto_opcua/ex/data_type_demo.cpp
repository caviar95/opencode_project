// ============================================================================
// OPC UA 数据类型应用详解 + 示例程序
// ============================================================================
// OPC UA 类型系统分为三层:
//   1. Built-in 类型 (25 种基础类型)
//   2. 预定义结构化类型 (如 LocalizedText, QualifiedName, NodeId)
//   3. 应用自定义类型 (通过 DataTypeDefinition 定义)
//
// 本例展示如何在实际工业控制场景中使用这些类型,
// 通过 protobuf 封装实现与 open62541 之间的双向转换。
// ============================================================================

#include "opcua_client.h"
#include <open62541/types.h>
#include <open62541/types_generated.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cassert>

// ============================================================================
// 第一部分: Built-in 类型概览
//
// 下表列了 OPC UA 规范定义的 25 种内置数据类型:
//
//   ID | 类型           | C/C++ 映射       | 说明
//  ─────────────────────────────────────────────────────────
//   1  | Boolean        | bool             | 逻辑值
//   2  | SByte          | int8_t           | 有符号 8-bit
//   3  | Byte           | uint8_t          | 无符号 8-bit
//   4  | Int16          | int16_t          | 有符号 16-bit
//   5  | UInt16         | uint16_t         | 无符号 16-bit
//   6  | Int32          | int32_t          | 有符号 32-bit
//   7  | UInt32         | uint32_t         | 无符号 32-bit
//   8  | Int64          | int64_t          | 有符号 64-bit
//   9  | UInt64         | uint64_t         | 无符号 64-bit
//   10 | Float          | float            | 单精度浮点 (IEEE 754)
//   11 | Double         | double           | 双精度浮点 (IEEE 754)
//   12 | String         | UA_String        | UTF-8 字符串
//   13 | DateTime       | UA_DateTime      | 100ns 间隔, 始于 1601-01-01
//   14 | Guid           | UA_Guid          | 16 字节 UUID
//   15 | ByteString     | UA_ByteString    | 原始字节序列
//   16 | XmlElement     | UA_XmlElement    | XML 片段
//   17 | NodeId         | UA_NodeId        | 节点标识符
//   18 | ExpandedNodeId | UA_ExpandedNodeId| 带 namespace URI 的 NodeId
//   19 | StatusCode     | UA_StatusCode    | 状态码
//   20 | QualifiedName  | UA_QualifiedName | 带 namespace 索引的字符串
//   21 | LocalizedText  | UA_LocalizedText | 带 locale 的语言字符串
//   22 | ExtensionObject| UA_ExtensionObject| 扩展对象 (任意类型包装)
//   23 | DataValue      | UA_DataValue     | 带品质/时间戳的值包装
//   24 | Variant        | UA_Variant       | 动态类型容器 (支持任意类型+数组)
//   25 | DiagnosticInfo | UA_DiagnosticInfo | 诊断信息
//
// 这些类型是构建地址空间节点属性的基础。
// ============================================================================

static const std::string PLC_ENDPOINT = "opc.tcp://192.168.1.100:4840";

// ============================================================================
// 1. 标量类型 (Scalar) 读写示例
// ============================================================================
void demo_builtin_scalar_types() {
    std::cout << "\n========== Built-in 标量类型 ==========\n";

    // 每个节点在 OPC UA 服务器中有一个 DataType 属性,
    // 客户端在读写时必须使用匹配的数据类型。
    //
    // NodeId 的构建方式:
    //   - 数字型:  ns=0;i=1001   (namespace=0, numeric_id=1001)
    //   - 字符串型: ns=3;s="MyVar" (namespace=3, string_id="MyVar")
    //
    // 应用场景:
    //   PLC 变量通常映射为以下类型:

    auto bool_node    = OPCUAClient::numericNodeId(2, 100);  // 设备开关状态
    auto int32_node   = OPCUAClient::numericNodeId(2, 101);  // 产品计数
    auto uint32_node  = OPCUAClient::numericNodeId(2, 102);  // 累计运行时间
    auto float_node   = OPCUAClient::numericNodeId(2, 103);  // 温度测量值
    auto double_node  = OPCUAClient::numericNodeId(2, 104);  // 高精度压力
    auto int64_node   = OPCUAClient::numericNodeId(2, 105);  // 总产量累计
    auto uint64_node  = OPCUAClient::numericNodeId(2, 106);  // 传感器序列号
    auto int16_node   = OPCUAClient::numericNodeId(2, 107);  // 通道选择器
    auto string_node  = OPCUAClient::numericNodeId(2, 108);  // 批次号/产品码

    // 读取示例 (实际运行时需要连接 PLC)
    // OPCUAClient client(PLC_ENDPOINT);
    // if (!client.connect()) return;
    //
    // bool   sw   = client.readBool(bool_node);
    // int32_t cnt = client.readInt32(int32_node);
    // uint32_t rt = client.readUInt32(uint32_node);
    // float  temp = client.readFloat(float_node);
    // double pres = client.readDouble(double_node);
    // int64_t total   = client.readInt64(int64_node);
    // uint64_t serial = client.readUInt64(uint64_node);  // 注意: readUInt64 需在客户端实现
    // std::string batch = client.readString(string_node);

    std::cout << "标量类型: Bool, Int8/16/32/64, UInt8/16/32/64, Float, Double, String\n";
    std::cout << "          每个类型有对应的 protobuf field 用于序列化\n";

    // 写入示例
    // client.writeBool(bool_node, true);        // 启动设备
    // client.writeInt32(int32_node, 1024);       // 设置计数
    // client.writeFloat(float_node, 25.5f);      // 设置温度
    // client.writeString(string_node, "BATCH001"); // 写入批次号
}

// ============================================================================
// 2. 数组类型 (Array) 读写示例
// ============================================================================
void demo_array_types() {
    std::cout << "\n========== 数组类型 ==========\n";
    // 应用场景:
    //   - 波形数据: 连续采样的 ADC 值 (Int32[] / Float[])
    //   - 坐标点集: 机器人路径点 (Double[])
    //   - 配方参数: 批量工艺参数 (Float[])
    //   - 状态字:   PLC 状态字数组 (Bool[] / UInt16[])
    //
    // OPC UA 使用同一个 Variant 表示标量和数组,
    // 通过 arrayLength 字段区分 (0=标量, >0=数组)。

    auto wave_node    = OPCUAClient::numericNodeId(2, 200);  // 波形采样点 Float[]
    auto coords_node  = OPCUAClient::numericNodeId(2, 201);  // 坐标点 Double[]
    auto flags_node   = OPCUAClient::numericNodeId(2, 202);  // 状态标志 Bool[]
    auto labels_node  = OPCUAClient::numericNodeId(2, 203);  // 数组标签 String[]

    std::cout << "数组类型: 通过 repeated field 在 protobuf 中传输\n";
    std::cout << "          Float[] → float_array, Double[] → double_array,\n";
    std::cout << "          Int32[] → int32_array,  Bool[]  → bool_array\n";

    // 读取示例:
    // auto wave = client.readFloatArray(wave_node);
    // 写入示例:
    // client.writeFloatArray(wave_node, {0.0f, 1.0f, 2.0f, 3.0f, 4.0f});

    // 在 protobuf 内部的区分:
    //   opcua::DataTypeId::FLOAT       = 9   (标量 Float)
    //   opcua::DataTypeId::ARRAY_FLOAT = 18  (数组 Float)
    //   variantToProto() 根据 UA_Variant.arrayLength 自动区分
}

// ============================================================================
// 3. DateTime 类型 — 时间戳读写
// ============================================================================
void demo_datetime_type() {
    std::cout << "\n========== DateTime ==========\n";
    // OPC UA 的 DateTime:
    //   - 单位: 100 纳秒间隔 (100ns = 1e-7 s)
    //   - 纪元: 1601-01-01 00:00:00 UTC (Windows FILETIME)
    //   - 转换为 Unix timestamp: unix_sec = (ua_dt / 10_000_000) - 11644473600
    //   - 范围: 可表示约 30,000 年
    //
    // 应用场景:
    //   - 事件时间戳 (报警触发时间)
    //   - 历史数据查询的时间范围
    //   - 批次记录时间

    UA_DateTime now = UA_DateTime_now();
    UA_DateTimeStruct dts = UA_DateTime_toStruct(now);

    std::cout << "当前 UA_DateTime: " << now << "\n";
    std::cout << "分解时间: " << dts.year << "-" << (int)dts.month << "-" << dts.day
              << " " << dts.hour << ":" << dts.min << ":" << dts.sec << "\n";

    // protobuf 中使用 uint64 传输
    uint64_t proto_dt = static_cast<uint64_t>(now);
    // Unix 转换:
    uint64_t unix_sec = (proto_dt / 10000000) - 11644473600ULL;
    std::cout << "Unix timestamp: " << unix_sec << "\n";

    std::cout << "protobuf field: datetime_val (uint64)\n";

    // 读取示例:
    // auto resp = client.readValue(dt_node);
    // uint64_t dt = resp.value().datetime_val();
}

// ============================================================================
// 4. LocalizedText — 多语言描述
// ============================================================================
void demo_localized_text() {
    std::cout << "\n========== LocalizedText ==========\n";
    // 结构体定义:
    //   struct UA_LocalizedText { UA_String locale; UA_String text; };
    //
    // locale = "en_US", "zh_CN", "de_DE" 等
    //
    // 应用场景:
    //   - 设备 DisplayName: {"zh_CN","温度传感器"}, {"en_US","Temperature Sensor"}
    //   - 报警描述: {"en_US","Overheat warning"}, {"zh_CN","过热警告"}
    //   - 菜单标签: 多语言 HMI 界面

    UA_LocalizedText lt;
    UA_LocalizedText_init(&lt);
    lt.locale = UA_STRING_STATIC("zh_CN");
    lt.text   = UA_STRING_STATIC("电机转速");

    std::cout << "LocalizedText: locale=\"" << lt.locale.data
              << "\" text=\"" << lt.text.data << "\"\n";

    UA_LocalizedText_clear(&lt);

    // protobuf 编码策略: 使用字符串 "locale:text"
    // 例如: "zh_CN:电机转速", "en_US:Motor Speed"
    // VariantValue.string_val() 承载
    std::cout << "protobuf: LOCALIZEDTEXT=21, string_val=\"locale:text\"\n";
}

// ============================================================================
// 5. QualifiedName — 带命名空间的限定名
// ============================================================================
void demo_qualified_name() {
    std::cout << "\n========== QualifiedName ==========\n";
    // 结构体定义:
    //   struct UA_QualifiedName { uint16_t namespaceIndex; UA_String name; };
    //
    // 类似 XML 的 namespace:name 概念
    //
    // 应用场景:
    //   - BrowseName 标识符: {ns=2,name="Temperature"}
    //   - 方法输入参数名称
    //   - DataType 定义中的字段名

    UA_QualifiedName qn;
    UA_QualifiedName_init(&qn);
    qn.namespaceIndex = 2;
    qn.name = UA_STRING_STATIC("MotorCurrent");

    std::cout << "QualifiedName: ns=" << qn.namespaceIndex
              << " name=\"" << qn.name.data << "\"\n";

    UA_QualifiedName_clear(&qn);

    // protobuf: 专有 message QualifiedName { uint32 namespace_index; string name; }
    // 也可编码为字符串 "ns:name", 例如 "2:MotorCurrent"
    std::cout << "protobuf: QUALIFIEDNAME=22, 专用 message 或 string \"ns:name\"\n";
}

// ============================================================================
// 6. NodeId — 节点标识符的四种形式
// ============================================================================
void demo_node_id_types() {
    std::cout << "\n========== NodeId 四种形式 ==========\n";
    // NodeId 是 OPC UA 中最核心的标识符类型, 有四种变体:
    //
    // 1. 数字型 (Numeric):    ns=0;i=85         — 标准类型, 效率最高
    // 2. 字符串型 (String):   ns=3;s="MyVar"    — PLC 常用, 可读性好
    // 3. GUID 型 (Guid):      ns=1;g=...        — 全局唯一标识
    // 4. 不透明型 (Opaque):   ns=1;b=...        — 自定义二进制

    // protobuf 定义:
    // message NodeId {
    //     uint32 namespace_index = 1;
    //     oneof id {
    //         uint32 numeric_id = 2;    // i=xxxx
    //         string string_id = 3;     // s="xxxx"
    //         bytes  guid_id   = 4;     // g=...
    //         uint64 opaque_id = 5;     // b=...
    //     }
    // }

    auto numeric_nid = OPCUAClient::numericNodeId(0, 85);
    auto string_nid  = OPCUAClient::stringNodeId(3, "::Program:Main.Temperature");

    std::cout << "数字型 NodeId: ns=" << numeric_nid.namespace_index()
              << " id=" << numeric_nid.numeric_id() << "\n";
    std::cout << "字符串型 NodeId: ns=" << string_nid.namespace_index()
              << " id=\"" << string_nid.string_id() << "\"\n";

    // 数字型用于: 内置类型、标准对象
    // 字符串型用于: PLC 符号寻址 (西门子 S7-1500, Beckhoff, Codesys)
}

// ============================================================================
// 7. Variant — OPC UA 的动态类型系统
// ============================================================================
void demo_variant_type() {
    std::cout << "\n========== Variant ==========\n";
    // Variant 是 OPC UA 的"任意类型"容器, 可以携带:
    //   - 任意 Built-in 类型 (scalar)
    //   - 任意 Built-in 类型的数组
    //   - ExtensionObject (自定义结构的包装)
    //
    // 应用场景:
    //   - Read/Write 服务返回任意类型的值
    //   - Subscribe 收到的 DataChange 通知
    //   - Method 的输入参数和输出参数
    //
    // protobuf 中对应的 VariantValue message:
    //   message VariantValue {
    //       DataTypeId type = 1;
    //       oneof value { ... }           // 标量
    //       repeated ... *_array = ...    // 数组
    //   }
    //
    // 类型映射表:
    //   OPC UA Type     → DataTypeId         → protobuf field
    //   ─────────────────────────────────────────────────────
    //   Boolean         → BOOL              → bool_val
    //   Int32           → INT32             → int32_val
    //   UInt32          → UINT32            → uint32_val
    //   Float           → FLOAT             → float_val
    //   Double          → DOUBLE            → double_val
    //   String          → STRING            → string_val
    //   DateTime        → DATETIME          → datetime_val
    //   ByteString      → BYTESTRING        → bytes_val
    //   Int32[]         → ARRAY_INT32       → int32_array[]
    //   Float[]         → ARRAY_FLOAT       → float_array[]

    std::cout << "Variant 是 OPC UA 序列化的核心, 本项目的 protoToVariant /\n";
    std::cout << "variantToProto 负责 open62541 ↔ protobuf 的双向转换\n";
}

// ============================================================================
// 8. 枚举类型在 protobuf 中的表示
// ============================================================================
void demo_enum_types() {
    std::cout << "\n========== 枚举类型 ==========\n";
    // OPC UA 规范中有大量枚举类型:
    //
    //   NodeClass:      Object=1, Variable=2, Method=4, ObjectType=8, VariableType=16
    //   AttributeId:    Value=13, DisplayName=14, Description=15, NodeClass=2, ...
    //   DataTypeId:     上述 DataTypeId 本质上是枚举
    //
    // 应用场景:
    //   - Browse 时按 NodeClass 过滤节点
    //   - Read/Write 时指定 AttributeId
    //
    // protobuf 定义:
    //   enum DataTypeId {
    //       BOOL=0, INT8=1, ..., STRUCTURED=23
    //   }
    //
    // 在 proto 中枚举的好处:
    //   1. 编译期类型检查
    //   2. 自动序列化/反序列化
    //   3. 跨语言一致

    std::cout << "protobuf 枚举示例:\n";
    std::cout << "  DataTypeId::INT32 = " << opcua::DataTypeId::INT32 << "\n";
    std::cout << "  DataTypeId::FLOAT = " << opcua::DataTypeId::FLOAT << "\n";
    std::cout << "  DataTypeId::STRING = " << opcua::DataTypeId::STRING << "\n";

    // 实际应用: 按类型分发处理
    opcua::VariantValue val;
    val.set_type(opcua::DataTypeId::FLOAT);
    val.set_float_val(25.5f);

    switch (val.type()) {
    case opcua::DataTypeId::BOOL:
        std::cout << "处理 BOOL: " << val.bool_val() << "\n"; break;
    case opcua::DataTypeId::INT32:
        std::cout << "处理 INT32: " << val.int32_val() << "\n"; break;
    case opcua::DataTypeId::FLOAT:
        std::cout << "处理 FLOAT: " << val.float_val() << "\n"; break;
    case opcua::DataTypeId::STRING:
        std::cout << "处理 STRING: " << val.string_val() << "\n"; break;
    default:
        std::cout << "未知类型: " << val.type() << "\n"; break;
    }
}

// ============================================================================
// 9. ExtensionObject — 任意结构化数据的包装
// ============================================================================
void demo_extension_object() {
    std::cout << "\n========== ExtensionObject ==========\n";
    // ExtensionObject 用于传输自定义结构化数据.
    // 内部包含:
    //   - TypeId (NodeId, 指向数据类型定义)
    //   - 编码方式: Binary / XML / JSON
    //   - body: 编码后的字节流
    //
    // 应用场景:
    //   - 调用方法时的复杂的输入/输出参数
    //   - HistoryRead 的过滤器参数 (ReadRawModifiedDetails)
    //   - 事件通知的字段

    // protobuf 中可通过 BYTESTRING 传输序列化的 ExtensionObject:
    //   opcua::VariantValue val;
    //   val.set_type(opcua::DataTypeId::BYTESTRING);
    //   val.set_bytes_val(binary_data);
    //
    // 复杂结构可以定义独立的 protobuf message,
    // 然后通过 bytes 字段嵌入 VariantValue, 实现嵌套扩展.

    std::cout << "ExtensionObject 通过 BYTESTRING 或自定义 proto message 传输\n";
}

// ============================================================================
// 10. StatusCode — 操作结果状态码
// ============================================================================
void demo_status_code() {
    std::cout << "\n========== StatusCode ==========\n";
    // StatusCode 是 uint32, 高 16 位表示主要错误码, 低 16 位表示子码.
    //
    // 关键 StatusCode:
    //   0x00000000  Good
    //   0x803C0000  BadNodeIdUnknown
    //   0x80410000  BadAttributeIdInvalid
    //   0x804B0000  BadUserAccessDenied
    //   0x805A0000  BadNotReadable
    //   0x805B0000  BadNotWritable
    //   0x80730000  BadTypeMismatch
    //   0x807A0000  BadTimeout
    //
    // 应用场景:
    //   - 判断 Read/Write/Browse 是否成功
    //   - Subscribe 的 DataChange 数据品质
    //   - Method 调用结果

    std::cout << "StatusCode 在 protobuf 中:\n";
    std::cout << "  message StatusCode { uint32 code; string symbolic_name; string description; }\n";

    // 检查操作结果
    opcua::StatusCode sc;
    sc.set_code(0x803C0000);
    sc.set_symbolic_name("BadNodeIdUnknown");

    if (sc.code() == 0) {
        std::cout << "操作成功\n";
    } else {
        std::cout << "操作失败: " << sc.symbolic_name()
                  << " (0x" << std::hex << sc.code() << std::dec << ")\n";
    }
}

// ============================================================================
// 11. 数据品质 (DataValue 的 quality/status)
// ============================================================================
void demo_data_quality() {
    std::cout << "\n========== 数据品质 ==========\n";
    // DataValue 包含:
    //   - Value:       实际数据 (Variant)
    //   - Status:      数据品质 (StatusCode)
    //   - SourceTimestamp: 源时间戳 (数据产生时间)
    //   - ServerTimestamp: 服务端时间戳 (数据到达服务端时间)
    //
    // 品质等级:
    //   Good (0x00)              — 数据有效
    //   Uncertain (0x40)        — 数据可用但品质不确定
    //   Bad (0x80)              — 数据无效
    //   BadSensorFailure        — 传感器故障
    //   BadNotConnected         — 通信断开
    //
    // 应用场景:
    //   - 数据采集时先检查品质, 避免使用无效数据
    //   - SCADA 系统的数据质量告警

    // protobuf 中 ReadResponse 携带时间戳:
    //   message ReadResponse {
    //       StatusCode status = 1;
    //       VariantValue value = 2;
    //       uint64 server_timestamp = 3;
    //       uint64 source_timestamp = 4;
    //   }

    std::cout << "ReadResponse protobuf 包含 status + value + 双时间戳\n";
    std::cout << "品质判断: status.code() == 0 → Good\n";
}

// ============================================================================
// 12. 类型转换: 从 open62541 UA_Variant 到 protobuf VariantValue
// ============================================================================
void demo_type_conversion() {
    std::cout << "\n========== UA_Variant ↔ protobuf VariantValue ==========\n";

    // open62541 → protobuf (variantToProto)
    UA_Variant ua_var;
    UA_Variant_init(&ua_var);

    UA_Int32 scalar_val = 42;
    UA_Variant_setScalarCopy(&ua_var, &scalar_val, &UA_TYPES[UA_TYPES_INT32]);
    std::cout << "UA_Variant type: " << UA_TYPES[UA_TYPES_INT32].typeName
              << " isArray: " << (ua_var.arrayLength > 0) << "\n";
    UA_Variant_clear(&ua_var);

    // Array case
    UA_Float arr[4] = {1.1f, 2.2f, 3.3f, 4.4f};
    UA_Variant_setArrayCopy(&ua_var, arr, 4, &UA_TYPES[UA_TYPES_FLOAT]);
    std::cout << "UA_Variant Float[4]: typeName="
              << UA_TYPES[ua_var.type->typeKind].typeName
              << " length=" << ua_var.arrayLength << "\n";
    UA_Variant_clear(&ua_var);

    // String case
    UA_String ua_str = UA_STRING_ALLOC("Hello OPC UA");
    UA_Variant_setScalarCopy(&ua_var, &ua_str, &UA_TYPES[UA_TYPES_STRING]);
    UA_String_clear(&ua_str);
    std::cout << "UA_Variant String: \""
              << ((UA_String*)ua_var.data)->data << "\"\n";
    UA_Variant_clear(&ua_var);

    // protobuf VariantValue → open62541 (protoToVariant) 是反向过程
    std::cout << "\n双向转换流程:\n";
    std::cout << "  1. UA_Variant → opcua::VariantValue:  variantToProto()\n";
    std::cout << "  2. opcua::VariantValue → UA_Variant:  protoToVariant()\n";
    std::cout << "转换在 opcua_client.cpp 中完整实现.\n";
}

// ============================================================================
// 13. 综合场景: 温度监控系统的数据处理
// ============================================================================
void demo_integrated_scenario() {
    std::cout << "\n========== 综合应用场景: 温度监控系统 ==========\n";
    // 这是一个工业温度监控系统的数据类型应用全景

    // ---- 13a. 数据类型映射 ----
    std::cout << "\n-- 13a. 温度监控系统的数据类型映射 --\n";
    struct TempChannel {
        uint32_t ns;
        uint32_t id;
        opcua::DataTypeId dtype;
        const char* unit;
        const char* description;
    };

    TempChannel channels[] = {
        {2, 100, opcua::DataTypeId::FLOAT,  "°C",  "当前温度值 (Float)"},
        {2, 101, opcua::DataTypeId::FLOAT,  "°C",  "温度设定值 (Float)"},
        {2, 102, opcua::DataTypeId::BOOL,   "",    "加热器启停 (Bool)"},
        {2, 103, opcua::DataTypeId::FLOAT,  "°C/s","升温速率 (Float)"},
        {2, 104, opcua::DataTypeId::INT32,  "pcs", "超温报警计数 (Int32)"},
        {2, 105, opcua::DataTypeId::DOUBLE, "bar", "腔体压力 (Double)"},
        {2, 106, opcua::DataTypeId::INT64,  "ms",  "累计运行时长 (Int64)"},
        {2, 107, opcua::DataTypeId::STRING, "",    "当前配方名 (String)"},
        {2, 200, opcua::DataTypeId::ARRAY_FLOAT, "°C", "温度曲线数组 (Float[])"},
        {2, 201, opcua::DataTypeId::DATETIME, "",   "最后校准时间 (DateTime)"},
    };

    for (const auto& ch : channels) {
        std::cout << "  ns=" << ch.ns << ";i=" << ch.id
                  << " [" << ch.dtype << "] "
                  << ch.description;
        if (ch.unit[0]) std::cout << " unit=" << ch.unit;
        std::cout << "\n";
    }

    // ---- 13b. 读取并分类处理 ----
    std::cout << "\n-- 13b. 按类型分发处理 --\n";
    auto process_value = [](opcua::DataTypeId type, const opcua::VariantValue& val) {
        switch (type) {
        case opcua::DataTypeId::FLOAT:
            std::cout << "  Float: " << val.float_val();
            if (val.float_val() > 100.0f)
                std::cout << " [超温告警!]";
            break;
        case opcua::DataTypeId::BOOL:
            std::cout << "  Bool: " << (val.bool_val() ? "ON" : "OFF");
            break;
        case opcua::DataTypeId::INT32:
            std::cout << "  Int32: " << val.int32_val();
            break;
        case opcua::DataTypeId::STRING:
            std::cout << "  String: " << val.string_val();
            break;
        case opcua::DataTypeId::ARRAY_FLOAT: {
            std::cout << "  Float[" << val.float_array_size() << "]:";
            for (int i = 0; i < val.float_array_size(); ++i)
                std::cout << " " << val.float_array(i);
            break;
        }
        default:
            std::cout << "  type=" << type;
        }
        std::cout << "\n";
    };

    // 模拟一批读取结果
    opcua::BatchReadResponse batch_resp;
    for (int i = 0; i < 5; ++i) {
        auto* resp = batch_resp.add_responses();
        resp->mutable_status()->set_code(0);
        auto* val = resp->mutable_value();
        switch (i) {
        case 0: val->set_type(opcua::DataTypeId::FLOAT);  val->set_float_val(85.5f);  break;
        case 1: val->set_type(opcua::DataTypeId::FLOAT);  val->set_float_val(95.0f);  break;
        case 2: val->set_type(opcua::DataTypeId::BOOL);   val->set_bool_val(true);    break;
        case 3: val->set_type(opcua::DataTypeId::INT32);  val->set_int32_val(3);      break;
        case 4: val->set_type(opcua::DataTypeId::STRING); val->set_string_val("Recipe_01"); break;
        }
    }

    for (int i = 0; i < batch_resp.responses_size(); ++i) {
        const auto& r = batch_resp.responses(i);
        if (r.status().code() == 0) {
            std::cout << "  Ch[" << i << "]:";
            process_value(r.value().type(), r.value());
        }
    }

    // ---- 13c. 序列化用于日志/传输 ----
    std::cout << "\n-- 13c. protobuf 序列化到日志/消息队列 --\n";
    std::string wire;
    batch_resp.SerializeToString(&wire);
    std::cout << "  Batch response serialized: " << wire.size() << " bytes\n";

    // 反序列化验证
    opcua::BatchReadResponse parsed;
    parsed.ParseFromString(wire);
    std::cout << "  Deserialized " << parsed.responses_size() << " values\n";
}

// ============================================================================
// 14. 自定义结构化类型的扩展方式
// ============================================================================
void demo_custom_structured_type() {
    std::cout << "\n========== 自定义结构化类型 ==========\n";
    // OPC UA 允许服务端定义自己的数据类型 (通过 DataType 节点).
    //
    // 方式 1: 在 protobuf 中定义专属 message, 使用 BYTESTRING 封装
    //   message AnalogSignal {
    //       float  value   = 1;
    //       float  range_min = 2;
    //       float  range_max = 3;
    //       string unit    = 4;
    //       uint64 timestamp = 5;
    //   }
    //
    //   opcua::VariantValue val;
    //   val.set_type(opcua::DataTypeId::BYTESTRING);
    //   AnalogSignal sig; sig.SerializeToString(&tmp);
    //   val.set_bytes_val(tmp);
    //
    // 方式 2: 使用 VariantValue 的结构化字段 (如果扩展)
    //   可以在 VariantValue 的 oneof 中增加 message 字段
    //
    // 方式 3: 使用 ExtensionObject + 自定义编码
    //   复杂场景, 需要 TypeId 和编码器注册

    std::cout << "自定义类型扩展方案:\n";
    std::cout << "  1. BYTESTRING + 嵌套 proto message (轻量, 推荐)\n";
    std::cout << "  2. VariantValue 增加结构化字段 (修改 proto)\n";
    std::cout << "  3. ExtensionObject + UA_DataType 注册 (完整 OPC UA 兼容)\n";
}

// ============================================================================
// 15. 数据处理备注
// ============================================================================
void demo_type_safety_notes() {
    std::cout << "\n========== 类型安全与最佳实践 ==========\n";
    std::cout << "1. 读写时必须确保 DataTypeId 与服务器节点类型匹配\n";
    std::cout << "2. 数组和标量使用同一个 VariantValue, 根据 type 字段区分\n";
    std::cout << "3. 收到请求先检查 status.code() == 0, 再使用 value\n";
    std::cout << "4. Proto 序列化用于跨语言/跨进程传输 (C++/Python/Java/C#)\n";
    std::cout << "5. open62541 内置所有 25 种 Built-in 类型的序列化/反序列化\n";
    std::cout << "6. Variant 的嵌套使用: Variant 可以包含另一个 Variant (嵌套)\n";
    std::cout << "7. 类型不匹配时服务器返回 BadTypeMismatch (0x80730000)\n";
}

// ============================================================================
int main() {
    std::cout << "=========================================\n";
    std::cout << "OPC UA 数据类型应用详解\n";
    std::cout << "=========================================\n";

    std::cout << "\nOPC UA 数据类型分层结构:\n";
    std::cout << "  ┌─────────────────────────┐\n";
    std::cout << "  │  Application Types      │  ← 自定义结构体\n";
    std::cout << "  │  (custom structs)       │\n";
    std::cout << "  ├─────────────────────────┤\n";
    std::cout << "  │  Predefined Types       │  ← NodeId, QualifiedName, ...\n";
    std::cout << "  │  (built-in structured)  │\n";
    std::cout << "  ├─────────────────────────┤\n";
    std::cout << "  │  Built-in Types (25)    │  ← Boolean, Int32, Float, ...\n";
    std::cout << "  │  (primitive types)      │\n";
    std::cout << "  └─────────────────────────┘\n";

    demo_builtin_scalar_types();
    demo_array_types();
    demo_datetime_type();
    demo_localized_text();
    demo_qualified_name();
    demo_node_id_types();
    demo_variant_type();
    demo_enum_types();
    demo_extension_object();
    demo_status_code();
    demo_data_quality();
    demo_type_conversion();
    demo_integrated_scenario();
    demo_custom_structured_type();
    demo_type_safety_notes();

    std::cout << "\n=========================================\n";
    std::cout << "总结:\n";
    std::cout << "  OPC UA 数据类型体系由 25 种 Built-in 类型为基础,\n";
    std::cout << "  通过 Variant 实现动态类型, 通过 ExtensionObject 扩展自定义类型.\n";
    std::cout << "  Protobuf 封装 (opcua.proto) 将所有类型映射为统一的\n";
    std::cout << "  VariantValue message, 实现高效的跨语言序列化.\n";
    std::cout << "=========================================\n";
    return 0;
}
