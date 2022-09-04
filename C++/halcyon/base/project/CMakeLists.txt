cmake_minimum_required(VERSION 3.10)

# 设定工程名
project(hbase)

set(MAIN_PATH ${PROJECT_SOURCE_DIR}/..)

# 默认存放静态库的文件夹位置
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MAIN_PATH}/lib/linux)

# 默认存放动态库的文件夹位置
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MAIN_PATH}/bin/linux)

# 存放可执行软件的目录
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MAIN_PATH}/bin/linux)

# 默认存放库文件的位置，如果产生的是静态库并且没有指定 CMAKE_ARCHIVE_OUTPUT_DIRECTORY 则存放在该目录下，动态库也类似
# set(LIBRARY_OUTPUT_PATH ${MAIN_PATH}/)

# debug release 编译方式
if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MAIN_PATH}/lib/linux_debug)
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MAIN_PATH}/lib/linux_release)
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MAIN_PATH}/lib/linux_release)
endif()

# C++ 标准
if(CMAKE_BUILD_CXX_STANDARD AND (CMAKE_BUILD_CXX_STANDARD STREQUAL "c++11"))
	set(CMAKE_CXX_STANDARD 11)
elseif(CMAKE_BUILD_CXX_STANDARD AND (CMAKE_BUILD_CXX_STANDARD STREQUAL "c++14"))
	set(CMAKE_CXX_STANDARD 14)
else()
	set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED True)

# 源文件路径
set(SRC_PATH ${MAIN_PATH}/src)

# include 路径
set(INCLUDE_PATH ${MAIN_PATH}/include)

# depends 路径
set(DEPENDS_PATH ${MAIN_PATH}/../depends)

# include
include_directories(${INCLUDE_PATH})
include_directories(${SRC_PATH})

# 源文件
aux_source_directory(${SRC_PATH} SRC_FILE)
aux_source_directory(${SRC_PATH}/base64 SRC_FILE)
aux_source_directory(${SRC_PATH}/md5 SRC_FILE)

# 生成静态库
add_library(halcyon_base ${SRC_FILE})
