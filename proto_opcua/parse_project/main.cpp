#include "node_cache.h"
#include "plc_client.h"
#include "plc_poller.h"
#include "struct_parser.h"
#include <chrono>
#include <cstring>
#include <iostream>
#include <open62541/types.h>
#include <sstream>
#include <string>
#include <thread>

static const std::string PLC_URL = "opc.tcp://192.168.1.100:4840";

// ============================================================================
// demo 1: analyze built-in structured types (LocalizedText, QualifiedName)
// ============================================================================
void demo_type_layout() {
  std::cout << "\n=== Demo 1: TypeLayout ===\n";

  const UA_DataType *lt = &UA_TYPES[UA_TYPES_LOCALIZEDTEXT];
  auto layout = StructParser::analyzeType(lt);
  std::cout << "Type: " << layout.name << "  memSize=" << layout.mem_size
            << "  members=" << layout.members.size() << "\n";
  for (auto &m : layout.members) {
    std::cout << "  member: " << m.name << "  type=" << m.type_name
              << "  ft=" << static_cast<int>(m.ft) << "  offset=" << m.offset
              << "  array=" << m.is_array << "\n";
  }

  const UA_DataType *qn = &UA_TYPES[UA_TYPES_QUALIFIEDNAME];
  auto ql = StructParser::analyzeType(qn);
  std::cout << "Type: " << ql.name << "  memSize=" << ql.mem_size
            << "  members=" << ql.members.size() << "\n";
  for (auto &m : ql.members) {
    std::cout << "  member: " << m.name << "  type=" << m.type_name
              << "  ft=" << static_cast<int>(m.ft) << "\n";
  }
}

// ============================================================================
// demo 2: parse UA_LocalizedText in memory
// ============================================================================
void demo_parse_localized_text() {
  std::cout << "\n=== Demo 2: parse UA_LocalizedText ===\n";
  UA_LocalizedText lt;
  lt.locale = UA_STRING_ALLOC("zh_CN");
  lt.text = UA_STRING_ALLOC("TemperatureSensor");
  const UA_DataType *dt = &UA_TYPES[UA_TYPES_LOCALIZEDTEXT];
  ParsedValue pv = StructParser::parseValue(&lt, dt, "DisplayName");
  StructParser::print(pv, std::cout);
  UA_LocalizedText_clear(&lt);
}

// ============================================================================
// demo 3: parse UA_QualifiedName
// ============================================================================
void demo_parse_qualified_name() {
  std::cout << "\n=== Demo 3: parse UA_QualifiedName ===\n";
  UA_QualifiedName qn;
  qn.namespaceIndex = 2;
  qn.name = UA_STRING_ALLOC("MotorSpeed");
  const UA_DataType *dt = &UA_TYPES[UA_TYPES_QUALIFIEDNAME];
  ParsedValue pv = StructParser::parseValue(&qn, dt, "BrowseName");
  StructParser::print(pv, std::cout);
  UA_QualifiedName_clear(&qn);
}

// ============================================================================
// demo 4: manually parse a flat struct (simulate CustomAxis)
// ============================================================================
void demo_parse_custom_struct() {
  std::cout << "\n=== Demo 4: parse struct (simulated CustomAxis) ===\n";

  struct CustomAxis {
    float position;
    float velocity;
    float torque;
    bool enabled;
  };

  // Build the type layout manually since we cannot use UA_DataType_init
  TypeLayout layout;
  layout.name = "CustomAxis";
  layout.mem_size = sizeof(CustomAxis);

  MemberInfo m0;
  m0.name = "position";
  m0.type_name = "Float";
  m0.ft = FieldType::FLOAT;
  m0.offset = offsetof(CustomAxis, position);
  layout.members.push_back(m0);

  MemberInfo m1;
  m1.name = "velocity";
  m1.type_name = "Float";
  m1.ft = FieldType::FLOAT;
  m1.offset = offsetof(CustomAxis, velocity);
  layout.members.push_back(m1);

  MemberInfo m2;
  m2.name = "torque";
  m2.type_name = "Float";
  m2.ft = FieldType::FLOAT;
  m2.offset = offsetof(CustomAxis, torque);
  layout.members.push_back(m2);

  MemberInfo m3;
  m3.name = "enabled";
  m3.type_name = "Boolean";
  m3.ft = FieldType::BOOL;
  m3.offset = offsetof(CustomAxis, enabled);
  layout.members.push_back(m3);

  std::cout << "Layout: " << layout.name << "\n";
  for (auto &m : layout.members)
    std::cout << "  " << m.name << " : " << m.type_name << "\n";

  // Create instance and parse
  CustomAxis axis{123.45f, 67.89f, 0.95f, true};
  ParsedValue pv;
  pv.field_name = "Axis1";
  pv.ft = FieldType::STRUCTURE;

  for (auto &m : layout.members) {
    const void *ptr = reinterpret_cast<const uint8_t *>(&axis) + m.offset;
    ParsedValue sv;
    sv.field_name = m.name;
    sv.ft = m.ft;
    switch (m.ft) {
    case FieldType::FLOAT:
      sv.num.d = *static_cast<const float *>(ptr);
      break;
    case FieldType::DOUBLE:
      sv.num.d = *static_cast<const double *>(ptr);
      break;
    case FieldType::BOOL:
      sv.num.b = *static_cast<const bool *>(ptr);
      break;
    case FieldType::I32:
      sv.num.i64 = *static_cast<const int32_t *>(ptr);
      break;
    case FieldType::U32:
      sv.num.u64 = *static_cast<const uint32_t *>(ptr);
      break;
    default:
      break;
    }
    pv.struct_fields.push_back(std::move(sv));
  }

  StructParser::print(pv, std::cout);
  std::cout << "JSON:\n" << StructParser::toJson(pv) << "\n";
}

// ============================================================================
// demo 5: parse struct with fixed-size array members
// ============================================================================
void demo_parse_struct_with_arrays() {
  std::cout << "\n=== Demo 5: struct with array members ===\n";

  struct MultiAxis {
    float setpoints[3];
    float actual[3];
    int32_t count;
  };

  MultiAxis ma;
  ma.setpoints[0] = 10.0f;
  ma.setpoints[1] = 20.0f;
  ma.setpoints[2] = 30.0f;
  ma.actual[0] = 9.8f;
  ma.actual[1] = 20.1f;
  ma.actual[2] = 29.9f;
  ma.count = 42;

  ParsedValue pv;
  pv.field_name = "MultiAxisNode";
  pv.ft = FieldType::STRUCTURE;

  // setpoints array
  ParsedValue arr1;
  arr1.field_name = "setpoints";
  arr1.is_array = true;
  arr1.ft = FieldType::FLOAT;
  for (int i = 0; i < 3; ++i) {
    ParsedValue elem;
    elem.field_name = "setpoints[" + std::to_string(i) + "]";
    elem.ft = FieldType::FLOAT;
    elem.num.d = ma.setpoints[i];
    arr1.array_elems.push_back(elem);
  }
  pv.struct_fields.push_back(std::move(arr1));

  // actual array
  ParsedValue arr2;
  arr2.field_name = "actual";
  arr2.is_array = true;
  arr2.ft = FieldType::FLOAT;
  for (int i = 0; i < 3; ++i) {
    ParsedValue elem;
    elem.field_name = "actual[" + std::to_string(i) + "]";
    elem.ft = FieldType::FLOAT;
    elem.num.d = ma.actual[i];
    arr2.array_elems.push_back(elem);
  }
  pv.struct_fields.push_back(std::move(arr2));

  // scalar
  ParsedValue cnt;
  cnt.field_name = "count";
  cnt.ft = FieldType::I32;
  cnt.num.i64 = ma.count;
  pv.struct_fields.push_back(std::move(cnt));

  StructParser::print(pv, std::cout);
}

// ============================================================================
// demo 6: NodeCache - thread-safe cache with change callback
// ============================================================================
void demo_node_cache() {
  std::cout << "\n=== Demo 6: NodeCache ===\n";
  NodeCache cache(100);

  cache.setOnChange(
      [](const NodeKey &k, const CachedValue &old_v, const CachedValue &new_v) {
        std::cout << "  [CHANGE] ns=" << k.ns << " id=" << k.id;
        if (old_v.valid())
          std::cout << " old_ver=" << old_v.version;
        else
          std::cout << " (new)";
        std::cout << " -> ver=" << new_v.version << "\n";
      });

  NodeKey k1{1, "TempSensor"};
  CachedValue cv1;
  cv1.parsed.ft = FieldType::FLOAT;
  cv1.parsed.num.d = 25.3;
  cv1.status_code = 0;
  cache.put(k1, std::move(cv1));

  NodeKey k2{1, "Pressure"};
  CachedValue cv2;
  cv2.parsed.ft = FieldType::DOUBLE;
  cv2.parsed.num.d = 1.013;
  cv2.status_code = 0;
  cache.put(k2, std::move(cv2));

  // update k1 to trigger change callback
  CachedValue cv3;
  cv3.parsed.ft = FieldType::FLOAT;
  cv3.parsed.num.d = 26.1;
  cv3.status_code = 0;
  cache.put(k1, std::move(cv3));

  // read back
  CachedValue out;
  if (cache.get(k1, out)) {
    std::cout << "  k1: ft=" << static_cast<int>(out.parsed.ft)
              << " d=" << out.parsed.num.d << " ver=" << out.version << "\n";
  }

  auto stale = cache.getStaleKeys(std::chrono::seconds(0));
  std::cout << "  stale keys (0s): " << stale.size() << "\n";
  cache.dump(std::cout);
}

// ============================================================================
// demo 7: PlcPoller configuration
// ============================================================================
void demo_plc_poller_config() {
  std::cout << "\n=== Demo 7: PlcPoller configuration ===\n";
  PlcClient client(PLC_URL);
  NodeCache cache(1000);
  PlcPoller poller(client, cache);

  PollGroup fast;
  fast.name = "fast_100ms";
  fast.interval = std::chrono::milliseconds(100);
  fast.items = {
      {{0, "1001"}, "MotorSpeed", "Motor RPM"},
      {{0, "1002"}, "Temperature", "Current temp"},
      {{0, "1003"}, "Pressure", "Tank pressure"},
  };

  PollGroup slow;
  slow.name = "slow_1000ms";
  slow.interval = std::chrono::milliseconds(1000);
  slow.items = {
      {{0, "2001"}, "SerialNumber", "Device serial"},
      {{0, "2002"}, "FirmwareVer", "FW version"},
  };

  poller.addGroup(std::move(fast));
  poller.addGroup(std::move(slow));

  poller.setOnPollDone([](const std::string &grp, size_t n) {
    std::cout << "  [poll] group=" << grp << " ok=" << n << "\n";
  });
  poller.setOnError([](const NodeKey &k, uint32_t st) {
    std::cout << "  [error] ns=" << k.ns << " id=" << k.id << " status=0x"
              << std::hex << st << std::dec << "\n";
  });

  std::cout << "  Poller: 2 groups, fast=100ms, slow=1000ms\n";

  poller.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(300));
  poller.stop();
  std::cout << "  Poller started->stopped\n";
}

// ============================================================================
// demo 8: browse PLC address space
// ============================================================================
void demo_browse_and_discover(PlcClient &client) {
  std::cout << "\n=== Demo 8: Browse & Discover ===\n";
  auto vars = client.browseVariables(0, 85);
  std::cout << "Discovered " << vars.size() << " variable nodes:\n";
  for (auto &[ns, id] : vars) {
    auto cv = client.readNode(ns, id);
    std::cout << "  ns=" << ns << " id=" << id
              << " type=" << static_cast<int>(cv.parsed.ft) << "\n";
  }
}

// ============================================================================
// demo 9: read a node and parse
// ============================================================================
void demo_read_node(PlcClient &client) {
  std::cout << "\n=== Demo 9: Read & Parse node ===\n";
  auto cv = client.readNode(0, 1002, "MyVar");
  if (!cv.valid()) {
    std::cout << "  Read failed: status=0x" << std::hex << cv.status_code
              << std::dec << "\n";
    return;
  }
  StructParser::print(cv.parsed, std::cout);
}

// ============================================================================
// demo 10: full round-trip: build parsed value -> cache -> retrieve ->
// serialize
// ============================================================================
void demo_full_roundtrip() {
  std::cout << "\n=== Demo 10: Full round-trip ===\n";
  NodeCache cache(100);

  ParsedValue pv;
  pv.field_name = "J1_Axis";
  pv.ft = FieldType::STRUCTURE;

  ParsedValue pos;
  pos.field_name = "position";
  pos.ft = FieldType::FLOAT;
  pos.num.d = 42.0;
  pv.struct_fields.push_back(std::move(pos));

  ParsedValue vel;
  vel.field_name = "velocity";
  vel.ft = FieldType::FLOAT;
  vel.num.d = 3.14;
  pv.struct_fields.push_back(std::move(vel));

  ParsedValue en;
  en.field_name = "enabled";
  en.ft = FieldType::BOOL;
  en.num.b = true;
  pv.struct_fields.push_back(std::move(en));

  NodeKey key{2, "3001"};
  CachedValue cv;
  cv.parsed = pv;
  cv.status_code = 0;
  cache.put(key, std::move(cv));

  CachedValue out;
  if (cache.get(key, out)) {
    std::cout << "Cached value (ver=" << out.version << "):\n";
    StructParser::print(out.parsed, std::cout);
    std::cout << "JSON:\n" << StructParser::toJson(out.parsed) << "\n";
  }
}

// ============================================================================
// demo 11: deeply nested struct (struct-in-struct)
// ============================================================================
void demo_deep_nested_struct() {
  std::cout << "\n=== Demo 11: Deep nested struct ===\n";

  ParsedValue outer;
  outer.field_name = "OuterData";
  outer.ft = FieldType::STRUCTURE;

  // inner struct
  ParsedValue inner;
  inner.field_name = "sensor";
  inner.ft = FieldType::STRUCTURE;

  ParsedValue temp;
  temp.field_name = "temperature";
  temp.ft = FieldType::FLOAT;
  temp.num.d = 23.5;
  inner.struct_fields.push_back(std::move(temp));

  ParsedValue hum;
  hum.field_name = "humidity";
  hum.ft = FieldType::FLOAT;
  hum.num.d = 65.2;
  inner.struct_fields.push_back(std::move(hum));

  outer.struct_fields.push_back(std::move(inner));

  ParsedValue zone;
  zone.field_name = "zone_id";
  zone.ft = FieldType::I32;
  zone.num.i64 = 7;
  outer.struct_fields.push_back(std::move(zone));

  ParsedValue lbl;
  lbl.field_name = "label";
  lbl.ft = FieldType::STRING;
  lbl.str = "Zone-7-Sensor";
  outer.struct_fields.push_back(std::move(lbl));

  StructParser::print(outer, std::cout);
  std::cout << "JSON:\n" << StructParser::toJson(outer) << "\n";
}

// ============================================================================
// main
// ============================================================================
int main() {
  std::cout << "============================================\n";
  std::cout << "  PLC Node Parser - Comprehensive Demo\n";
  std::cout << "============================================\n";

  demo_type_layout();
  demo_parse_localized_text();
  demo_parse_qualified_name();
  demo_parse_custom_struct();
  demo_parse_struct_with_arrays();
  demo_node_cache();
  demo_full_roundtrip();
  demo_deep_nested_struct();

  PlcClient client(PLC_URL);
  if (client.connect(5000)) {
    std::cout << "\nPLC connected\n";
    demo_browse_and_discover(client);
    demo_read_node(client);
  } else {
    std::cout << "\nPLC offline - skipping online demos\n";
  }

  demo_plc_poller_config();

  std::cout << "\n============================================\n";
  std::cout << "  All demos complete.\n";
  std::cout << "============================================\n";
  return 0;
}
