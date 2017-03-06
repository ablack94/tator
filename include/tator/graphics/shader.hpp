// Andrew Black
// February 26, 2017

#ifndef __TATOR_GRAPHICS_SHADER_HPP
#define __TATOR_GRAPHICS_SHADER_HPP

#include "tator/graphics/gl.hpp"
	using tator::graphics::GlObject;

#include <string>

namespace tator {
namespace graphics {
namespace detail {
	class IShader {
	public:
		virtual void bind();
		virtual void unbind();
		virtual void destroy();
		virtual bool compile(GLint* success, GLuint* id, std::string* error);
		virtual bool isCompiled();
	};
}
using detail::IShader;

class Shader : public IShader, public GlObject {
public:
	// Constructors / Destructors
	Shader(GLenum type);
	Shader(GLenum type, std::string source);
	~Shader();

	// Builders
	static Shader fromFile(GLenum type, std::string file);
	static Shader vertexShaderFromFile(std::string file);
	static Shader fragmentShaderFromFile(std::string file);

	// Functions
	void bind();
	void unbind();
	void destroy();
	bool compile();
	bool compile(GLint* success, GLuint* id, std::string* error);
	std::string getSource();
	void setSource(std::string source);
	bool isCompiled();

protected:
	GLenum type;
	std::string source;

	IShader *state;
	void setState(IShader* new_state);

private:
	class _UncompiledShader : public IShader {
	public:
		// Constructor(s) / Destructor
		_UncompiledShader(Shader& shader);
		_UncompiledShader(Shader& shader, std::string source);
		~_UncompiledShader();
		// Functions
		virtual void destroy() override;
		bool compile(GLint* success, GLuint* id, std::string* error);
		bool isCompiled();
	protected:
		Shader& shader;
	};

	class _CompiledShader : public IShader {
	public:
		// Constructor(s) / Destructor
		_CompiledShader(Shader& shader);
		~_CompiledShader();
		// Functions
		void bind();
		void unbind();
		void destroy();
		bool isCompiled();
	protected:
		Shader& shader;
	};
};

} // graphics
} // tator

#endif