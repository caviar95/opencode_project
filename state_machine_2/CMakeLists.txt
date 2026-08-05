cmake_minimum_required(VERSION 3.14)
project(state_machine_2 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(state_machine_2
    main.cpp
    examples/device_control.cpp
)

target_include_directories(state_machine_2 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(state_machine_2 PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
