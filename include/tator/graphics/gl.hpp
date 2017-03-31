// Andrew Black
// February 26, 2017

#ifndef __TATOR_GRAPHICS_GL_HPP
#define __TATOR_GRAPHICS_GL_HPP

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace tator {
namespace graphics {

class GlBoundObject;

class GlObject {
public:
	GlObject();
	virtual ~GlObject();

	bool hasId();
	bool hasId(GLuint* id);
	GLuint getId();

	virtual void bind();
	virtual void unbind();
	virtual void destroy();
protected:
	void setId(GLuint id);
private:
	bool has_id;
	GLuint id;
};

class GlBoundObject {
public:
	GlBoundObject(std::initializer_list<GlObject*> objects);
	~GlBoundObject();
protected:
	std::vector<GlObject*> objects;
};
/*
class OpenGLManager {
public:
	OpenGLManager() {
		this->initialized = false;
	}

	void setContext(GLFWwindow* window) {
		this->window = window;
		this->initialized = true;
	}

	// TODO: Have these call state functions, so when we destroy the opengl context
	// we can change their behavior to throw an error or something.
	void bind(GlObject& obj) {
		obj.bind();
	}
	void unbind(GlObject& obj) {
		obj.unbind();
	}
	void destroy(GlObject& obj) {
		obj.destroy();
	}

	void glGenVertexArrays(GLsizei n, GLuint* arrays) {
		glGenVertexArrays(n, arrays);
	}
	void glBindVertexArray(GLuint array) {
		glBindVertexArray(array);
	}

	void glGenBuffers(GLsizei n, GLuint* buffers) {
		glGenBuffers(n, buffers);
	}
	void glBindBuffer(GLenum target, GLuint buffer) {
		glBindBuffer(target, buffer);
	}
	void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage) {
		glBufferData(target, size, data, usage);
	}
	void glVertexAttribPointer(GLuint index, GLint size, 


protected:
	bool initialized;
	GLFWwindow *window;

};
*/
// graphics 
/*
class GlTexture2D : public GlObject {
public:
	enum class TextureWrap : int {
		REPEAT = GL_REPEAT,
	};
	enum class TextureInterpolation : int {
		LINEAR = GL_LINEAR,
	};

	GlTexture2D();
	virtual ~GlTexture2D();

	void bind() override {
		glBindTexture(GL_TEXTURE_2D, this->getId());
	}
	void unbind() override {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void destroy() override {
		glDestroyTextur
	}

protected:
	int width = -1;
	int height = -1;
	std::string data;
	TextureWrap wrap_s = TextureWrap::REPEAT;
	TextureWrap wrap_t = TextureWrap::REPEAT;
	TextureInterpolation min_filter = TextureInterpolation::LINEAR;
	TextureInterpolation mag_filter = TextureInterpolation::LINEAR;
	bool dirty_settings = true;
};
*/
}
}

#endif