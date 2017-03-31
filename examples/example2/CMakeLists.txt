cmake_minimum_required(VERSION 2.8)
project(example2)

# Example 1
set(EXAMPLE2_RESOURCES_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/Resources
	)
set(EXAMPLE2_RESOURCES
	${EXAMPLE2_RESOURCES_DIR}/shaders/default_fragment.glsl
	${EXAMPLE2_RESOURCES_DIR}/shaders/default_vertex.glsl
	)
	
set(EXAMPLE2_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/example2.cpp
	)

add_executable(example2 ${EXAMPLE2_SOURCES})
target_include_directories(example2 SYSTEM PUBLIC
	tator
	${OPENGL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
	glm
	glfw
	soil
	)
target_link_libraries(example2
	tator
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	glm
	glfw
	soil
	)
	
source_group("Resources" FILES ${RESOURCES_FILES})

add_custom_command(TARGET example2 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tator> $<TARGET_FILE_DIR:example2>
	COMMAND ${CMAKE_COMMAND} -E copy ${TATOR_GLEW_SHARED_LIB} $<TARGET_FILE_DIR:example2>
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${EXAMPLE2_RESOURCES_DIR} "$<TARGET_FILE_DIR:example2>/../Resources"
	)


