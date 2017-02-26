cmake_minimum_required(VERSION 2.8)
project(tator)

##########################
# LIBRARIES
##########################
add_subdirectory("libs/glfw") # GLFW from source

find_package(GLEW REQUIRED) # GLEW
find_package(OpenGL REQUIRED) # OpenGL

##########################
# TARGETS
##########################
add_executable(myapp "src/test1.cpp")
target_include_directories(myapp SYSTEM PUBLIC 
	${OPENGL_INCLUDE_DIR} 
	${GLEW_INCLUDE_DIRS}
	glfw
)

message("GLEW: ${GLEW_INCLUDE_DIRS}")
message("GLEW: ${GLEW_LIBRARIES}")
message("OPENGL: ${OPENGL_LIBRARIES}")

target_link_libraries(myapp ${OPENGL_LIBRARIES})
target_link_libraries(myapp ${GLEW_LIBRARIES})
target_link_libraries(myapp glfw)
#target_link_libraries(myapp glfw)
#target_link_libraries(myapp ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} glfw)
#target_link_libraries(myapp glfw)



