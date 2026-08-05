#include "struct_parser.h"
#include <open62541/client_highlevel.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <sstream>

FieldType StructParser::typeKindToFieldType(int kind, const UA_DataType* dt) {
    switch (kind) {
    case UA_TYPES_BOOLEAN:      return FieldType::BOOL;
    case UA_TYPES_SBYTE:        return FieldType::I8;
    case UA_TYPES_BYTE:         return FieldType::U8;
    case UA_TYPES_INT16:        return FieldType::I16;
    case UA_TYPES_UINT16:       return FieldType::U16;
    case UA_TYPES_INT32:        return FieldType::I32;
    case UA_TYPES_UINT32:       return FieldType::U32;
    case UA_TYPES_INT64:        return FieldType::I64;
    case UA_TYPES_UINT64:       return FieldType::U64;
    case UA_TYPES_FLOAT:        return FieldType::FLOAT;
    case UA_TYPES_DOUBLE:       return FieldType::DOUBLE;
    case UA_TYPES_STRING:       return FieldType::STRING;
    case UA_TYPES_DATETIME:     return FieldType::DATETIME;
    case UA_TYPES_GUID:         return FieldType::GUID;
    case UA_TYPES_BYTESTRING:   return FieldType::BYTESTRING;
    case UA_TYPES_NODEID:       return FieldType::NODEID;
    case UA_TYPES_QUALIFIEDNAME:return FieldType::QN;
    case UA_TYPES_LOCALIZEDTEXT:return FieldType::LT;
    case UA_TYPES_STATUSCODE:   return FieldType::STATUSCODE;
    default: break;
    }
    if (dt && dt->typeKind == UA_DATATYPEKIND_ENUM)
        return FieldType::ENUM;
    if (dt && dt->typeKind == UA_DATATYPEKIND_STRUCTURE)
        return FieldType::STRUCTURE;
    return FieldType::UNKNOWN;
}

TypeLayout StructParser::analyzeType(const UA_DataType* dt) {
    TypeLayout layout;
    if (!dt) return layout;
    layout.name = dt->typeName ? dt->typeName : "";
    layout.mem_size = dt->memSize;
    const UA_DataType* current = dt;
    size_t offset = 0;
    for (size_t i = 0; i < dt->membersSize; ++i) {
        auto& um = dt->members[i];
        MemberInfo m;
        m.name = um.memberName ? um.memberName : "";
        const UA_DataType* mtype = um.memberType;
        m.type_name = mtype && mtype->typeName ? mtype->typeName : "?";
        m.ua_type = mtype;
        m.offset = um.padding;
        m.is_array = um.isArray;
        m.is_dyn_array = (um.isArray);
        m.fixed_len = 0;
        if (mtype) {
            m.ft = typeKindToFieldType(mtype->typeKind, mtype);
            if (m.ft == FieldType::STRUCTURE || m.ft == FieldType::UNKNOWN) {
                if (mtype->typeKind == UA_DATATYPEKIND_STRUCTURE)
                    m.ft = FieldType::STRUCTURE;
                else if (mtype->typeKind == UA_DATATYPEKIND_ENUM)
                    m.ft = FieldType::ENUM;
            }
        }
        layout.members.push_back(m);
        offset = um.padding + (mtype ? mtype->memSize : 0);
    }
    return layout;
}

TypeLayout StructParser::walkTypes(UA_Client* client, const UA_NodeId& type_node) {
    UA_Variant v;
    UA_Variant_init(&v);
    UA_StatusCode st = UA_Client_readValueAttribute(client, type_node, &v);
    if (st != UA_STATUSCODE_GOOD || !v.type) {
        TypeLayout empty;
        return empty;
    }
    const UA_DataType* dt = static_cast<const UA_DataType*>(v.type);
    TypeLayout result = analyzeType(dt);
    UA_Variant_clear(&v);
    return result;
}

ParsedValue StructParser::parseValue(const void* data, const UA_DataType* dt,
                                     const std::string& field_name) {
    ParsedValue pv;
    pv.field_name = field_name;
    if (!data || !dt) return pv;

    pv.ft = typeKindToFieldType(dt->typeKind, dt);

    if (pv.ft == FieldType::STRUCTURE) {
        auto layout = analyzeType(dt);
        size_t base = reinterpret_cast<size_t>(data);
        for (auto& m : layout.members) {
            const void* mptr = reinterpret_cast<const uint8_t*>(base) + m.offset;
            if (m.is_array) {
                const void* arr_ptr = *reinterpret_cast<const void* const*>(mptr);
                size_t count = m.fixed_len;
                if (m.is_dyn_array) {
                    count = *reinterpret_cast<const size_t*>(
                        reinterpret_cast<const uint8_t*>(mptr) + sizeof(void*));
                }
                if (arr_ptr && count > 0 && m.ua_type) {
                    ParsedValue av;
                    av.field_name = m.name;
                    av.is_array = true;
                    av.ft = m.ft;
                    for (size_t i = 0; i < count; ++i) {
                        const void* elem = static_cast<const uint8_t*>(arr_ptr) + i * m.ua_type->memSize;
                        av.array_elems.push_back(parseValue(elem, m.ua_type, m.name + "[" + std::to_string(i) + "]"));
                    }
                    pv.struct_fields.push_back(std::move(av));
                }
            } else if (m.ft == FieldType::STRUCTURE && m.ua_type) {
                pv.struct_fields.push_back(parseValue(mptr, m.ua_type, m.name));
            } else if (m.ft == FieldType::STRUCTURE && m.ua_type &&
                       m.ua_type->typeKind == UA_DATATYPEKIND_OPTSTRUCT) {
                pv.struct_fields.push_back(parseValue(mptr, m.ua_type, m.name));
            } else {
                ParsedValue sv;
                sv.field_name = m.name;
                sv.ft = m.ft;
                sv.is_array = false;
                if (m.ua_type) {
                    const void* scalar_ptr = mptr;
                    if (m.is_array) scalar_ptr = *reinterpret_cast<const void* const*>(mptr);
                    auto parsed = parseScalar(scalar_ptr, m.ua_type, m.name);
                    pv.struct_fields.push_back(std::move(parsed));
                }
            }
        }
        return pv;
    }

    if (pv.ft == FieldType::ENUM) {
        pv.num.i64 = 0;
        const uint8_t* raw = static_cast<const uint8_t*>(data);
        switch (dt->memSize) {
        case 1: pv.num.i64 = *raw; break;
        case 2: pv.num.i64 = *reinterpret_cast<const int16_t*>(raw); break;
        case 4: pv.num.i64 = *reinterpret_cast<const int32_t*>(raw); break;
        default: break;
        }
        return pv;
    }

    return parseScalar(data, dt, field_name);
}

static ParsedValue parseScalarImpl(const void* data, const UA_DataType* dt,
                                   const std::string& field_name) {
    ParsedValue pv;
    pv.field_name = field_name;
    pv.ft = StructParser::typeKindToFieldType(dt->typeKind, dt);
    switch (dt->typeKind) {
    case UA_TYPES_BOOLEAN:
        pv.num.b = *static_cast<const UA_Boolean*>(data); break;
    case UA_TYPES_SBYTE:
        pv.num.i64 = *static_cast<const UA_SByte*>(data); break;
    case UA_TYPES_BYTE:
        pv.num.u64 = *static_cast<const UA_Byte*>(data); break;
    case UA_TYPES_INT16:
        pv.num.i64 = *static_cast<const UA_Int16*>(data); break;
    case UA_TYPES_UINT16:
        pv.num.u64 = *static_cast<const UA_UInt16*>(data); break;
    case UA_TYPES_INT32:
        pv.num.i64 = *static_cast<const UA_Int32*>(data); break;
    case UA_TYPES_UINT32:
        pv.num.u64 = *static_cast<const UA_UInt32*>(data); break;
    case UA_TYPES_INT64:
        pv.num.i64 = *static_cast<const UA_Int64*>(data); break;
    case UA_TYPES_UINT64:
        pv.num.u64 = *static_cast<const UA_UInt64*>(data); break;
    case UA_TYPES_FLOAT:
        pv.num.d = *static_cast<const UA_Float*>(data); break;
    case UA_TYPES_DOUBLE:
        pv.num.d = *static_cast<const UA_Double*>(data); break;
    case UA_TYPES_STRING: {
        auto* s = static_cast<const UA_String*>(data);
        if (s->length > 0 && s->data)
            pv.str.assign(reinterpret_cast<const char*>(s->data), s->length);
        break;
    }
    case UA_TYPES_DATETIME: {
        pv.num.u64 = *static_cast<const UA_DateTime*>(data);
        break;
    }
    default:
        break;
    }
    return pv;
}

ParsedValue StructParser::parseScalar(const void* data, const UA_DataType* dt,
                                      const std::string& field_name) {
    return parseScalarImpl(data, dt, field_name);
}

void StructParser::print(const ParsedValue& v, std::ostream& os, int depth) {
    std::string pad(depth * 2, ' ');
    os << pad;
    if (!v.field_name.empty()) os << v.field_name << " = ";

    if (v.is_array) {
        os << "[Array " << v.array_elems.size() << "]\n";
        for (auto& e : v.array_elems) print(e, os, depth + 1);
        return;
    }
    if (!v.struct_fields.empty()) {
        os << "{\n";
        for (auto& f : v.struct_fields) print(f, os, depth + 1);
        os << pad << "}\n";
        return;
    }
    switch (v.ft) {
    case FieldType::BOOL: os << (v.num.b ? "true" : "false"); break;
    case FieldType::I8: case FieldType::I16: case FieldType::I32: case FieldType::I64:
        os << v.num.i64; break;
    case FieldType::U8: case FieldType::U16: case FieldType::U32: case FieldType::U64:
        os << v.num.u64; break;
    case FieldType::FLOAT: case FieldType::DOUBLE:
        os << v.num.d; break;
    case FieldType::STRING: case FieldType::DATETIME:
        os << v.str; break;
    default:
        os << "(type=" << static_cast<int>(v.ft) << ")";
    }
    os << "\n";
}

std::string StructParser::toJson(const ParsedValue& v, int indent) {
    std::ostringstream oss;
    std::string pad(indent, ' ');
    if (!v.field_name.empty()) oss << pad << "\"" << v.field_name << "\": ";

    if (v.is_array) {
        oss << "[\\n";
        for (size_t i = 0; i < v.array_elems.size(); ++i) {
            oss << toJson(v.array_elems[i], indent + 2);
            if (i + 1 < v.array_elems.size()) oss << ",";
            oss << "\\n";
        }
        oss << pad << "]";
        return oss.str();
    }
    if (!v.struct_fields.empty()) {
        oss << "{\\n";
        for (size_t i = 0; i < v.struct_fields.size(); ++i) {
            oss << toJson(v.struct_fields[i], indent + 2);
            if (i + 1 < v.struct_fields.size()) oss << ",";
            oss << "\\n";
        }
        oss << pad << "}";
        return oss.str();
    }
    switch (v.ft) {
    case FieldType::BOOL: oss << (v.num.b ? "true" : "false"); break;
    case FieldType::I8: case FieldType::I16: case FieldType::I32: case FieldType::I64:
        oss << v.num.i64; break;
    case FieldType::U8: case FieldType::U16: case FieldType::U32: case FieldType::U64:
        oss << v.num.u64; break;
    case FieldType::FLOAT: case FieldType::DOUBLE:
        oss << v.num.d; break;
    default:
        oss << "\"" << v.str << "\"";
    }
    return oss.str();
}
