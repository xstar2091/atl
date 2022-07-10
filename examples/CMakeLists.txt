project(examples)

add_executable(time_string_example
    ${PROJECT_ROOT_DIR}/examples/utils/time_string_example.cpp
)
target_include_directories(time_string_test PRIVATE ${PROJECT_ROOT_DIR})
target_link_libraries(time_string_example atl)
