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
	~GlObject();

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
	GlBoundObject(std::initializer_list<GlObject> objects);
	~GlBoundObject();
protected:
	std::vector<GlObject> objects;
};

}
}

#endif