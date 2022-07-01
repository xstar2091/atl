cmake_minimum_required(VERSION 3.20)
project(atl)

add_compile_options(-g -O0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR})

add_subdirectory(atl)
add_subdirectory(test)
