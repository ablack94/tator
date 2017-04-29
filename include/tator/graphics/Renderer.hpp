// Andrew Black
// March 18, 2017
// Abstraction for the underlying rendering engine

#ifndef _HGUARD_TATOR_GRAPHICS_RENDERER_HPP
#define _HGUARD_TATOR_GRAPHICS_RENDERER_HPP

#include <stdint.h>

#include <vector>
#include <map>
#include <memory>
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

enum class VertexAttributeType {
	FLOAT, DOUBLE, UINT,
};

class BaseVertexAttribute {
public:
	BaseVertexAttribute(VertexAttributeType type, int components) {
		this->type = type;
		this->components = components;
	}

	int getIndex(int vertex) {
		return vertex * this->components;
	}

	int getIndex(int vertex, int component) {
		// TODO: Should make sure component isn't greater than count
		//	Will have a performance impact though, not sure if it's worth it.
		return this->getIndex(vertex) + component;
	}

	int getComponents() {
		return components;
	}

	VertexAttributeType getType() {
		return type;
	}

	virtual std::vector<float>& getFloat() {
		throw std::runtime_error("VertexAttribute is not of type float");
	}
	virtual std::vector<double>& getDouble() {
		throw std::runtime_error("VertexAttribute is not of type double");
	}
	virtual std::vector<uint32_t>& getUint() {
		throw std::runtime_error("VertexAttribute is not of type uint32_t");
	}

protected:
	VertexAttributeType type;	
	int components;
};

template <typename T>
class VertexAttribute : public BaseVertexAttribute {
private:
	VertexAttribute() : BaseVertexAttribute(0, 0) { }
};

template <>
class VertexAttribute<float> : public BaseVertexAttribute {
public:
	VertexAttribute(int components)
		: BaseVertexAttribute(VertexAttributeType::FLOAT, components) { }

	std::vector<float>& getFloat() { return data; }
	void set(int start_index, std::vector<float> values) {
		auto data_it = data.begin() + start_index;
		auto input_it = values.begin();
		while (data_it != data.end() && input_it != values.end()) {
			*(data_it++) = *(input_it++);
		}
		if (data_it != data.end() || input_it != values.end()) {
			throw std::runtime_error("Invalid input size.");
		}
	}
protected:
	std::vector<float> data;
};

template <>
class VertexAttribute<double> : public BaseVertexAttribute {
public:
	VertexAttribute(int components)
		: BaseVertexAttribute(VertexAttributeType::DOUBLE, components) { }

	std::vector<double>& getDouble() { return data; }
protected:
	std::vector<double> data;
};

template <>
class VertexAttribute<uint32_t> : public BaseVertexAttribute {
public:
	VertexAttribute(int components)
		: BaseVertexAttribute(VertexAttributeType::UINT, components) { }

	std::vector<uint32_t>& getUint() { return data; }
protected:
	std::vector<uint32_t> data;
};

class VertexData {
public:
	VertexData() { }

	template <typename T>
	VertexAttribute<T>* addAttribute(std::string name, int components) {
		if (this->attributes.count(name) == 0) {
			auto v = new VertexAttribute<T>(components);
			attributes[name] = v;
			return v;
		}
		else {
			throw std::runtime_error("VertexData attribute with name '" + name + "' already exists.");
		}
	}

	std::vector<std::string> getAttributeNames() {
		std::vector<std::string> keys;
		for (auto it = attributes.begin(); it != attributes.end(); ++it) {
			keys.push_back(it->first);
		}
		return keys;
	}

	BaseVertexAttribute* getAttribute(std::string name) {
		if (attributes.count(name) != 0) {
			return attributes[name];
		}
		else {
			return NULL;
		}
	}

	std::vector<uint32_t>& getIndices() {
		return this->indices;
	}

protected:
	std::map<std::string, BaseVertexAttribute*> attributes;
	std::vector<float> data;
	std::vector<uint32_t> indices;
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

class Renderable {
public:
	Renderable() {
		this->material = NULL; // TODO: default material
	}

	Renderable(Material* mat) {
		this->material = mat;
	}

	virtual glm::mat4 getTransform() {
		return this->transform;
	}

	virtual void setTransform(glm::mat4 m) {
		this->transform = m;
	}

	virtual Material* getMaterial() {
		return this->material;
	}

	virtual void setMaterial(Material* mat) {
		this->material = mat;
	}

	virtual void draw() = 0;

protected:
	glm::mat4 transform;
	Material* material;
};

class Mesh : public Renderable {
public:
	Mesh() : Renderable() {
	}

	VertexData& getData() { return data; }

protected:
	VertexData data;
};

class Quad : public Mesh {
public:
	Quad() : Mesh(*this) {
		auto va = data.addAttribute<float>("position", 3);
		va->getFloat().assign({
			0.5f, 0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f });
		data.getIndices().assign({ 0, 1, 3, 1, 2, 3 });
	}

	Quad(Material* material) {
		this->material = material;
	}

	virtual void draw() = 0;

protected:
	glm::mat4 transform;
	Material* material;
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
	virtual void draw(Renderable& renderable) = 0;
};

} // graphics
} // tator

#endif