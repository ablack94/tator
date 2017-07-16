
#include "Windows.h"

#include <iostream>
#include <exception>
#include <stdint.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "tator/system/TatorException.hpp"
	using tator::system::TatorException;

#include "tator/graphics/view/Camera.hpp"
	using tator::graphics::Camera;

#include <glm/vec3.hpp>
	using glm::vec3;
#include <glm/vec4.hpp>
	using glm::vec4;
#include <glm/mat4x4.hpp>
	using glm::mat4;

#include "tator/graphics/impl/opengl/OpenGLRenderingEngine.hpp"
using tator::graphics::OpenGLRenderingEngine;
#include "tator/graphics/impl/opengl/OpenGlMaterialFactory.hpp"
using tator::graphics::impl::opengl::OpenGLMaterialFactory;
#include "tator/graphics/data/VertexData.hpp"
using tator::graphics::data::VertexData;
#include "tator/graphics/IRenderable.hpp"
using tator::graphics::Renderable;

#include "tator/graphics/material/NumericMaterialSetting.hpp"
using tator::graphics::material::NumericMaterialSetting;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../soil/inc/SOIL/SOIL.h"

#include "ablack94/hmap/HMap.hpp"
	using ablack94::hmap::HMap;

int _main(int argc, char **argv) {
	// Create window
	GLFWwindow *window;
	if (!glfwInit()) {
		throw TatorException("Failed to initialize GLFW");
	}
	window = glfwCreateWindow(640, 480, "Hello World!", NULL, NULL);
	if (!window) {
		glfwTerminate();
		throw TatorException("Failed to create window!");
	}
	glfwMakeContextCurrent(window);
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw TatorException("Failed to initialize GLEW");
	}

	OpenGLRenderingEngine renderer;
	OpenGLMaterialFactory material_factory;

	std::string default_vs_code =
		"#version 330 core\n"
		"layout (location=0) in vec3 position;\n"
		"layout (location=1) in vec3 color;\n"
		"out vec3 frag_color;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(position, 1.0f);\n"
		"frag_color = color;\n"
		"}\n";
	Shader vs = Shader::createVertexShader(default_vs_code);
	std::string default_fs_code =
		"#version 330 core\n"
		"in vec3 frag_color;\n"
		"out vec3 color;\n"
		"uniform float ts;\n"
		"void main()\n"
		"{\n"
		"color = frag_color;\n"
		"}\n";

	std::shared_ptr<Material> default_mat = material_factory.createMaterial(default_vs_code, default_fs_code);

	VertexData vd;
	auto att1 = vd.addAttribute<float, 3, 1>("position");
	att1->addRow(std::array<float, 3>{-1.0f, -1.0f, 0.0f});
	att1->addRow(std::array<float, 3>{1.0f, -1.0f, 0.0f});
	att1->addRow(std::array<float, 3>{0.0f, 1.0f, 0.0f});
	auto att2 = vd.addAttribute<float, 3, 1>("color");
	att2->addRow(std::array<float, 3>{1, 0, 0});
	att2->addRow(std::array<float, 3>{0, 1, 0});
	att2->addRow(std::array<float, 3>{0, 0, 1});

	Renderable r(&vd, default_mat.get());

	uint64_t frame_count = 0;
	double last_time = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		// time per frame
		double current_time = glfwGetTime();
		frame_count++;
		if (current_time - last_time >= 1.0) {
			printf("%f ms/frame\n", 1000.0 / frame_count);
			frame_count = 0;
			last_time = current_time;
		}
	
		default_mat->setProperty("ts", std::make_unique<NumericMaterialSetting<float, 1>>(current_time));

		glfwPollEvents(); // Check events
		/* RENDERING */
		// Clear backbuffer
		glClearColor(0.1f, 0.3f, 0.1f, 1.f); // Set color to white
		glClear(GL_COLOR_BUFFER_BIT); // Clear buffer to set clear color
	
		GLfloat timeValue = glfwGetTime();
		renderer.draw(r);

		// Display
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

int main(int argc, char** argv) {
	try {
		return _main(argc, argv);
	}
	catch (std::exception& exc) {
		std::cout << "Unhandled exception: " << exc.what() << std::endl;
		throw exc;
	}

}