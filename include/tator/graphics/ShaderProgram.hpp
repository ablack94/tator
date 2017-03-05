// Andrew Black
// March 04, 2017

#ifndef __TATOR_GRAPHICS_SHADERPROGRAM_HPP
#define __TATOR_GRAPHICS_SHADERPROGRAM_HPP

#include <string>
#include <vector>

#include "tator/graphics/gl.hpp"
	using tator::graphics::GlObject;
#include "tator/graphics/shader.hpp"
	using tator::graphics::Shader;

namespace tator {
namespace graphics {
namespace detail {
	class IShaderProgram {
	public:
		virtual void bind();
		virtual void unbind();
		virtual void destroy();
		virtual void addShader(Shader* shader);
		virtual void clearShaders();
		virtual bool compile(GLint* success, GLuint* id, std::string* error);
		virtual bool isCompiled();
	};
}

class ShaderProgram : public detail::IShaderProgram, public GlObject {
public:
	// Constructors / Destructors
	ShaderProgram();
	~ShaderProgram();

	// GlObject
	void bind();
	void unbind();
	void destroy();
	// Functions
	void addShader(Shader* shader);
	void clearShaders();
	bool compile();
	bool compile(GLint* success, GLuint* id, std::string* error);
	bool isCompiled();

protected:
	std::vector<Shader*> shaders;
	detail::IShaderProgram *state;
	void setState(detail::IShaderProgram* new_state);

private:
	class _UncompiledShaderProgram : public detail::IShaderProgram {
	public:
		// Constructor(s) / Destructor
		_UncompiledShaderProgram(ShaderProgram& sp);
		~_UncompiledShaderProgram();
		// Functions
		void addShader(Shader* shader);
		void clearShaders();
		bool compile(GLint* success, GLuint* id, std::string* error);
		bool isCompiled();
	protected:
		ShaderProgram& sp;
	};

	class _CompiledShaderProgram : public detail::IShaderProgram {
	public:
		// Constructor(s) / Destructor
		_CompiledShaderProgram(ShaderProgram& sp);
		~_CompiledShaderProgram();
		// Functions
		void bind();
		void unbind();
		void destroy();
		bool isCompiled();
	protected:
		ShaderProgram& sp;
	};
};

}
}

#endif