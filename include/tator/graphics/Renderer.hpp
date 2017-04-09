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

#include "tator/graphics/ShaderProgram.hpp"
using tator::graphics::ShaderProgram;

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
/*
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
*/
/*
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
*/


enum class MaterialSettingType : int {
	FLOAT, VEC2, VEC3, VEC4, MAT4, TEXTURE2D
};

class MaterialSetting {
public:
	virtual ~MaterialSetting() { }
	MaterialSettingType getType() {
		return type;
	}
	virtual float getFloat() { throw std::runtime_error("Not a float"); }
	virtual glm::vec2 getVector2() { throw std::runtime_error("Not a vector2"); }
	virtual glm::vec3 getVector3() { throw std::runtime_error("Not a vector3"); }
	virtual glm::vec4 getVector4() { throw std::runtime_error("Not a vector4"); }
	virtual glm::mat4 getMat4() { throw std::runtime_error("Not a mat4"); }
	virtual Texture2D* getTexture2D() { throw std::runtime_error("Not a texture2d"); }
protected:
	MaterialSetting(MaterialSettingType type) {
		this->type = type;
	}
	MaterialSettingType type;
};

class MaterialSettingFloat : public MaterialSetting {
public:
	MaterialSettingFloat(float value)
		: MaterialSetting(MaterialSettingType::FLOAT) {
		this->value = value;
	}
	float getFloat() override { return value; }
protected:
	float value;
};

class MaterialSettingVector2 : public MaterialSetting {
public:
	MaterialSettingVector2(glm::vec2 value)
		: MaterialSetting(MaterialSettingType::VEC2) {
		this->value = value;
	}
	glm::vec2 getVector2() override { return value; }
protected:
	glm::vec2 value;
};

class MaterialSettingVector3 : public MaterialSetting {
public:
	MaterialSettingVector3(glm::vec3 value)
		: MaterialSetting(MaterialSettingType::VEC3) {
		this->value = value;
	}
	glm::vec3 getVector3() override { return value; }
protected:
	glm::vec3 value;
};

class MaterialSettingVector4 : public MaterialSetting {
public:
	MaterialSettingVector4(glm::vec4 value)
		: MaterialSetting(MaterialSettingType::VEC4) {
		this->value = value;
	}
	glm::vec4 getVector4() override { return value; }
protected:
	glm::vec4 value;
};

class MaterialSettingMat4 : public MaterialSetting {
public:
	MaterialSettingMat4(glm::mat4 value)
		: MaterialSetting(MaterialSettingType::MAT4) {
		this->value = value;
	}
	glm::mat4 getMat4() override { return value; }
protected:
	glm::mat4 value;
};

class MaterialSettingTexture2D : public MaterialSetting {
public:
	MaterialSettingTexture2D(Texture2D* value)
		: MaterialSetting(MaterialSettingType::TEXTURE2D) {
		this->value = value;
	}
	Texture2D* getTexture2D() override { return value; }
protected:
	Texture2D* value;
};

class Material {
public:
	ShaderProgram& getShaderProgram() {
		return this->sp;
	}

	void setShaderProgram(ShaderProgram prog) {
		this->sp = prog;
	}

	void set(std::string name, float value) {
		set(name, new MaterialSettingFloat(value));
	}

	void set(std::string name, glm::vec2 value) {
		set(name, new MaterialSettingVector2(value));
	}

	void set(std::string name, glm::vec3 value) {
		set(name, new MaterialSettingVector3(value));
	}

	void set(std::string name, glm::vec4 value) {
		set(name, new MaterialSettingVector4(value));
	}

	void set(std::string name, glm::mat4 value) {
		set(name, new MaterialSettingMat4(value));
	}

	void set(std::string name, Texture2D* value) {
		set(name, new MaterialSettingTexture2D(value));
	}

	virtual void activate() = 0;

	std::map<std::string, MaterialSetting*>& getSettings() {
		return settings;
	}

protected:
	std::map<std::string, MaterialSetting*> settings;
	ShaderProgram sp;
	Material() { }

	void set(std::string name, MaterialSetting* ms) {
		if (settings.count(name) != 0) {
			delete settings[name];
		}
		settings[name] = ms;
	}
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

class IRenderable {
public:
	virtual glm::mat4 getTransform() = 0;
	virtual void setTransform(glm::mat4 m) = 0;
	virtual Material* getMaterial() { return NULL; }
	virtual void setMaterial(Material* mat) {}
	virtual void draw() = 0;
};

class IQuad {
public:
	virtual glm::mat4 getTransform() {
		return glm::mat4();
	}
	virtual void draw(IQuad& q) { }
};

class Quad : public IRenderable, public IQuad {
public:
	Quad(IQuad *implementor) {
		this->implementor = implementor;
		this->material = NULL;
	}

	Quad(IQuad *implementor, Material* material) {
		this->implementor = implementor;
		this->material = material;
	}

	glm::mat4 getTransform() override {
		return transform;
	}

	void setTransform(glm::mat4 m) override {
		transform = m;
	}

	Material* getMaterial() override {
		return material;
	}

	void setMaterial(Material* material) override {
		this->material = material;
	}

	void draw() override {
		implementor->draw(*this);
	}

protected:
	glm::mat4 transform;
	Material* material;
	IQuad *implementor;
};


class RendererFactory {
public:
	virtual Texture2D* createTexture(std::string data,
		int width, int height,
		TextureFormat format, TextureWrap wrap_s,
		TextureWrap wrap_t, TextureInterpolation min_interp,
		TextureInterpolation mag_interp) = 0;

	virtual Material* createMaterial(Shader* vs, Shader* fs) = 0;
	virtual Quad* createQuad() = 0;
};

class Renderer {
public:
	virtual RendererFactory& getFactory() = 0;
	virtual void draw(IRenderable& renderable) = 0;
};

} // graphics
} // tator

#endif