#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <exception>

int main(void) {
	// Create window
	GLFWwindow *window;
	if (!glfwInit()) {
		return -1;
	}
	window = glfwCreateWindow(640, 480, "Hello World!", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw std::exception("Failed to initialize GLEW");
	}
	// Application loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(1.f, 1.f, 1.f, 1.f); // Set color to white
		glClear(GL_COLOR_BUFFER_BIT); // Clear buffer to set clear color
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}