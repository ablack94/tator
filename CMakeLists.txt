cmake_minimum_required(VERSION 2.8)
project(tator)

##########################
# LIBRARIES
##########################
# ---- GLFW ----
set(TATOR_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
add_subdirectory(${TATOR_LIBS_DIR}/glfw) # GLFW from source
# ---- GLEW ----
set(GLEW_INCLUDE_DIR ${TATOR_LIBS_DIR}/glew/include)
set(GLEW_LIBRARY_DEBUG ${TATOR_LIBS_DIR}/glew/lib/Release/Win32/glew32s.lib)
set(GLEW_LIBRARY_RELEASE ${TATOR_LIBS_DIR}/glew/lib/Release/Win32/glew32s.lib)
find_package(GLEW REQUIRED) # GLEW
# ---- SOIL ----
add_subdirectory(${TATOR_LIBS_DIR}/soil)
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
	${TATOR_SOURCE_DIR}/graphics/Shader.cpp
	)
set(TATOR_GRAPHICS_HEADERS
	${_TATOR_INCLUDE_DIR}/graphics/gl.hpp
	${_TATOR_INCLUDE_DIR}/graphics/Shader.hpp
	)
source_group("graphics" FILES ${TATOR_GRAPHICS_HEADERS} ${TATOR_GRAPHICS_SOURCE})
	
# ---- System module ----
set(TATOR_SYSTEM_SOURCE "")
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
set(MYAPP_MAIN ${TATOR_SOURCE_DIR}/test1.cpp)
add_executable(myapp ${MYAPP_MAIN} ${TATOR_SOURCES} ${TATOR_HEADERS})
target_include_directories(myapp SYSTEM PUBLIC
	${TATOR_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR} 
	${GLEW_INCLUDE_DIRS}
	glfw
	soil
)
target_link_libraries(myapp
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	glfw
	soil
)


