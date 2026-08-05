add_executable(test_calculator test_calculator.cpp)
target_link_libraries(test_calculator PRIVATE auto_core)

add_test(NAME test_calculator COMMAND test_calculator)
