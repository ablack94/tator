// Andrew Black
// February 26, 2017

#include "tator/graphics/shader.hpp"
	using tator::graphics::IShaderInterface;
	using tator::graphics::Shader;

#include "tator/system/TatorException.hpp"
	using tator::system::TatorException;

#include <iostream>
#include <fstream>

/*
IShaderInterface default
*/
#define _ISHADER_INTERFACE_DEFAULT_ERROR "Invalid operation."
void IShaderInterface::bind() {
	throw TatorException(_ISHADER_INTERFACE_DEFAULT_ERROR);
}
void IShaderInterface::unbind() {
	throw TatorException(_ISHADER_INTERFACE_DEFAULT_ERROR);
}
void IShaderInterface::destroy() {
	throw TatorException(_ISHADER_INTERFACE_DEFAULT_ERROR);
}
bool IShaderInterface::compile(GLint* success, GLuint* id, std::string* error) {
	throw TatorException(_ISHADER_INTERFACE_DEFAULT_ERROR);
}
bool IShaderInterface::isCompiled() {
	throw TatorException(_ISHADER_INTERFACE_DEFAULT_ERROR);
}

/*
Uncompiled shader
*/
Shader::_UncompiledShader::_UncompiledShader(Shader& shader) : shader(shader) {
	this->shader.source = "";
}

Shader::_UncompiledShader::_UncompiledShader(Shader& shader, std::string source) : shader(shader) {
	this->shader.source = source;
}

Shader::_UncompiledShader::~_UncompiledShader() { }

bool Shader::_UncompiledShader::compile(GLint* success, GLuint* id, std::string* error) {
	// Compile shader
	GLuint _id = glCreateShader(this->shader.type);
	const char *c = this->shader.source.c_str();
	glShaderSource(_id, 1, &c, NULL);
	glCompileShader(_id);
	// Validate
	GLint _success;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &_success);
	if (success != NULL) *success = _success;
	if (_success) {
		this->shader.setId(_id);
		this->shader.setState(new Shader::_CompiledShader(this->shader));
		if (id != NULL) *id = _id;
		return true;
	} else if(error != NULL) {
		// Get error details
		GLchar info[512];
		glGetShaderInfoLog(_id, 512, NULL, info);
		*error = std::string(info);
		return false;
	}
	return false;
}

bool Shader::_UncompiledShader::isCompiled() {
	return false;
}

/*
Compiled Shader
*/
Shader::_CompiledShader::_CompiledShader(Shader& shader) : shader(shader) { }

Shader::_CompiledShader::~_CompiledShader() { }

void Shader::_CompiledShader::bind() {
	throw TatorException("Cannot bind shaders.");
}

void Shader::_CompiledShader::unbind() {
	throw TatorException("Cannot unbind shaders.");
}

void Shader::_CompiledShader::destroy() {
	GLuint _id;
	if (this->shader.hasId(&_id)) {
		glDeleteShader(_id);
	}
	this->shader.setState(new Shader::_UncompiledShader(this->shader));
}

bool Shader::_CompiledShader::isCompiled() {
	return true;
}

/*
Shader public interface
*/

Shader::Shader(GLenum type) {
	this->type = type;
	this->source = "";
	this->state = new Shader::_UncompiledShader(*this);
}

Shader::Shader(GLenum type, std::string source) {
	this->type = type;
	this->source = source;
	this->state = new Shader::_UncompiledShader(*this);
}

Shader::~Shader() { 
	this->destroy();
}

Shader Shader::fromFile(GLenum type, std::string file) {
	std::ifstream input(file);
	if (input.is_open()) {
		string src((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
		return Shader(type, src);
	}
	else {
		throw TatorException("Unable to open file " + file);
	}
}

Shader Shader::vertexShaderFromFile(std::string file) {
	return Shader::fromFile(GL_VERTEX_SHADER, file);
}

Shader Shader::fragmentShaderFromFile(std::string file) {
	return Shader::fromFile(GL_FRAGMENT_SHADER, file);
}


void Shader::bind() {
	this->state->bind();
}

void Shader::unbind() {
	this->state->unbind();
}

void Shader::destroy() {
	this->state->destroy();
	GlObject::destroy();
}

bool Shader::compile() {
	string err;
	bool success = this->state->compile(NULL, NULL, &err);
	if (success) {
		return success;
	} else {
		throw TatorException("Error compiling shader: " + err);
	}
}

bool Shader::compile(GLint* success, GLuint* id, std::string* error) {
	return this->state->compile(success, id, error);
}

std::string Shader::getSource() {
	return this->source;
}

void Shader::setSource(std::string source) {
	this->source = source;
}

bool Shader::isCompiled() {
	return this->state->isCompiled();
}

void Shader::setState(IShaderInterface* new_state) {
	if (this->state != NULL)
		delete this->state;
	this->state = new_state;
}

