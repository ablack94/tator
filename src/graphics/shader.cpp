// Andrew Black
// February 26, 2017

#include "tator/graphics/shader.hpp"
	using tator::graphics::Shader;

#include "tator/system/TatorException.hpp"
	using tator::system::TatorException;

Shader::Shader() {
	this->source = "";
	this->id = 0;
	this->compiled = false;
	this->type = GL_INVALID_VALUE;
}

Shader::Shader(string source, GLenum type) {
	this->source = source;
	this->id = 0;
	this->compiled = false;
	this->type = type;
}

Shader::~Shader() { 
	if (this->isCompiled()) {
		glDeleteShader(this->getId());
	}
}

bool Shader::compile() {
	GLint success;
	GLuint id;
	string str;
	bool result = this->compile(&success, &id, &str);
	if (!result) {
		throw TatorException(str);
	}
	return result;
}

bool Shader::compile(GLint* success, GLuint* id, string* error) {
	GLuint sid = glCreateShader(this->type);
	this->id = sid;
	const char *c = source.c_str();
	glShaderSource(sid, 1, &c, NULL);
	glCompileShader(sid);
	GLint _success;
	glGetShaderiv(sid, GL_COMPILE_STATUS, &_success);
	GLchar info[512];
	bool succeeded = _success;
	this->compiled = succeeded;
	if(!succeeded) {
		glGetShaderInfoLog(sid, 512, NULL, info);
		*error = string(info);
	}
	if (id != NULL) {
		*id = sid;
	}
	if (success != NULL) {
		*success = _success;
	}
	return succeeded;
}

bool Shader::destroy() {
	bool rval = this->isCompiled();
	if (rval) {
		glDeleteShader(this->getId());
		this->compiled = false;
		this->id = 0;
	}
	return rval;
}

string Shader::getSource() {
	return this->source;
}

GLuint Shader::getId() {
	return this->id;
}

bool Shader::isCompiled() {
	return this->compiled;
}


