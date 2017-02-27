
#include "tator/graphics/gl.hpp"

#include <iostream>
#include <exception>

#include "tator/system/TatorException.hpp"
	using tator::system::TatorException;

#include "tator/graphics/Shader.hpp"
	using tator::graphics::VertexShader;
	using tator::graphics::FragmentShader;

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
	if (!success) {
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		throw TatorException(infoLog);
	}
	return shader_program;
}

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
		throw TatorException("Failed to initialize GLEW");
	}
	// Compile shaders
	//GLuint fragment_shader = compileFragmentShader(fragment_shader_source);
	//GLuint vertex_shader = compileVertexShader(vertex_shader_source);
	FragmentShader fragment_shader(fragment_shader_source);
	VertexShader vertex_shader(vertex_shader_source);

	fragment_shader.compile();
	vertex_shader.compile();

	// Make shader program
	GLuint sprog = createShaderProgram({vertex_shader.getId(), fragment_shader.getId()});
	
	fragment_shader.destroy();
	vertex_shader.destroy();

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
			glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind
	glBindVertexArray(0); // unbind

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe

	// Application loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents(); // Check events
		/* RENDERING */
		// Clear backbuffer
		glClearColor(0.5f, 0.5f, 0.5f, 1.f); // Set color to white
		glClear(GL_COLOR_BUFFER_BIT); // Clear buffer to set clear color
		// Draw
		glUseProgram(sprog);
		glBindVertexArray(VAO); // bind
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); // unbind

		// Display
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

int _main(void) {
	try {
		return _main();
	}
	catch (std::exception& exc) {
		std::cerr << "Unhandled exception: " << exc.what() << std::endl;
		system("pause");
	}
}