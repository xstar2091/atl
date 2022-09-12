project(unittest)

add_executable(${PROJECT_NAME}
    utils/time_string_test.cpp
    utils/thread_pool_async_group_test.cpp
    utils/thread_pool_async_task_callable_test.cpp
    utils/thread_pool_test.cpp
    utils/thread_pool2_test.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_ROOT_DIR})
target_link_libraries(${PROJECT_NAME} atl gtest_main gtest pthread)
