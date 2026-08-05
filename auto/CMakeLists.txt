cmake_minimum_required(VERSION 3.20)

project(auto_pipeline VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(AUTO_BUILD_TESTS "Build unit tests" ON)
option(AUTO_ENABLE_SANITIZERS "Enable ASan/UBSan" OFF)
option(AUTO_BUILD_EXAMPLES "Build example binaries" ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(AUTO_ENABLE_SANITIZERS AND NOT MSVC)
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

add_library(auto_core
  src/calculator.cpp
)
target_include_directories(auto_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_features(auto_core PUBLIC cxx_std_17)

if(AUTO_BUILD_EXAMPLES)
  add_executable(auto_demo src/main.cpp)
  target_link_libraries(auto_demo PRIVATE auto_core)
endif()

if(AUTO_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

include(GNUInstallDirs)
install(TARGETS auto_core
  EXPORT auto_coreTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT auto_coreTargets
  NAMESPACE auto::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/auto_core
)
