cmake_minimum_required(VERSION 3.22)
project(ddd_order_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra)

# 核心库：领域 + 应用 + 基础设施
add_library(order_core
  src/common/result.h
  src/common/domain_error.h
  src/domain/money.h
  src/domain/money.cpp
  src/domain/order_id.h
  src/domain/order_status.h
  src/domain/order_item.h
  src/domain/order.h
  src/domain/order_repository.h
  src/domain/order_pricing_service.h
  src/application/order_dto.h
  src/application/order_application_service.h
  src/application/order_application_service.cpp
  src/infrastructure/in_memory_order_repository.h
)
target_include_directories(order_core PUBLIC src)

# 终端演示
add_executable(order_demo src/presentation/main.cpp)
target_link_libraries(order_demo PRIVATE order_core)

# 单元测试
enable_testing()
add_executable(order_domain_test tests/order_domain_test.cpp)
target_link_libraries(order_domain_test PRIVATE order_core)
add_test(NAME order_domain_test COMMAND order_domain_test)