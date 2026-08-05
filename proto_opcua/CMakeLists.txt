cmake_minimum_required(VERSION 3.16)
project(opcua-protobuf-demo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Dependencies ----
find_package(protobuf CONFIG REQUIRED)
find_package(open62541 REQUIRED)
find_package(Threads REQUIRED)

# ---- Library: OPC UA Client Wrapper ----
add_library(opcua_client
    opcua_client.cpp
)

target_include_directories(opcua_client PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${open62541_INCLUDE_DIRS}
)

target_link_libraries(opcua_client PUBLIC
    protobuf::libprotobuf
    ${open62541_LIBRARIES}
)

# ---- Protobuf code generation ----
protobuf_generate(TARGET opcua_client LANGUAGE cpp PROTOS opcua.proto)

# ---- Executable: All Usage Scenarios ----
add_executable(scenarios scenarios.cpp)
target_include_directories(scenarios PRIVATE
    ${open62541_INCLUDE_DIRS}
)
target_link_libraries(scenarios PRIVATE
    opcua_client
    Threads::Threads
)

# ---- Executable: Data Type Demo ----
add_executable(data_type_demo ex/data_type_demo.cpp)
target_include_directories(data_type_demo PRIVATE
    ${open62541_INCLUDE_DIRS}
)
target_link_libraries(data_type_demo PRIVATE
    opcua_client
    Threads::Threads
)
