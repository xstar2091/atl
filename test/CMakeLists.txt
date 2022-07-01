project(test)

#include_directories(${PROJECT_ROOT_DIR})
add_executable(time_string_test utils/time_string_test.cpp)
target_include_directories(time_string_test PRIVATE ${PROJECT_ROOT_DIR})
target_link_libraries(time_string_test atl gtest_main gtest)
