// Andrew Black
// March 18, 2017
// Abstraction for the underlying rendering engine

#ifndef _HGUARD_TATOR_GRAPHICS_RENDERER_HPP
#define _HGUARD_TATOR_GRAPHICS_RENDERER_HPP

#include <stdint.h>

#include <vector>
#include <map>
#include <fstream>
#include "glm/glm.hpp"
#include "gl.hpp"
#include "tator/system/TatorException.hpp"
using tator::system::TatorException;

namespace tator {
namespace graphics {

class GameTime {
public:

};

class Component;

class GameObject {
public:
	enum class State : int { ACTIVE = 0, DISABLED = 1 };
	virtual void update(GameTime& gt) { }

	void addComponent(Component* component) {
		this->components.push_back(component);
	}

protected:
	GameObject* parent;
	std::vector<GameObject*> children;
	std::vector<Component*> components;
};

class Component {
public:
	virtual bool isUnique() = 0;
	virtual void update() { }
	virtual void draw() { }
	GameObject* getOwner() { return this->owner; }
protected:
	GameObject *owner;
};

class TransformComponent : public Component {
public:
	TransformComponent(GameObject *owner, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}

	glm::vec3 getPosition() { return this->position; }
	glm::vec3 getRotation() { return this->rotation; }
	glm::vec3 getScale() { return this->scale; }

protected:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale; 

};

class IRendererResource {
public:
	uint32_t getRendererResourceId() { return renderer_resource_id; }
	void setRendererResourceId(uint32_t id) { renderer_resource_id = id; }

protected:
	uint32_t renderer_resource_id = 0;
};

enum class TextureFormat : int { RGB = 0, RGBA = 1 };
enum class TextureWrap : int { REPEAT = 0 };
enum class TextureInterpolation : int { LINEAR = 0 };

// Represents a standard texture
class Texture2D : public IRendererResource {
public:
	virtual ~Texture2D() { }

	virtual void prepare() = 0;
	virtual void cleanup() = 0;
protected:
	int width, height;
	std::string data;
	TextureFormat format;
	TextureWrap wrap_s, wrap_t;
	TextureInterpolation min_filter, mag_filter;
};

class Shader {
public:
	enum class Type { OPENGL, DIRECTX };
	Shader(Type type) {
		this->type = type;
	}
	virtual ~Shader() { }
protected:
	Type type;
};

class ShaderOpenGL : public Shader, public GlObject {
public:
	enum class GlShaderType : GLenum { VERTEX=GL_VERTEX_SHADER, FRAGMENT=GL_FRAGMENT_SHADER, GEOMETRY=GL_GEOMETRY_SHADER };
	ShaderOpenGL(GlShaderType type, std::string code) 
			: Shader(Type::OPENGL), GlObject() {
		this->type = type;
		this->code = code;
		this->compile();
	}

	void destroy() override {
		if (this->hasId()) {
			glDeleteShader(this->getId());
			GlObject::destroy();
		}
	}

protected:
	void compile() {
		// Compile shader
		GLuint _id = glCreateShader((GLenum)this->type);
		const char *c = this->code.c_str();
		glShaderSource(_id, 1, &c, NULL);
		glCompileShader(_id);
		// Validate
		GLint success;
		glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
		if (success == GL_TRUE) {
			this->setId(_id);
		}
		else {
			// Get error details
			GLchar info[512];
			glGetShaderInfoLog(_id, 512, NULL, info);
			glDeleteShader(_id);
			throw std::runtime_error("Unable to compile shader: " + std::string(info));
		}
	}

	GlShaderType type;
	std::string code;
};

class ShaderOpenGLFactory {
public:

	static ShaderOpenGL fragmentShaderFromFile(std::string file_path) {
		return shaderFromFile(ShaderOpenGL::GlShaderType::FRAGMENT, file_path);
	}
	static ShaderOpenGL vertexShaderFromFile(std::string file_path) {
		return shaderFromFile(ShaderOpenGL::GlShaderType::VERTEX, file_path);
	}

	static ShaderOpenGL shaderFromFile(ShaderOpenGL::GlShaderType type, std::string file_path) {
		std::ifstream input(file_path);
		if (input.is_open()) {
			std::string src((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
			return ShaderOpenGL(type, src);
		}
		else {
			throw TatorException("Unable to open file " + file_path);
		}
	}
private:
	ShaderOpenGLFactory() { }
};

class MaterialOption {
public:
	enum class Type { UNKNOWN, STRING, FLOAT, DOUBLE, VEC2, VEC3, VEC4, TEXTURE2D };
	MaterialOption(Type type, std::string name) {
		this->type = type;
		this->name = name;
	}
protected:
	Type type;
	std::string name;
};

class MaterialOptionString : public MaterialOption {
public:
	MaterialOptionString(std::string name, std::string value)
			: MaterialOption(Type::STRING, name) {
		this->value = value;
	}
protected:
	std::string value;
};

class MaterialOptionTexture2D : public MaterialOption {
public:
	MaterialOptionTexture2D(std::string name, Texture2D* value)
			: MaterialOption(Type::TEXTURE2D, name) {
		this->value = value;
	}
protected:
	Texture2D *value;
};

class Material : public IRendererResource {
public:
	virtual ~Material() { }

	MaterialOption& getOption(std::string name) {
		if (options.count(name) == 0) {
			throw std::runtime_error("Key not found in options. " + name);
		}
		else {
			return options[name];
		}
	}

	void setOption(std::string name, MaterialOption* option) {
		options[name] = *option;
	}

protected:
	std::map<std::string, MaterialOption> options;
};

class TexturedQuadComponent : public Component {
public:
	TexturedQuadComponent(GameObject* owner, Texture2D* texture) {
		this->owner = owner;
		this->texture = texture;
	}

	bool isUnique() override {
		return true;
	}
protected:
	Texture2D *texture;
};


class RendererFactory {
public:
	virtual Texture2D* createTexture(std::string data,
		int width, int height,
		TextureFormat format, TextureWrap wrap_s,
		TextureWrap wrap_t, TextureInterpolation min_interp,
		TextureInterpolation mag_interp) = 0;
};

class Renderer {
public:
	virtual RendererFactory& getFactory() = 0;
};

} // graphics
} // tator

#endif