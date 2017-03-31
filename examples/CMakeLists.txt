cmake_minimum_required(VERSION 2.8)
project(examples)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/example1")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/example2")

# Testing/Development project
#set(EXAMPLE1_DIR ${CMAKE_CURRENT_SOURCE_DIR}/example1)
#set(EXAMPLE1_SOURCES
#	${EXAMPLE1_DIR}/example1.cpp
#)
#add_executable(example1 ${EXAMPLE1_SOURCES})
#target_include_directories(example1 SYSTEM PUBLIC
#	tator
#	${OPENGL_INCLUDE_DIR}
#	${GLEW_INCLUDE_DIRS}
#	glm
#	glfw
#)
#target_link_libraries(example1
#	tator
#	${OPENGL_LIBRARIES}
#	${GLEW_LIBRARIES}
#	glm
#	glfw
#)

#add_custom_command(TARGET example1 POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tator> $<TARGET_FILE_DIR:example1>
#	COMMAND ${CMAKE_COMMAND} -E copy ${TATOR_GLEW_SHARED_LIB} $<TARGET_FILE_DIR:example1>
#	)

#set_target_properties(example1 PROPERTIES
#	ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/../../Debug"
#	LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/../../Debug"
#	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/../../Debug"
#	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/../../Release"
#	LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/../../Release"
#	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/../../Release"
#)
	
	
	
	