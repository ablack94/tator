// Andrew Black
// March 18, 2017

#ifndef _HGUARD_TATOR_GRAPHICS_OPENGLRENDERER_HPP
#define _HGUARD_TATOR_GRAPHICS_OPENGLRENDERER_HPP

#include "tator/graphics/Renderer.hpp"
using tator::graphics::Renderer;
using tator::graphics::RendererFactory;
using tator::graphics::Texture2D;
using tator::graphics::TextureFormat;
using tator::graphics::TextureWrap;
using tator::graphics::TextureInterpolation;

#include "tator/graphics/gl.hpp"
using tator::graphics::GlObject;

#include <map>

namespace tator {
namespace graphics {
namespace opengl {
namespace detail {
class OpenGLTexture2D : public Texture2D, public GlObject {
public:
	OpenGLTexture2D(std::string data, int width, int height, int texture_format) {
		GLuint _id;
		glGenTextures(1, &_id);
		this->setId(_id);
		this->width = width;
		this->height = height;
		this->data = data;
		this->gl_texture_format = texture_format;
		//glBindTexture(GL_TEXTURE_2D, _id);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexImage2D(GL_TEXTURE_2D, 0, texture_format, width, height, 0,
		//	texture_format, GL_UNSIGNED_BYTE, data.data());
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	~OpenGLTexture2D() {
		this->destroy();
	}
	void prepare() override {
		glBindTexture(GL_TEXTURE_2D, this->getId());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, gl_texture_format, width, height, 0,
			gl_texture_format, GL_UNSIGNED_BYTE, data.data());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void cleanup() override {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void destroy() override {
		if (this->hasId()) {
			GLuint _id = this->getId();
			glDeleteTextures(1, &_id);
			GlObject::destroy();
		}
	}

protected:
	int gl_texture_format;
};
} // detail
using namespace detail;

class OpenGLRenderer;

class OpenGLRendererFactory : public RendererFactory {
public:
	OpenGLRendererFactory(OpenGLRenderer& renderer) : renderer(renderer) { }

	Texture2D* createTexture(std::string data, int width, int height,
			TextureFormat format, TextureWrap wrap_s,
			TextureWrap wrap_t, TextureInterpolation min_interp,
			TextureInterpolation mag_interp) {
		return new OpenGLTexture2D(data, width, height, GL_RGB);
	}
protected:
	OpenGLRenderer &renderer;
};

class OpenGLRenderer : public Renderer {
public:
	OpenGLRenderer() : fact(*this) { }

	RendererFactory& getFactory() {
		return this->fact;
	}
protected:
	OpenGLRendererFactory fact;
};

} // opengl
} // graphics
} // tator

#endif