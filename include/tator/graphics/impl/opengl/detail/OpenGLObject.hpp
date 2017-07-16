// Andrew Black
// July 04, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_IMPL_OPENGL_DETAIL_OPENGLOBJECT_HPP
#define HG_TATOR_GRAPHICS_IMPL_OPENGL_DETAIL_OPENGLOBJECT_HPP

#include <GL/glew.h>

namespace tator {
namespace graphics {
namespace impl {
namespace opengl {
namespace detail {

typedef void(*gldelete)(GLuint id);

class OpenGLObject {
public:
	OpenGLObject(GLuint id, gldelete fp) {
		this->id = id;
		this->fp = fp;
	}
	~OpenGLObject() {
		fp(id);
	}
	OpenGLObject(const OpenGLObject&) = delete; // no copying
	OpenGLObject& operator=(const OpenGLObject&) & = delete; // no copy assignment

	GLuint getId() { return id; }

protected:
	GLuint id;
	gldelete fp; // function pointer to delete
};

} // detail
} // opengl
} // impl
} // graphics
} // tator

#endif