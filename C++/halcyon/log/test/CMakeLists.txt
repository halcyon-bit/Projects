cmake_minimum_required(VERSION 3.10)

# 设定工程名
project(log_test)

set(MAIN_PATH ${PROJECT_SOURCE_DIR}/..)

# 存放静态库的文件夹位置
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${MAIN_PATH}/lib/linux)

# 存放动态库的文件夹位置
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${MAIN_PATH}/bin/linux)

# 存放可执行软件的目录
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MAIN_PATH}/bin/linux)

# 默认存放库文件的位置，如果产生的是静态库并且没有指定 CMAKE_ARCHIVE_OUTPUT_DIRECTORY 则存放在该目录下，动态库也类似
# set(LIBRARY_OUTPUT_PATH ${MAIN_PATH}/)

# 源文件路径
set(SRC_PATH ${MAIN_PATH}/test)

# depends 路径
set(DEPENDS_PATH ${MAIN_PATH}/../depends)

# set(CMAKE_VERBOSE_MAKEFILE on) # for message
# debug release 编译方式
if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
	# 文件生成路径
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MAIN_PATH}/bin/linux_debug)
	# log 模块 lib 路径
	set(LOG_LIB_PATH ${MAIN_PATH}/lib/linux_debug)
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MAIN_PATH}/bin/linux_release)
	set(LOG_LIB_PATH ${MAIN_PATH}/lib/linux_release)
else()
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MAIN_PATH}/bin/linux_release)
	set(LOG_LIB_PATH ${MAIN_PATH}/lib/linux_release)
    # message(${CMAKE_CXX_FLAGS_RELEASE})
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

# 源文件
aux_source_directory(${MAIN_PATH}/src SRC_FILE)

include_directories(${MAIN_PATH}/include)
include_directories(${MAIN_PATH}/src)
include_directories(${DEPENDS_PATH}/halcyon/base/1.0.3)
include_directories(${DEPENDS_PATH}/thirdparty/googletest/1.11.0/include)

# LINK_DIRECTORIES(${MAIN_PATH}/lib/linux)
LINK_DIRECTORIES(${LOG_LIB_PATH})
LINK_DIRECTORIES(${DEPENDS_PATH}/thirdparty/googletest/1.11.0/lib/linux)

# file_opt
add_executable(ut_file_opt ${SRC_PATH}/ut_file_opt.cpp ${MAIN_PATH}/src/file_opt.cpp)
target_link_libraries(ut_file_opt gtest pthread)
