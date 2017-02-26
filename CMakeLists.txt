cmake_minimum_required(VERSION 2.8)
project(tator)

##########################
# LIBRARIES
##########################
add_subdirectory("libs/glfw") # GLFW from source
	set(GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/include)
	set(GLEW_LIBRARY_DEBUG 
		${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/lib/Release/Win32/glew32s.lib
		)
	set(GLEW_LIBRARY_RELEASE
		${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/lib/Release/Win32/glew32s.lib
		)
find_package(GLEW REQUIRED) # GLEW
find_package(OpenGL REQUIRED) # OpenGL

##########################
# Tator variables
##########################
set(TATOR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TATOR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(TATOR_GRAPHICS_INCLUDE_DIR ${TATOR_INCLUDE_DIR}/graphics)

##########################
# Sources and Headers
##########################

# Graphics module
set(TATOR_GRAPHICS_SOURCE
	${TATOR_SOURCE_DIR}/graphics/shader.cpp
	)
source_group("graphics" FILES ${TATOR_GRAPHICS_SOURCE})

set(TATOR_GRAPHICS_HEADERS
	${TATOR_INCLUDE_DIR}/graphics/shader.hpp
	)
source_group("graphics" FILES ${TATOR_GRAPHICS_HEADER})
	
# All modules
set(TATOR_SOURCES
	${TATOR_GRAPHICS_SOURCE}
	)
set(TATOR_HEADERS
	${TATOR_GRAPHICS_HEADERS}
	)

##########################
# TARGETS
##########################
set(MYAPP_MAIN ${TATOR_SOURCE_DIR}/test1.cpp)
add_executable(myapp ${MYAPP_MAIN} ${TATOR_SOURCES})
target_include_directories(myapp SYSTEM PUBLIC
	${TATOR_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR} 
	${GLEW_INCLUDE_DIRS}
	glfw
)

target_link_libraries(myapp ${OPENGL_LIBRARIES})
target_link_libraries(myapp ${GLEW_LIBRARIES})
target_link_libraries(myapp glfw)



