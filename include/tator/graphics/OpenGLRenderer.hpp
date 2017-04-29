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
using tator::graphics::Material;

using tator::graphics::Quad;

#include "tator/graphics/gl.hpp"
using tator::graphics::GlObject;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tator/graphics/shader.hpp"
using tator::graphics::Shader;
#include "tator/graphics/ShaderProgram.hpp"
using tator::graphics::ShaderProgram;

#include <map>

namespace tator {
namespace graphics {
namespace opengl {
namespace detail {

class OpenGlMaterial : public Material {
public:
	OpenGlMaterial(Shader* vs, Shader* fs) {
		this->sp.addShader(vs);
		this->sp.addShader(fs);
		this->sp.compile();
	}

	void activate() {
		this->sp.bind();
		for (auto it = settings.begin(); it != settings.end(); ++it) {
			GLint loc = glGetUniformLocation(this->sp.getId(), it->first.c_str());
			glm::vec2 _v2;
			glm::vec3 _v3;
			glm::vec4 _v4;
			glm::mat4 _m4;
			if (loc != -1) {
				switch (it->second->getType()) {
				case MaterialSettingType::FLOAT:
					glUniform1f(loc, it->second->getFloat());
					break;
				case MaterialSettingType::VEC2:
					_v2 = it->second->getVector2();
					glUniform2f(loc, _v2.x, _v2.y);
					break;
				case MaterialSettingType::VEC3:
					_v3 = it->second->getVector3();
					glUniform3f(loc, _v3.x, _v3.y, _v3.z);
					break;
				case MaterialSettingType::VEC4:
					_v4 = it->second->getVector4();
					glUniform4f(loc, _v4.x, _v4.y, _v4.z, _v4.w);
					break;
				case MaterialSettingType::MAT4:
					_m4 = it->second->getMat4();
					glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(_m4));
					break;
				}
			}
		}

		/*
		GLint loc_model = glGetUniformLocation(default_sp.getId(), "model");
		glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(renderable.getTransform()));
		GLint loc_view = glGetUniformLocation(default_sp.getId(), "view");
		glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(view));
		GLint loc_projection = glGetUniformLocation(default_sp.getId(), "projection");
		glUniformMatrix4fv(loc_projection, 1, GL_FALSE, glm::value_ptr(projection));
		GLint loc_time = glGetUniformLocation(default_sp.getId(), "time");
		glUniform1f(loc_time, time);
		*/
		//sp.unbind();
	}
};

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

class OpenGlMesh : public Mesh {
public:
	OpenGlMesh() {
		VBO = 0;
		VAO = 0;
		EBO = 0;
	}

	~OpenGlMesh() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void draw() override {
		if (data.getAttributeNames().size() == 0) return;
		// Delete old buffers
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &EBO);
		// Generate geometry (horribly inefficient)
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Reserve space
		int total_bytes = 0;
		std::vector<std::string> attributes = data.getAttributeNames();
		for (auto it = attributes.begin(); it != attributes.end(); ++it) {
			switch (data.getAttribute(*it)->getType()) {
			case VertexAttributeType::FLOAT:
				total_bytes += sizeof(float) * data.getAttribute(*it)->getFloat().size();
				break;
			case VertexAttributeType::DOUBLE:
				total_bytes += sizeof(double) * data.getAttribute(*it)->getDouble().size();
				break;
			case VertexAttributeType::UINT:
				total_bytes += sizeof(uint32_t) * data.getAttribute(*it)->getUint().size();
				break;
			default:
				throw std::runtime_error("Unsupported attribute type.");
			}
		}
		glBufferData(GL_ARRAY_BUFFER, total_bytes, NULL, GL_STATIC_DRAW);
		// Fill in data
		std::map<std::string, int> offsets;
		int offset = 0;
		for (auto it = attributes.begin(); it != attributes.end(); ++it) {
			int size = 0;
			void *d = NULL;
			switch (data.getAttribute(*it)->getType()) {
			case VertexAttributeType::FLOAT:
				size = sizeof(float)*data.getAttribute(*it)->getFloat().size();
				d = data.getAttribute(*it)->getFloat().data();
				break;
			case VertexAttributeType::DOUBLE:
				size = sizeof(double)*data.getAttribute(*it)->getDouble().size();
				d = data.getAttribute(*it)->getDouble().data();
				break;
			case VertexAttributeType::UINT:
				size = sizeof(uint32_t)*data.getAttribute(*it)->getUint().size();
				d = data.getAttribute(*it)->getUint().data();
				break;
			}
			offsets[*it] = offset;
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, d);
		}
		// Fill in indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
			(sizeof(uint32_t)*data.getIndices().size()),
			data.getIndices().data(), 
			GL_STATIC_DRAW);
		// Setup attributes
		BaseVertexAttribute *attr = NULL;
		std::vector<std::string> atts;
		atts.assign({ "position","normal","color","tex_coord" });
		int counter = 0;
		for (auto it = atts.begin(); it != atts.end(); ++it) {
			if ((attr = data.getAttribute(*it)) != NULL) {
				int type = GL_FLOAT;
				switch (attr->getType()) {
				case VertexAttributeType::FLOAT:
					type = GL_FLOAT;
					break;
				case VertexAttributeType::DOUBLE:
					type = GL_DOUBLE;
					break;
				case VertexAttributeType::UINT:
					type = GL_UNSIGNED_INT;
					break;
				}
				glVertexAttribPointer(counter,
					attr->getComponents(),
					GL_FLOAT,
					type,
					0,
					&offsets[*it]);
				glEnableVertexAttribArray(counter);
				++counter;
			}
		}
		glBindVertexArray(0); // Unbind VAO
		// Draw operations
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

protected:
	GLuint VBO, VAO, EBO;
};

class OpenGlQuad : public Quad {
public:
	OpenGlQuad() : Quad() {
		VAO = 0;
		VBO = 0;
		EBO = 0;
	}

	~OpenGlQuad() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void draw() override {
		if (data.getAttributeNames().size() == 0) return;
		// Delete old buffers
		if(VBO != 0) glDeleteBuffers(1, &VBO);
		if(VAO != 0) glDeleteVertexArrays(1, &VAO);
		if(EBO != 0) glDeleteBuffers(1, &EBO);
		// Generate geometry (horribly inefficient)
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Reserve space
		int total_bytes = 0;
		std::vector<std::string> attributes = data.getAttributeNames();
		for (auto it = attributes.begin(); it != attributes.end(); ++it) {
			switch (data.getAttribute(*it)->getType()) {
			case VertexAttributeType::FLOAT:
				total_bytes += sizeof(float) * data.getAttribute(*it)->getFloat().size();
				break;
			case VertexAttributeType::DOUBLE:
				total_bytes += sizeof(double) * data.getAttribute(*it)->getDouble().size();
				break;
			case VertexAttributeType::UINT:
				total_bytes += sizeof(uint32_t) * data.getAttribute(*it)->getUint().size();
				break;
			default:
				throw std::runtime_error("Unsupported attribute type.");
			}
		}
		glBufferData(GL_ARRAY_BUFFER, total_bytes, NULL, GL_STATIC_DRAW);
		// Fill in data
		std::map<std::string, int> offsets;
		int offset = 0;
		for (auto it = attributes.begin(); it != attributes.end(); ++it) {
			int size = 0;
			void *d = NULL;
			switch (data.getAttribute(*it)->getType()) {
			case VertexAttributeType::FLOAT:
				size = sizeof(float)*data.getAttribute(*it)->getFloat().size();
				d = data.getAttribute(*it)->getFloat().data();
				break;
			case VertexAttributeType::DOUBLE:
				size = sizeof(double)*data.getAttribute(*it)->getDouble().size();
				d = data.getAttribute(*it)->getDouble().data();
				break;
			case VertexAttributeType::UINT:
				size = sizeof(uint32_t)*data.getAttribute(*it)->getUint().size();
				d = data.getAttribute(*it)->getUint().data();
				break;
			}
			offsets[*it] = offset;
			glBufferSubData(GL_ARRAY_BUFFER, offset, size, d);
			offset += size;
		}
		// Fill in indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			(sizeof(uint32_t)*data.getIndices().size()),
			data.getIndices().data(),
			GL_STATIC_DRAW);
		// Setup attributes
		BaseVertexAttribute *attr = NULL;
		std::vector<std::string> atts;
		atts.assign({ "position","normal","color","tex_coord" });
		int counter = 0;
		for (auto it = atts.begin(); it != atts.end(); ++it) {
			if ((attr = data.getAttribute(*it)) != NULL) {
				switch (attr->getType()) {
				case VertexAttributeType::FLOAT:
					glVertexAttribPointer(counter,
						attr->getComponents(),
						GL_FLOAT,
						GL_FALSE,
						0,
						(GLvoid*)(offsets[*it]));
						//(GLvoid*)(offsets[*it] * sizeof(float)));
					glEnableVertexAttribArray(counter);
					break;
				case VertexAttributeType::DOUBLE:
					glVertexAttribPointer(counter,
						attr->getComponents(),
						GL_DOUBLE,
						GL_FALSE,
						0,
						(GLvoid*)(offsets[*it]));
						//(GLvoid*)(offsets[*it] * sizeof(double)));
					glEnableVertexAttribArray(counter);
					break;
				case VertexAttributeType::UINT:
					glVertexAttribPointer(counter,
						attr->getComponents(),
						GL_UNSIGNED_INT,
						GL_FALSE,
						0,
						(GLvoid*)(offsets[*it]));
						//(GLvoid*)(offsets[*it] * sizeof(uint32_t)));
					glEnableVertexAttribArray(counter);
					break;
				}
			}
			++counter;
		}
		glBindVertexArray(0); // Unbind VAO
							  // Draw operations
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

protected:
	GLuint VBO, VAO, EBO;
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

	Material* createMaterial(Shader* vs, Shader* fs) {
		return new OpenGlMaterial(vs, fs);
	}

	Quad* createQuad() {
		return new OpenGlQuad();
	}

protected:
	OpenGLRenderer &renderer;
};

class OpenGLRenderer : public Renderer {
public:
	OpenGLRenderer(Shader frag_shader, Shader vert_shader) : fact(*this) { 
		default_sp.addShader(&frag_shader);
		default_sp.addShader(&vert_shader);
		if (!default_sp.compile()) {
			throw std::runtime_error("Bad shaders");
		}
	}

	void setView(glm::mat4 view) {
		this->view = view;
	}

	void setProjection(glm::mat4 projection) {
		this->projection = projection;
	}

	void setTime(GLfloat time) {
		this->time = time;
	}

	RendererFactory& getFactory() {
		return this->fact;
	}

	void draw(Renderable& renderable) {
		//default_sp.bind();
		// Set shader params
		
		/*
		GLint loc_model = glGetUniformLocation(default_sp.getId(), "model");
		glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(renderable.getTransform()));
		GLint loc_view = glGetUniformLocation(default_sp.getId(), "view");
		glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(view));
		GLint loc_projection = glGetUniformLocation(default_sp.getId(), "projection");
		glUniformMatrix4fv(loc_projection, 1, GL_FALSE, glm::value_ptr(projection));
		GLint loc_time = glGetUniformLocation(default_sp.getId(), "time");
		glUniform1f(loc_time, time);
		*/

		// Draw stuff
		renderable.getMaterial()->activate();
		renderable.draw();
		//default_sp.unbind();
	}

protected:
	GLfloat time;
	glm::mat4 view;
	glm::mat4 projection;
	ShaderProgram default_sp;
	OpenGLRendererFactory fact;
};

} // opengl
} // graphics
} // tator

#endif