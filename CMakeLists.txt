cmake_minimum_required(VERSION 3.22.1)
project(EasyCompiler)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_DIR src)
set(INCLUDE_DIR include)
set(LIB_DIR lib)

# 原文件和头文件
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/*.h")

# 第三方库
file(GLOB_RECURSE LIB_SOURCES "${LIB_DIR}/*.cpp")
file(GLOB_RECURSE LIB_HEADERS "${LIB_DIR}/*.h")

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${LIB_SOURCES} ${LIB_HEADERS})

# 添加头文件搜索路径
include_directories(${INCLUDE_DIR})