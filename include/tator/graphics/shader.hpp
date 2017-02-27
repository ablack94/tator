// Andrew Black
// February 26, 2017

#ifndef __TATOR_GRAPHICS_SHADER_HPP
#define __TATOR_GRAPHICS_SHADER_HPP

#include "tator/graphics/gl.hpp"

#include <string>
	using std::string;

namespace tator {
namespace graphics {

class Shader {
public:
	// Constructors / Destructors
	Shader();
	Shader(string source, GLenum type);
	~Shader();

	// Functions
	bool compile();
	bool compile(GLint* success, GLuint* id, string* error);
	bool destroy();

	// Accessors / Mutators
	string getSource();
	GLuint getId();
	bool isCompiled();

protected:
	string source; // shader source code
	GLuint id; // opengl id for compiled shader
	bool compiled; // flag if we've been compiled or not
	GLenum type; // shader type (vertex, fragment, etc...)
};

class VertexShader : public Shader {
public:
	VertexShader(string source) : Shader(source, GL_VERTEX_SHADER) { }
};

class FragmentShader : public Shader {
public:
	FragmentShader(string source) : Shader(source, GL_FRAGMENT_SHADER) { }
};

} // graphics
} // tator

#endif