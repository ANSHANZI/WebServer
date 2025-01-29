# CMake 最低版本要求
cmake_minimum_required(VERSION 3.15)

# 项目名称与支持语言
project(WebServer LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置源代码目录
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${SRC_DIR}/include)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)

# 构建主库
file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp)
add_library(WebServerLib ${SRC_FILES})

# 为主库添加头文件路径
target_include_directories(WebServerLib PRIVATE ${INCLUDE_DIR})

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 构建测试目标
file(GLOB_RECURSE TEST_FILES ${TEST_DIR}/*.cpp)
foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_include_directories(${TEST_NAME} PRIVATE ${INCLUDE_DIR})
    target_link_libraries(${TEST_NAME} PRIVATE WebServerLib)
endforeach()
