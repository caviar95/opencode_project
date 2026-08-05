cmake_minimum_required(VERSION 3.16)
project(plc-node-parser CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(protobuf CONFIG REQUIRED)
find_package(open62541 REQUIRED)
find_package(Threads REQUIRED)

set(PB_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../build)

add_library(opcua_proto STATIC ${PB_SRC_DIR}/opcua.pb.cc)
target_include_directories(opcua_proto PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${PB_SRC_DIR}
    ${open62541_INCLUDE_DIRS}
)
target_link_libraries(opcua_proto PUBLIC protobuf::libprotobuf)

add_library(plc_parser STATIC
    struct_parser.cpp node_cache.cpp plc_poller.cpp plc_client.cpp
)
target_include_directories(plc_parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${PB_SRC_DIR}
    ${open62541_INCLUDE_DIRS}
)
target_link_libraries(plc_parser PUBLIC opcua_proto open62541::open62541 Threads::Threads)

add_executable(plc_demo main.cpp)
target_link_libraries(plc_demo PRIVATE plc_parser open62541::open62541 Threads::Threads)
