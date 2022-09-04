cmake_minimum_required(VERSION 3.10)

# 设定工程名
project(base_test)

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
	# base 模块 lib 路径
	set(BASE_LIB_PATH ${MAIN_PATH}/lib/linux_debug)
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MAIN_PATH}/bin/linux_release)
	set(BASE_LIB_PATH ${MAIN_PATH}/lib/linux_release)
else()
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${MAIN_PATH}/bin/linux_release)
	set(BASE_LIB_PATH ${MAIN_PATH}/lib/linux_release)
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

include_directories(${MAIN_PATH}/include)
include_directories(${DEPENDS_PATH}/thirdparty/googletest/1.11.0/include)

# LINK_DIRECTORIES(${MAIN_PATH}/lib/linux)
LINK_DIRECTORIES(${BASE_LIB_PATH})
LINK_DIRECTORIES(${DEPENDS_PATH}/thirdparty/googletest/1.11.0/lib/linux)

# any
add_executable(ut_any ${SRC_PATH}/ut_any.cc)
target_link_libraries(ut_any gtest pthread)

# base64
add_executable(ut_base64 ${SRC_PATH}/ut_base64.cc)
target_link_libraries(ut_base64 gtest halcyon_base pthread)

# count_down_latch
add_executable(ut_count_down_latch ${SRC_PATH}/ut_count_down_latch.cc)
target_link_libraries(ut_count_down_latch pthread)

# defer
add_executable(ut_defer ${SRC_PATH}/ut_defer.cc)

# elapsed_timer
add_executable(ut_elapsed_timer ${SRC_PATH}/ut_elapsed_timer.cc)

# function
add_executable(ut_function ${SRC_PATH}/ut_function.cc)
target_link_libraries(ut_function gtest pthread)

# md5
add_executable(ut_md5 ${SRC_PATH}/ut_md5.cc)
target_link_libraries(ut_md5 gtest halcyon_base pthread)

# range
add_executable(ut_range ${SRC_PATH}/ut_range.cc)
target_link_libraries(ut_range)

# singleton
add_executable(ut_singleton ${SRC_PATH}/ut_singleton.cc)
target_link_libraries(ut_singleton pthread)

# string_view
add_executable(ut_string_view ${SRC_PATH}/ut_string_view.cc)
target_link_libraries(ut_string_view gtest pthread)

# weak_callback
add_executable(ut_weak_callback ${SRC_PATH}/ut_weak_callback.cc)
target_link_libraries(ut_weak_callback gtest pthread)
