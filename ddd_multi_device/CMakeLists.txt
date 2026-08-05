cmake_minimum_required(VERSION 3.16)
project(device_ddd CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(device_ddd
    src/main.cpp
    src/ddd/domain/process/node.cpp
    src/ddd/domain/module/device_module.cpp
)

target_include_directories(device_ddd PRIVATE src)