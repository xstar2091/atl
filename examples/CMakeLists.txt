project(examples)

add_executable(time_string_example
    ${PROJECT_ROOT_DIR}/examples/utils/time_string_example.cpp
)
target_include_directories(time_string_example PRIVATE ${PROJECT_ROOT_DIR})
target_link_libraries(time_string_example atl)

add_executable(thread_pool_example
    ${PROJECT_ROOT_DIR}/examples/utils/thread_pool_example.cpp
)
target_include_directories(thread_pool_example PRIVATE ${PROJECT_ROOT_DIR})
target_link_libraries(thread_pool_example atl)
