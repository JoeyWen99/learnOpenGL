cmake_minimum_required(VERSION 3.20)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 14)


#我们先把 glad 加进我们的项目

#包含 glad 头文件
include_directories(./glad/include)
#包含 glfw 头文件，「提供这个库在电脑中的位置」
include_directories(/opt/homebrew/Cellar/glfw/3.3.6/include)

#将 glfw 的库文件也加进来
link_directories(/opt/homebrew/Cellar/glfw/3.3.6/lib)

#这是我们生成的项目
add_executable(LearnOpenGL ./glad/src/glad.c main.cpp)

#最后把库文件链接进入项目
target_link_libraries(LearnOpenGL glfw)