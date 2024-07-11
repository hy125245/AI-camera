#cmake最低版本
cmake_minimum_required (VERSION 3.16.3)


#项目名
project (AI-camera)

# 添加多个 头文件 的搜索路径
include_directories(include)

#寻找连接库
find_library(LIB_PATH msc HINTS ${PROJECT_SOURCE_DIR}/libs)

# 将当前文件夹下的源代码，收集到变量src_path
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_PATH)
aux_source_directory(${PROJECT_SOURCE_DIR}/include INCLUDE_PATH)
set(SRC_PATH ./main.cpp)



# 可执行文件放在bin目录侠
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#目标生成
add_executable(${PROJECT_NAME} ${SRC_PATH} ${INCLUDE_PATH})

#动态库连接
target_link_libraries(${PROJECT_NAME} ${LIB_PATH})