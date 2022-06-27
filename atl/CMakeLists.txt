
add_library(${PROJECT_NAME} STATIC
    ${PROJECT_ROOT_DIR}/atl/utils/time_string.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_ROOT_DIR}
)
