cmake_minimum_required(VERSION 3.14)
project(cancelable_pipeline CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Threads REQUIRED)

add_library(stage INTERFACE)
target_include_directories(stage INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(stage INTERFACE Threads::Threads)

add_executable(demo src/main.cpp)
target_link_libraries(demo PRIVATE stage)
