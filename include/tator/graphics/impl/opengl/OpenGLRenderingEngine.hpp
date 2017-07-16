// Andrew Black
// July 09, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_OPENGL_OPENGLRENDERINGENGINE_HPP
#define HG_TATOR_GRAPHICS_OPENGL_OPENGLRENDERINGENGINE_HPP

#include "tator/graphics/IRenderable.hpp"
using tator::graphics::IRenderable;

#include "tator/graphics/RenderingEngine.hpp"
using tator::graphics::RenderingEngine;

#include "tator/graphics/data/VertexData.hpp"
using tator::graphics::data::IVertexData;
#include "tator/graphics/data/VertexAttributeInfo.hpp"
using tator::graphics::data::BaseVertexAttributeInfo;

#include "detail/OpenGLObject.hpp"
using tator::graphics::impl::opengl::detail::OpenGLObject;

namespace tator {
namespace graphics {

class OpenGLRenderingEngine : public RenderingEngine {
public:
	virtual void draw(IRenderable& renderable) {
		renderable.getMaterial()->activate();
		vertexDataToObjects(renderable.getVertexData());
		renderable.getMaterial()->deactivate();
	}

protected:
	GLenum vertexAttributeToEnum(BaseVertexAttributeInfo *info) {
		GLenum rval;
		if (info->isTypeIntegral()) {
			if (info->isTypeSigned()) {
				switch (info->getTypeSize()) {
				case sizeof(int8_t) : rval = GL_BYTE; break;
				case sizeof(int16_t) : rval = GL_SHORT; break;
				case sizeof(int32_t) : rval = GL_INT; break;
				default:
					throw std::exception("VertexAttribute type not supported");
				}
			}
			else {
				switch (info->getTypeSize()) {
				case sizeof(uint8_t) : rval = GL_UNSIGNED_BYTE; break;
				case sizeof(uint16_t) : rval = GL_UNSIGNED_SHORT; break;
				case sizeof(uint32_t) : rval = GL_UNSIGNED_INT; break;
				default:
					throw std::exception("VertexAttribute type not supported");
				}
			}
		}
		else {
			if (info->isTypeSigned()) {
				switch (info->getTypeSize()) {
				case sizeof(float) : rval = GL_FLOAT; break;
				default:
					throw std::exception("VertexAttribute type not supported");
				}
			}
			else {
				throw std::exception("Unsigned float VertexAttribute type not supported");
			}
		}
		return rval;
	}

	void vertexDataToObjects(IVertexData *data) {
		std::vector<std::unique_ptr<OpenGLObject>> objects;
		GLuint vao;
		//glGenVertexArrays(1, &id);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		objects.push_back(std::make_unique<OpenGLObject>(vao, [](GLuint id) { glDeleteVertexArrays(1, &id); }));
		//objects.push_back(std::move(OpenGLObject(id, [](GLuint id) { glDeleteVertexArrays(1, &id); })));
		std::vector<BaseVertexAttribute*> attributes = data->getAttributes();
		std::vector<OpenGLObject> attribute_objects;
		size_t i = 0;
		for (size_t i=0;i<attributes.size();++i) {
			//BaseVertexAttributeInfo *info = it->getAttributeInfo();
			BaseVertexAttributeInfo info = attributes[i]->getAttributeInfo();
			GLuint vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, info.getTotalSize() * attributes[i]->getLength(), attributes[i]->getData().data(), GL_STATIC_DRAW);
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, info.getDimensions()[0], vertexAttributeToEnum(&info), GL_FALSE, 0, 0);
			objects.push_back(std::make_unique<OpenGLObject>(vbo, [](GLuint id) { glDeleteBuffers(1, &id); }));
		}
		glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}
};

} // graphics
} // tator

#endif