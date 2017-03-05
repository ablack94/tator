// Andrew Black
// March 04, 2017

#include "tator/graphics/ShaderProgram.hpp"
	using tator::graphics::detail::IShaderProgram;
	using tator::graphics::ShaderProgram;
#include "tator/system/TatorException.hpp"
	using tator::system::TatorException;

/*
IShaderProgram default
*/
#define _ISHADERPROGRAM_INTERFACE_DEFAULT_ERROR "Invalid operation."
void IShaderProgram::addShader(Shader* shader) {
	throw TatorException(_ISHADERPROGRAM_INTERFACE_DEFAULT_ERROR);
}
void IShaderProgram::clearShaders() {
	throw TatorException(_ISHADERPROGRAM_INTERFACE_DEFAULT_ERROR);
}
void IShaderProgram::bind() {
	throw TatorException(_ISHADERPROGRAM_INTERFACE_DEFAULT_ERROR);
}
void IShaderProgram::unbind() {
	throw TatorException(_ISHADERPROGRAM_INTERFACE_DEFAULT_ERROR);
}
void IShaderProgram::destroy() {
	throw TatorException(_ISHADERPROGRAM_INTERFACE_DEFAULT_ERROR);
}
bool IShaderProgram::compile(GLint* success, GLuint* id, std::string* error) {
	throw TatorException(_ISHADERPROGRAM_INTERFACE_DEFAULT_ERROR);
}
bool IShaderProgram::isCompiled() {
	throw TatorException(_ISHADERPROGRAM_INTERFACE_DEFAULT_ERROR);
}

/*
Uncompiled ShaderProgram
*/
ShaderProgram::_UncompiledShaderProgram::_UncompiledShaderProgram(ShaderProgram& sp) : sp(sp) { }
ShaderProgram::_UncompiledShaderProgram::~_UncompiledShaderProgram() { }
void ShaderProgram::_UncompiledShaderProgram::addShader(Shader* shader) {
	this->sp.shaders.push_back(shader);
}
void ShaderProgram::_UncompiledShaderProgram::clearShaders() {
	this->sp.shaders.clear();
}
bool ShaderProgram::_UncompiledShaderProgram::compile(GLint* success, GLuint* id, std::string* error) {
	if (this->isCompiled()) {
		throw TatorException("ShaderProgram is already compiled. You must call destroy first.");
	}
	// Create shader program
	GLuint _id = glCreateProgram();
	for (auto shader : this->sp.shaders) {
		glAttachShader(_id, shader->getId());
	}
	glLinkProgram(_id);
	// Validate
	GLint _success;
	GLchar info_log[512];
	glGetProgramiv(_id, GL_LINK_STATUS, &_success);
	bool result = (_success == GLEW_OK);
	if (result) {
		this->sp.setId(_id);
		this->sp.setState(new ShaderProgram::_CompiledShaderProgram(this->sp));
		if (success != NULL) *success = _success;
		if (id != NULL) *id = _id;
	} else {
		glDeleteProgram(_id);
		glGetProgramInfoLog(_id, 512, NULL, info_log);
		*error = std::string(info_log);
	}
	return result;
}
bool ShaderProgram::_UncompiledShaderProgram::isCompiled() {
	return false;
}

/*
Compiled ShaderProgram
*/
ShaderProgram::_CompiledShaderProgram::_CompiledShaderProgram(ShaderProgram& sp) : sp(sp) { }
ShaderProgram::_CompiledShaderProgram::~_CompiledShaderProgram() { }
void ShaderProgram::_CompiledShaderProgram::bind() {
	glUseProgram(this->sp.getId());
}
void ShaderProgram::_CompiledShaderProgram::unbind() {
	glUseProgram(0);
}
void ShaderProgram::_CompiledShaderProgram::destroy() {
	glDeleteProgram(this->sp.getId());
	this->sp.setState(new ShaderProgram::_UncompiledShaderProgram(this->sp));
}
bool ShaderProgram::_CompiledShaderProgram::isCompiled() {
	return true;
}

/*
ShaderProgram public interface
*/
ShaderProgram::ShaderProgram() {
	this->state = new ShaderProgram::_UncompiledShaderProgram(*this);
}
ShaderProgram::~ShaderProgram() {
	this->destroy();
}

void ShaderProgram::bind() {
	this->state->bind();
}
void ShaderProgram::unbind() {
	this->state->unbind();
}
void ShaderProgram::destroy() {
	this->state->destroy();
}
void ShaderProgram::addShader(Shader* shader) {
	this->state->addShader(shader);
}
void ShaderProgram::clearShaders() {
	this->state->clearShaders();
}
bool ShaderProgram::compile() {
	std::string err;
	bool success = this->state->compile(NULL, NULL, &err);
	if (success) {
		return success;
	} else {
		throw TatorException("Error compiling shader program: " + err);
	}
}
bool ShaderProgram::compile(GLint* success, GLuint* id, std::string* error) {
	return this->state->compile(success, id, error);
}
bool ShaderProgram::isCompiled() {
	return this->state->isCompiled();
}
void ShaderProgram::setState(IShaderProgram* new_state) {
	if (this->state != NULL) {
		delete this->state;
	}
	this->state = new_state;
}





