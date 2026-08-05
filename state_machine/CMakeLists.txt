cmake_minimum_required(VERSION 3.20)
project(hfsm
    VERSION 1.0.0
    DESCRIPTION "Industrial-Grade Hierarchical Finite State Machine Framework"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion -Wno-c++98-compat")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion")
endif()

# Optional: address sanitizer
option(HFSM_ENABLE_ASAN "Enable Address Sanitizer" OFF)
if(HFSM_ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer")
endif()

option(HFSM_BUILD_TESTS "Build unit tests" ON)
option(HFSM_BUILD_EXAMPLES "Build examples" ON)

# Header-only interface library
add_library(hfsm INTERFACE)
target_include_directories(hfsm INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(hfsm INTERFACE cxx_std_17)

if(HFSM_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(HFSM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
