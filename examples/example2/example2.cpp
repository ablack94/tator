
#include "Windows.h"
#include "tator/graphics/gl.hpp"
	using tator::graphics::GlBoundObject;

#include <iostream>
#include <exception>
#include <stdint.h>

#include "tator/system/TatorException.hpp"
	using tator::system::TatorException;


#include "tator/graphics/Shader.hpp"
	using tator::graphics::Shader;
#include "tator/graphics/ShaderProgram.hpp"
	using tator::graphics::ShaderProgram;

#include "tator/graphics/Renderer.hpp"
	using tator::graphics::GameObject;
	using tator::graphics::Component;
	using tator::graphics::TexturedQuadComponent;

#include "tator/graphics/Camera.hpp"
	using tator::graphics::Camera;

#include <glm/vec3.hpp>
	using glm::vec3;
#include <glm/vec4.hpp>
	using glm::vec4;
#include <glm/mat4x4.hpp>
	using glm::mat4;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tator/graphics/Renderer.hpp"
	using tator::graphics::Renderer;
	using tator::graphics::RendererFactory;
#include "tator/graphics/OpenGLRenderer.hpp"
	using tator::graphics::opengl::OpenGLRenderer;
	using tator::graphics::opengl::OpenGLRendererFactory;

#include "../soil/inc/SOIL/SOIL.h"

#include "tator/scripting/PythonEngine.hpp"
	using tator::scripting::PythonEngine;

#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
#include <pybind11/pybind11.h>
#include <pybind11/eval.h>
namespace py = pybind11;

typedef struct VertexData {
	GLfloat position[3];
	GLfloat color[3];
	GLfloat tex[2];
} VertexData;

VertexData makeVertexData(glm::vec3 position, glm::vec3 color, glm::vec2 tex) {
	VertexData vd;
	vd.position[0] = position.x;
	vd.position[1] = position.y;
	vd.position[2] = position.z;
	vd.color[0] = color.x;
	vd.color[1] = color.y;
	vd.color[2] = color.z;
	vd.tex[0] = tex.x;
	vd.tex[1] = tex.y;
	return vd;
}

GLuint screen_width = 640, screen_height = 480;

int _main(int argc, char **argv) {

	//wchar_t *program = Py_DecodeLocale(argv[0], NULL);
	//std::cout << "ARGV: " << argv[0] << std::endl;
	//Py_SetPythonHome(L"C:\\Python36-32\\Lib");
	//Py_SetPath(L"C:\\Python36-32;C:\\Python36-32\\DLLs;C:\\Python36-32\\Lib;C:\\Python36-32\\lib\\site-packages");
	//Py_SetProgramName(L"C:\\Python36-32\\python.exe");
	//Py_SetPath(L"C:\\Python36-32\\Lib");
	//Py_SetPythonHome(L"Lib");
	//Py_SetPath(L"Lib;Lib/site-packages");
	//Py_SetPath(L";DLLs;lib;lib/site-packages");
	//Py_Initialize();
	//std::wcout << "PROG: " << Py_GetProgramName() << std::endl;
	//std::wcout << "PATH: " << Py_GetPath() << std::endl;
	//std::wcout << "HOME: " << Py_GetPythonHome() << std::endl;

	//FreeConsole();
	// Create window
	GLFWwindow *window;
	if (!glfwInit()) {
		throw TatorException("Failed to initialize GLFW");
	}
	window = glfwCreateWindow(screen_width, screen_width, "Hello World!", NULL, NULL);
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

	Shader fs = Shader::fragmentShaderFromFile("Resources\\shaders\\default_fragment.glsl");
	if (!fs.compile()) throw std::runtime_error("Unable to compile fragment shader");
	Shader vs = Shader::vertexShaderFromFile("Resources\\shaders\\default_vertex.glsl");
	if (!vs.compile()) throw std::runtime_error("Unable to compile vertex shader");

	OpenGLRenderer renderer(fs, vs);

	ShaderProgram default_sp;
	default_sp.addShader(&fs);
	default_sp.addShader(&vs);
	default_sp.compile();

	Material *default_mat = renderer.getFactory().createMaterial(&vs, &fs);

	// Initialize python interpreter
	PythonEngine python_engine;
	python_engine.initialize();
	py::dict python_globals;
	python_globals["__builtins__"] = py::module::import("builtins");

	Quad *q = renderer.getFactory().createQuad();
	q->getData().addAttribute<float>("color", 4);
	q->getData().getAttribute("color")->getFloat().assign(4 * 4, 0.5f);
	q->setMaterial(default_mat);

	// Create textures
	
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // bind
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		int twidth, theight;
		unsigned char *image = SOIL_load_image("Resources/Textures/vader.jpg", &twidth, &theight, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind
	
	// Application loop

	glm::vec3 cube_positions[] = {
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.5f,0.0f,0.0f),
		glm::vec3(0.0f,0.0f,2.0f),
	};

	Camera c = Camera::fromLookAt(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0));
	glm::mat4 projection = glm::perspective(45.0f,
		(float)screen_width / (float)screen_height, 0.1f, 1000.0f);
	glm::mat4 model;
	glm::mat4 view = c.getViewMatrix();


	glm::mat4 view2 = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	for (int y = 0; y < 4; ++y) {
		for (int x = 0; x < 4; ++x) {
			std::cout << view2[y][x] << " ";
		}
		std::cout << std::endl;
	}
	float rotation = 0.0f;
	float r1 = 0.0f;
	float r2 = 0.0f;

	double last_time = glfwGetTime();
	uint32_t frame_count = 0;

	renderer.setProjection(projection);
	renderer.setView(view);

	//py::object scope = py::dict(); // empty scope
	//py::eval("print('hello')", py::dict(), py::dict());
								   //py::eval("import os\nprint(os.getcwd())", scope);
	//py::eval_file("./Resources/scripts/main.py", scope);

	while (!glfwWindowShouldClose(window)) {
		// time per frame
		double current_time = glfwGetTime();
		frame_count++;
		if (current_time - last_time >= 1.0) {
			printf("%f ms/frame\n", 1000.0 / frame_count);
			frame_count = 0;
			last_time = current_time;
		}

		glfwPollEvents(); // Check events
		/* RENDERING */
		// Clear backbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.f); // Set color to white
		glClear(GL_COLOR_BUFFER_BIT); // Clear buffer to set clear color
	
			GLfloat timeValue = glfwGetTime();
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0, 0, 0));
			//model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
			model = glm::rotate(model, rotation, glm::vec3(0, 1, 0));
			//model = glm::scale(model, glm::vec3(2, 2, 2));
			//model = glm::rotate(model, r1, glm::vec3(1, 0, 0));
			//model = glm::rotate(model, r2, glm::vec3(0, 0, 1));

			q->setTransform(model);
			//renderer.setTime(timeValue);

			q->getMaterial()->set("model", q->getTransform());
			q->getMaterial()->set("view", view);
			q->getMaterial()->set("projection", projection);
			q->getMaterial()->set("time", timeValue);
			std::cout << "TIME: " << timeValue << std::endl;

			q->getData().getAttribute("position")->getFloat()[0] += 0.00001f;
			q->getData().getAttribute("position")->getFloat()[1] -= 0.00001f;
			q->getData().getAttribute("position")->getFloat()[2] += 0.0002f;

			renderer.draw(*q);

			rotation += 0.001f;
			if (rotation >= 360) { rotation = 0.0f; }
			r1 += 0.1f;
			if (r1 >= 360) { r1 = 0.0f; }
			r2 += 0.05f;
			if (r2 >= 360) { r2 = 0.0f; }

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