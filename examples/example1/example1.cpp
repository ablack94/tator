
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

int _main() {
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
	// Compile shaders
	//GLuint fragment_shader = compileFragmentShader(fragment_shader_source);
	//GLuint vertex_shader = compileVertexShader(vertex_shader_source);
	GLuint t = glCreateShader(GL_VERTEX_SHADER);

	Shader fragment_shader = Shader::fragmentShaderFromFile("Resources\\shaders\\default_fragment.glsl");
	Shader vertex_shader = Shader::vertexShaderFromFile("Resources\\shaders\\default_vertex.glsl");
	std::cout << "Compiling fragment shader..." << std::endl;
	bool _fs = fragment_shader.compile();
	std::cout << "Compiling vertex shader..." << std::endl;
	bool _vs = vertex_shader.compile();

	std::cout << "Shaders done! FS: " << _fs << " VS: " << _vs << std::endl;

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


	VertexData _vd[]{
		makeVertexData(glm::vec3(0.5f,0.5f,0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)),
		makeVertexData(glm::vec3(0.5f,-0.5f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0, 0.0f)),
		makeVertexData(glm::vec3(-0.5f,-0.5f,0.0f), glm::vec3(0.0f,0.0f,1.0f), glm::vec2(0.0f,0.0f)),
		makeVertexData(glm::vec3(-0.5f,0.5f,0.0f), glm::vec3(1.0f,1.0f,0.0f), glm::vec2(0.0f,1.0f)),
	};

	char* vd = reinterpret_cast<char*>(_vd);
	std::cout << sizeof(_vd) << " " << sizeof(VertexData) << std::endl;
	std::cout << "BYTES: " << sizeof(_vd) * sizeof(VertexData) << std::endl;

	GLuint indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	// Make shapes
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
		//glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexData, position));
		//glVertexAttribBinding(0, 0);
		//glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(VertexData, color));
		//glVertexAttribBinding(1, 0);
		//glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(VertexData, tex));
		//glVertexAttribBinding(2, 0);
		// Send vertices to GPU
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sizeof(_vd), vd, GL_STATIC_DRAW);
			// Send indices of shape to GPU
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			// Determine what data is sent to the vertex shader
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);

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

	glm::vec3 cube_positions[] = {
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.5f,0.0f,0.0f),
		glm::vec3(0.0f,0.0f,2.0f),
	};

	Camera c = Camera::fromLookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0));
	glm::mat4 projection = glm::perspective(45.0f,
		(float)screen_width / (float)screen_height, 0.1f, 1000.0f);
	glm::mat4 model;
	glm::mat4 view = c.getViewMatrix();

	std::cout << "Camera" << std::endl;
	std::cout << "Position: " << c.getPosition().x << " " << c.getPosition().y << " " << c.getPosition().z << std::endl;
	std::cout << "Forward: " << c.getForward().x << " " << c.getForward().y << " " << c.getForward().z << std::endl;
	std::cout << "Up: " << c.getUp().x << " " << c.getUp().y << " " << c.getUp().z << std::endl;
	std::cout << "--------------------------" << std::endl;
	

	for (int y = 0; y < 4; ++y) {
		for (int x = 0; x < 4; ++x) {
			std::cout << view[y][x] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;

	glm::mat4 view2 = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	for (int y = 0; y < 4; ++y) {
		for (int x = 0; x < 4; ++x) {
			std::cout << view2[y][x] << " ";
		}
		std::cout << std::endl;
	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents(); // Check events
		/* RENDERING */
		// Clear backbuffer
		glClearColor(0.5f, 0.5f, 0.5f, 1.f); // Set color to white
		glClear(GL_COLOR_BUFFER_BIT); // Clear buffer to set clear color
		// Draw
		{ auto bound=GlBoundObject({ &sp });
			glUseProgram(sp.getId());
			model = glm::translate(model, glm::vec3(0, 0, 0));
			//model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
			GLfloat timeValue = glfwGetTime();

			GLint loc_model = glGetUniformLocation(sp.getId(), "model");
			glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(model));
			GLint loc_view = glGetUniformLocation(sp.getId(), "view");
			glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(view));
			GLint loc_projection = glGetUniformLocation(sp.getId(), "projection");
			glUniformMatrix4fv(loc_projection, 1, GL_FALSE, glm::value_ptr(projection));
			GLint colorLoc = glGetUniformLocation(sp.getId(), "time");

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glUniform1i(glGetUniformLocation(sp.getId(), "texture1"), 0);

			glUniform1f(colorLoc, timeValue);
			glBindVertexArray(VAO); // bind
			for (glm::vec3 p : cube_positions) {
				glm::mat4 _m;
				_m = glm::translate(_m, p);
				//_m = glm::rotate(_m, 45.0f, glm::vec3(0,1,0));
				glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(_m));
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
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
		std::cout << "Unhandled exception: " << exc.what() << std::endl;
		throw exc;
	}
}