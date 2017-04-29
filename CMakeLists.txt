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
# ---- pybind11 ----
add_subdirectory(${TATOR_DEPS_DIR}/pybind11)
find_package(PythonLibs REQUIRED) # python for pybind11
# ---- OpenGL ----
find_package(OpenGL REQUIRED) # OpenGL
# ---- Lua ----
set(LUA_LIBS ${TATOR_DEPS_DIR}/lua/lua53.lib)
include_directories(${TATOR_DEPS_DIR}/lua/include)

# ---- Assimp ----
#add_subdirectory(${TATOR_DEPS_DIR}/assimp)
#add_subdirectory(${TATOR_DEPS_DIR}/cgal)

##########################
# Tator variables
##########################
set(TATOR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TATOR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(_TATOR_INCLUDE_DIR ${TATOR_INCLUDE_DIR}/tator) # used internally so we don't have to prefix tator to every path
set(TATOR_GRAPHICS_INCLUDE_DIR ${_TATOR_INCLUDE_DIR}/graphics)
set(TATOR_SYSTEM_INCLUDE_DIR ${_TATOR_INCLUDE_DIR}/system)
set(TATOR_SCRIPTING_INCLUDE_DIR ${_TATOR_INCLUDE_DIR}/scripting)

##########################
# Sources and Headers
##########################

# ---- Graphics module ----
set(TATOR_GRAPHICS_SOURCE
	${TATOR_SOURCE_DIR}/graphics/gl.cpp
	${TATOR_SOURCE_DIR}/graphics/OpenGLRenderer.cpp
	${TATOR_SOURCE_DIR}/graphics/Shader.cpp
	${TATOR_SOURCE_DIR}/graphics/ShaderProgram.cpp
	${TATOR_SOURCE_DIR}/graphics/Camera.cpp
	${TATOR_SOURCE_DIR}/graphics/Mesh.cpp
	${TATOR_SOURCE_DIR}/graphics/Renderable.cpp
	${TATOR_SOURCE_DIR}/graphics/StandardMesh.cpp
	)
set(TATOR_GRAPHICS_HEADERS
	${_TATOR_INCLUDE_DIR}/graphics/gl.hpp
	${_TATOR_INCLUDE_DIR}/graphics/Renderer.hpp
	${_TATOR_INCLUDE_DIR}/graphics/OpenGLRenderer.hpp
	${_TATOR_INCLUDE_DIR}/graphics/Shader.hpp
	${_TATOR_INCLUDE_DIR}/graphics/ShaderProgram.hpp
	${_TATOR_INCLUDE_DIR}/graphics/Camera.hpp
	${_TATOR_INCLUDE_DIR}/graphics/Mesh.hpp
	${_TATOR_INCLUDE_DIR}/graphics/Renderable.hpp
	${_TATOR_INCLUDE_DIR}/graphics/StandardMesh.hpp
	)
source_group("graphics" FILES ${TATOR_GRAPHICS_HEADERS} ${TATOR_GRAPHICS_SOURCE})
	
# ---- System module ----
set(TATOR_SYSTEM_SOURCE
	${TATOR_SOURCE_DIR}/system/Core.cpp
	${TATOR_SOURCE_DIR}/system/Window.cpp
	)
set(TATOR_SYSTEM_HEADERS
	${_TATOR_INCLUDE_DIR}/system/TatorException.hpp
	${_TATOR_INCLUDE_DIR}/system/Core.hpp
	${_TATOR_INCLUDE_DIR}/system/Window.hpp
	)
source_group("system" FILES ${TATOR_SYSTEM_HEADERS} ${TATOR_SYSTEM_SOURCE})
	
# ---- Scripting module ----
set(TATOR_SCRIPTING_SOURCE
	${TATOR_SOURCE_DIR}/scripting/PythonEngine.cpp
	)
set(TATOR_SCRIPTING_HEADERS
	${_TATOR_INCLUDE_DIR}/scripting/ScriptEngine.hpp
	${_TATOR_INCLUDE_DIR}/scripting/PythonEngine.hpp
	)
source_group("scripting" FILES ${TATOR_SCRIPTING_HEADERS} ${TATOR_SCRIPTING_SOURCE})
	
# ---- All modules ----
set(TATOR_SOURCES
	${TATOR_GRAPHICS_SOURCE}
	${TATOR_SYSTEM_SOURCE}
	${TATOR_SCRIPTING_SOURCE}
	)
set(TATOR_HEADERS
	${TATOR_GRAPHICS_HEADERS}
	${TATOR_SYSTEM_HEADERS}
	${TATOR_SCRIPTING_HEADERS}
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
	${PYBIND11_INCLUDE_DIR} # header only library
	${PYTHON_INCLUDE_DIRS}
	glm
	glfw
	)
target_link_libraries(tator
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	glm
	glfw
	${PYTHON_LIBRARIES}
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


