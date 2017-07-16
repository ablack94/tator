// Andrew Black
// July 04, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_IMPL_OPENGL_DETAIL_SHADER_HPP
#define HG_TATOR_GRAPHICS_IMPL_OPENGL_DETAIL_SHADER_HPP

#include <GL/glew.h>

#include <string>
#include <memory>

#include "detail/OpenGLObject.hpp"
using tator::graphics::impl::opengl::detail::OpenGLObject;

namespace tator {
namespace graphics {
namespace impl {
namespace opengl {
namespace detail {

enum class ShaderType { VERTEX, FRAGMENT };

class Shader {
public:
	// Factory methods
	static Shader createFragmentShader(std::string code) {
		return Shader(ShaderType::FRAGMENT, code);
	}
	static Shader createVertexShader(std::string code) {
		return Shader(ShaderType::VERTEX, code);
	}

	// Constructor
	Shader(ShaderType type, std::string code) {
		this->type = type;
		this->obj = nullptr;
		GLenum _type;
		switch (type) {
		case ShaderType::VERTEX:
			_type = GL_VERTEX_SHADER;
			break;
		case ShaderType::FRAGMENT:
			_type = GL_FRAGMENT_SHADER;
			break;
		}
		// Compile shader
		GLuint _id = glCreateShader(_type);
		const char *c = code.c_str();
		glShaderSource(_id, 1, &c, NULL);
		glCompileShader(_id);
		// Validate
		GLint success;
		glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
		if (success == GL_TRUE) {
			this->obj = std::unique_ptr<OpenGLObject>(new OpenGLObject(_id, [](GLuint id) {glDeleteShader(id); }));
		}
		else {
			// Get error details
			GLchar info[512];
			glGetShaderInfoLog(_id, 512, NULL, info);
			glDeleteShader(_id);
			throw std::exception(std::string(info).c_str());
		}
	}

	GLuint getId() {
		if (obj != nullptr)
			return obj->getId();
		else 
			throw std::exception("No OpenGL ID!");
	}
		
protected:
	ShaderType type;
	std::unique_ptr<OpenGLObject> obj;
};

} // detail
} // opengl
} // impl
} // graphics
} // tator

#endif