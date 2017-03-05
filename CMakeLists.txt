cmake_minimum_required(VERSION 2.8)
project(tator)

##########################
# SETTINGS
##########################
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

message(STATUS "OUTPUT: ${CMAKE_CURRENT_BINARY_DIR}")

##########################
# LIBRARIES
##########################
# ---- GLFW ----
set(TATOR_DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps)
add_subdirectory(${TATOR_DEPS_DIR}/glfw) # GLFW from source
# ---- GLEW ----
#set(GLEW_INCLUDE_DIR ${TATOR_DEPS_DIR}/glew/include)
#set(GLEW_LIBRARY_DEBUG ${TATOR_DEPS_DIR}/glew/lib/Release/Win32/glew32s.lib)
#set(GLEW_LIBRARY_RELEASE ${TATOR_DEPS_DIR}/glew/lib/Release/Win32/glew32s.lib)
find_package(GLEW REQUIRED) # GLEW
set(TATOR_GLEW_SHARED_LIB "" CACHE FILEPATH "Glew shared library")
message(STATUS "TATOR GLEW LIB: ${TATOR_GLEW_SHARED_LIB}")
#message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")
#message(STATUS "GLEW libraries: ${GLEW_LIBRARIES}")
# ---- SOIL ----
add_subdirectory(${TATOR_DEPS_DIR}/soil)
# ---- GLM ----
add_subdirectory(${TATOR_DEPS_DIR}/glm)
# ---- OpenGL ----
find_package(OpenGL REQUIRED) # OpenGL

##########################
# Tator variables
##########################
set(TATOR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TATOR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(_TATOR_INCLUDE_DIR ${TATOR_INCLUDE_DIR}/tator) # used internally so we don't have to prefix tator to every path
set(TATOR_GRAPHICS_INCLUDE_DIR ${_TATOR_INCLUDE_DIR}/graphics)
set(TATOR_SYSTEM_INCLUDE_DIR ${_TATOR_INCLUDE_DIR}/system)

##########################
# Sources and Headers
##########################

# ---- Graphics module ----
set(TATOR_GRAPHICS_SOURCE
	${TATOR_SOURCE_DIR}/graphics/gl.cpp
	${TATOR_SOURCE_DIR}/graphics/Shader.cpp
	${TATOR_SOURCE_DIR}/graphics/ShaderProgram.cpp
	${TATOR_SOURCE_DIR}/graphics/Camera.cpp
	)
set(TATOR_GRAPHICS_HEADERS
	${_TATOR_INCLUDE_DIR}/graphics/gl.hpp
	${_TATOR_INCLUDE_DIR}/graphics/Shader.hpp
	${_TATOR_INCLUDE_DIR}/graphics/ShaderProgram.hpp
	${_TATOR_INCLUDE_DIR}/graphics/Camera.hpp
	)
source_group("graphics" FILES ${TATOR_GRAPHICS_HEADERS} ${TATOR_GRAPHICS_SOURCE})
	
# ---- System module ----
set(TATOR_SYSTEM_SOURCE ""
	)
set(TATOR_SYSTEM_HEADERS
	${_TATOR_INCLUDE_DIR}/system/TatorException.hpp
	)
source_group("system" FILES ${TATOR_SYSTEM_HEADERS} ${TATOR_SYSTEM_SOURCE})
	
# ---- All modules ----
set(TATOR_SOURCES
	${TATOR_GRAPHICS_SOURCE}
	${TATOR_SYSTEM_SOURCE}
	)
set(TATOR_HEADERS
	${TATOR_GRAPHICS_HEADERS}
	${TATOR_SYSTEM_HEADERS}
	)

##########################
# TARGETS
##########################

# Engine
add_library(tator SHARED ${TATOR_SOURCES} ${TATOR_HEADERS})
target_include_directories(tator SYSTEM PUBLIC
	${TATOR_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
	glm
	glfw
	)
target_link_libraries(tator
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	glm
	glfw
	)

add_subdirectory(examples)

# Testing/Development project
#set(EXAMPLE1_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples/example1)
#set(EXAMPLE1_SOURCES
#	${EXAMPLE1_DIR}/main.cpp
#)
#add_executable(example1 ${EXAMPLE1_SOURCES})
#target_include_directories(example1 SYSTEM PUBLIC
#	tator
#)
#target_link_libraries(example1
#	tator
#)

#set(MYAPP_MAIN ${TATOR_SOURCE_DIR}/test1.cpp)
#add_executable(myapp ${MYAPP_MAIN} ${TATOR_SOURCES} ${TATOR_HEADERS})
#target_include_directories(myapp SYSTEM PUBLIC
#	${TATOR_INCLUDE_DIR}
#	${OPENGL_INCLUDE_DIR} 
#	${GLEW_INCLUDE_DIRS}
#	glm
#	glfw
#)
#target_link_libraries(myapp
#	${OPENGL_LIBRARIES}
#	${GLEW_LIBRARIES}
#	glm
#	glfw
#)


