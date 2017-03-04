cmake_minimum_required(VERSION 2.8)
project(example1)

# Example 1
set(EXAMPLE1_RESOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Resources
	)
set(EXAMPLE1_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/example1.cpp
	)

add_executable(example1 ${EXAMPLE1_SOURCES})
target_include_directories(example1 SYSTEM PUBLIC
	tator
	${OPENGL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
	glm
	glfw
	)
target_link_libraries(example1
	tator
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	glm
	glfw
	)

add_custom_command(TARGET example1 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tator> $<TARGET_FILE_DIR:example1>
	COMMAND ${CMAKE_COMMAND} -E copy ${TATOR_GLEW_SHARED_LIB} $<TARGET_FILE_DIR:example1>
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${EXAMPLE1_RESOURCES} "$<TARGET_FILE_DIR:example1>/Resources"
	)


