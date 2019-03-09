# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (Spider)

# 支持c++11编译
SET(CMAKE_C_COMPILER "/usr/local/bin/gcc")
SET(CMAKE_CXX_COMPILER "/usr/local/bin/g++")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DBOOST_LOG_DYN_LINK -g -Wall -Wno-unused-variable -pthread -DBOOST_LOG_DYN_LINK")
#add_definitions(-std=c++11)

#设置执行文件输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#设置库输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

MESSAGE("++++++++++++++Start Build+++++++++++++++++")

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(./src/. DIR_SRCS)

# 添加文件子目录 
#不可以合并
add_subdirectory(${PROJECT_SOURCE_DIR}/redis)
#add_subdirectory(./src)

# 添加第三方的库
set(LINK_DIR ${PROJECT_SOURCE_DIR}/lib)
set(INC_DIR  ${PROJECT_SOURCE_DIR}/redis ${PROJECT_SOURCE_DIR}/src)

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(hiredis json_linux-gcc-4.9.3_libmt boost_filesystem boost_system boost_thread boost_log boost_log_setup MyRedis MySrc)



# 指定生成目标
add_executable(app ${DIR_SRCS})

# 将第三方的库链接到可执行文件
target_link_libraries(app hiredis json_linux-gcc-4.9.3_libmt.a boost_filesystem  boost_system boost_thread boost_log boost_log_setup MyRedis MySrc)
