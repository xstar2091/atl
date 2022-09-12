
add_library(${PROJECT_NAME} STATIC
    ${PROJECT_ROOT_DIR}/atl/utils/thread_pool.cpp
    ${PROJECT_ROOT_DIR}/atl/utils/thread_pool2.cpp
    ${PROJECT_ROOT_DIR}/atl/utils/time_string.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_ROOT_DIR}
)

target_link_libraries(${PROJECT_NAME} pthread)
