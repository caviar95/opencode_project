add_executable(test_core test_core.cpp)
target_link_libraries(test_core hfsm)

add_executable(test_hfsm test_hfsm.cpp)
target_link_libraries(test_hfsm hfsm)

add_test(NAME test_core COMMAND test_core)
add_test(NAME test_hfsm COMMAND test_hfsm)
