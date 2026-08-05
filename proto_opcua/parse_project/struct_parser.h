#pragma once

#include <open62541/types.h>
#include <open62541/types_generated.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <ostream>
#include <cstdint>

struct UA_Client;

enum class FieldType {
    BOOL, I8, U8, I16, U16, I32, U32, I64, U64,
    FLOAT, DOUBLE, STRING, DATETIME, GUID, BYTESTRING,
    NODEID, QN, LT, STATUSCODE,
    STRUCTURE, ENUM, ARRAY, UNKNOWN
};

struct MemberInfo {
    std::string name;
    std::string type_name;
    FieldType ft{FieldType::UNKNOWN};
    bool is_array{false};
    bool is_dyn_array{false};
    size_t fixed_len{0};
    size_t offset{0};
    const UA_DataType* ua_type{nullptr};
};

struct TypeLayout {
    std::string name;
    size_t mem_size{0};
    std::vector<MemberInfo> members;
    std::unordered_map<int, std::string> enum_map;
};

struct ParsedValue {
    std::string field_name;
    FieldType ft{FieldType::UNKNOWN};
    bool is_array{false};
    union { bool b; int64_t i64; uint64_t u64; double d; } num{};
    std::string str;
    std::vector<ParsedValue> struct_fields;
    std::vector<ParsedValue> array_elems;
};

class StructParser {
public:
    static FieldType typeKindToFieldType(int kind, const UA_DataType* dt = nullptr);
    static TypeLayout analyzeType(const UA_DataType* dt);
    static TypeLayout walkTypes(UA_Client* client, const UA_NodeId& type_node);
    static ParsedValue parseValue(const void* data, const UA_DataType* dt,
                                  const std::string& field_name = "");
    static void print(const ParsedValue& v, std::ostream& os, int depth = 0);
    static std::string toJson(const ParsedValue& v, int indent = 0);
    static ParsedValue parseScalar(const void* data, const UA_DataType* dt,
                                   const std::string& field_name = "");
};
