
#include "Windows.h"
#include "tator/graphics/gl.hpp"
	using tator::graphics::GlBoundObject;

#include <iostream>
#include <exception>

#include "tator/system/TatorException.hpp"
	using tator::system::TatorException;

#include "tator/graphics/Shader.hpp"
	using tator::graphics::Shader;
#include "tator/graphics/ShaderProgram.hpp"
	using tator::graphics::ShaderProgram;

#include <glm/vec3.hpp>
	using glm::vec3;
#include <glm/vec4.hpp>
	using glm::vec4;
#include <glm/mat4x4.hpp>
	using glm::mat4;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

#include "../soil/inc/SOIL/SOIL.h"

// Shaders
GLchar *vertex_shader_source =
"#version 330 core\n\
layout (location=0) in vec3 position;\n\
void main(){\n\
gl_Position = vec4(position.x, 0.5f - position.y, position.z, 1.0);\n\
}\0";

GLchar *fragment_shader_source =
"#version 330 core\n\
out vec4 color;\n\
void main(){\n\
color=vec4(1.0f,1.0f,1.0f,1.0f);\n\
}\n";

void compileShader(GLuint id, GLchar *src) {
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);
	GLint success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	GLchar infoLog[512];
	if (!success) {
		glGetShaderInfoLog(id, sizeof(infoLog), NULL, infoLog);
		throw TatorException(infoLog);
	}
}

GLuint compileVertexShader(GLchar *src) {
	GLuint id = glCreateShader(GL_VERTEX_SHADER);
	compileShader(id, src);
	return id;
}

GLuint compileFragmentShader(GLchar *src) {
	GLuint id = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(id, src);
	return id;
}

GLuint createShaderProgram(std::initializer_list<GLuint> shaders) {
	// Create the shader program
	GLuint shader_program = glCreateProgram();
	for (auto shader : shaders) {
		glAttachShader(shader_program, shader);
	}
	glLinkProgram(shader_program);
	// Validate the shader program
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (success == GLEW_OK) {
		return shader_program;
	} else {
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		throw TatorException(infoLog);
	}
}

int _main() {
	//FreeConsole();
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
	// Compile shaders
	//GLuint fragment_shader = compileFragmentShader(fragment_shader_source);
	//GLuint vertex_shader = compileVertexShader(vertex_shader_source);
	GLuint t = glCreateShader(GL_VERTEX_SHADER);

	Shader fragment_shader = Shader::fragmentShaderFromFile("Resources\\shaders\\default_fragment.glsl");
	Shader vertex_shader = Shader::vertexShaderFromFile("Resources\\shaders\\default_vertex.glsl");
	fragment_shader.compile();
	vertex_shader.compile();

	// Make shader program
	//GLuint sprog = createShaderProgram({vertex_shader.getId(), fragment_shader.getId()});
	//fragment_shader.destroy();
	//vertex_shader.destroy();

	ShaderProgram sp;
	sp.addShader(&vertex_shader);
	sp.addShader(&fragment_shader);
	sp.compile();
	vertex_shader.destroy();
	fragment_shader.destroy();

	// Data
	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	GLuint indices[] = {
		0, 1, 3
		//1, 2, 3
	};
	// Make shapes
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
		// Send vertices to GPU
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// Send indices of shape to GPU
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			// Determine what data is sent to the vertex shader
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, )
			glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind
	glBindVertexArray(0); // unbind

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

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe

	// Application loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents(); // Check events
		/* RENDERING */
		// Clear backbuffer
		glClearColor(0.5f, 0.5f, 0.5f, 1.f); // Set color to white
		glClear(GL_COLOR_BUFFER_BIT); // Clear buffer to set clear color
		// Draw
		{ GlBoundObject({ sp });
			// Bind textures
			glBindTexture(GL_TEXTURE_2D, texture1);

			glBindVertexArray(VAO); // bind
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0); // unbind
		}

		// Display
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

int main(void) {
	try {
		return _main();
	}
	catch (std::exception& exc) {
		std::cerr << "Unhandled exception: " << exc.what() << std::endl;
		throw exc;
	}
}