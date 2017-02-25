cmake_minimum_required(VERSION 2.8)

project(engine)

# GLFW
add_subdirectory("libs/glfw") # compiling from source

# OpenGL
find_package(OpenGL REQUIRED)

add_executable(myapp "src/test1.cpp")

target_include_directories(myapp SYSTEM PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(myapp ${OPENGL_gl_LIBRARY})
target_link_libraries(myapp glfw)



