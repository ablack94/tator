// Andrew Black
// July 04, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_IMPL_OPENGL_OPENGLMATERIAL_HPP
#define HG_TATOR_GRAPHICS_IMPL_OPENGL_OPENGLMATERIAL_HPP

#include "tator/graphics/material/Material.hpp"
using tator::graphics::material::Material;

#include "tator/graphics/material/BaseMaterialSetting.hpp"
using tator::graphics::material::BaseMaterialSetting;

#include "tator/graphics/material/NumericMaterialSetting.hpp"
using tator::graphics::material::BaseNumericMaterialSetting;

#include "tator/graphics/material/AbstractMaterialSettingDispatcher.hpp"
using tator::graphics::material::AbstractMaterialSettingDispatcher;

#include "detail/Shader.hpp"
using tator::graphics::impl::opengl::detail::Shader;

#include <memory>
#include <map>
#include <vector>

namespace tator {
namespace graphics {
namespace impl {
namespace opengl {
namespace detail {
	class ShaderProgram {
	public:
		ShaderProgram() {
			this->obj = nullptr;
		}
	
		ShaderProgram(std::vector<Shader> shaders) {
			this->obj = nullptr;
			// Create shader program
			GLuint id = glCreateProgram();
			for (auto it=shaders.begin();it!=shaders.end();++it) {
				glAttachShader(id, it->getId());
			}
			glLinkProgram(id);
			// Validate
			GLint success;
			glGetProgramiv(id, GL_LINK_STATUS, &success);
			if (success == GL_TRUE) {
				obj = std::unique_ptr<OpenGLObject>(new OpenGLObject(id, [](GLuint id) {glDeleteProgram(id); }));
			}
			else {
				GLchar info[512];
				glGetProgramInfoLog(id, 512, NULL, info);
				glDeleteProgram(id);
				throw std::exception(std::string(info).c_str());
			}
		}

		GLuint getId() {
			if (obj != nullptr)
				return obj->getId();
			else
				throw std::exception("No opengl object bound!");
		}

	protected:
		std::unique_ptr<OpenGLObject> obj;
	};
} // detail

class OpenGLMaterialSettingDispatcher : public AbstractMaterialSettingDispatcher {
public:
	GLuint id;
	std::string name;
	void dispatch(BaseNumericMaterialSetting &setting) override {
		auto details = setting.getDetails();
		GLint loc = glGetUniformLocation(id, name.c_str()); // TODO: Make sure loc is valid
		setUniform(loc, details.isSigned, details.isIntegral, details.element_size, details.components, details.buffer);
	}
protected:
	void setUniform(GLint loc, bool is_signed, bool is_integral, size_t type_size, size_t components, void* data) {
		if (is_integral) {
			if (is_signed)
				if (type_size == sizeof(int32_t))
					setUniformInt(loc, components, data);
				else
					throw std::exception("Only 32 bit signed integer material settings are supported!");
			else
				if (type_size == sizeof(uint32_t))
					setUniformUint(loc, components, data);
				else
					throw std::exception("Only 32 bit unsigned integer material settings are supported!");
		}
		else {
			if (is_signed)
				if (type_size == sizeof(float))
					setUniformFloat(loc, components, data);
				else
					throw std::exception("Only 32 bit float material settings are supported!");
			else
				throw std::exception("Unsigned float material settings not supported!");
		}
	}

	void setUniformFloat(GLint loc, size_t size, void* data) {
		if (uniforms_float.count(size) != 0)
			uniforms_float[size](loc, size, data);
		else
			throw std::exception("Setting uniform float material setting doesn't support given vector length");
	}

	void setUniformInt(GLint loc, size_t size, void* data) {
		if (uniforms_int.count(size) != 0)
			uniforms_int[size](loc, size, data);
		else
			throw std::exception("Setting uniform int material setting doesn't support given vector length");
	}

	void setUniformUint(GLint loc, size_t size, void* data) {
		if (uniforms_uint.count(size) != 0)
			uniforms_uint[size](loc, size, data);
		else
			throw std::exception("Setting uniform uint material setting doesn't support given vector length");
	}

	typedef void(*setUniformFP)(GLint loc, size_t size, void* data);
	std::map<size_t, setUniformFP> uniforms_float = {
		{ (size_t)1, [](GLint loc, size_t size, void* data) {glUniform1fv(loc, size, (float*)data); } },
		{ (size_t)2, [](GLint loc, size_t size, void* data) {glUniform2fv(loc, size, (float*)data); } },
		{ (size_t)3, [](GLint loc, size_t size, void* data) {glUniform3fv(loc, size, (float*)data); } },
		{ (size_t)4, [](GLint loc, size_t size, void* data) {glUniform4fv(loc, size, (float*)data); } }
	};
	std::map<size_t, setUniformFP> uniforms_int = {
		{ (size_t)1, [](GLint loc, size_t size, void* data) {glUniform1iv(loc, size, (int32_t*)data); } },
		{ (size_t)2, [](GLint loc, size_t size, void* data) {glUniform2iv(loc, size, (int32_t*)data); } },
		{ (size_t)3, [](GLint loc, size_t size, void* data) {glUniform3iv(loc, size, (int32_t*)data); } },
		{ (size_t)4, [](GLint loc, size_t size, void* data) {glUniform4iv(loc, size, (int32_t*)data); } }
	};
	std::map<size_t, setUniformFP> uniforms_uint = {
		{ (size_t)1, [](GLint loc, size_t size, void* data) {glUniform1uiv(loc, size, (uint32_t*)data); } },
		{ (size_t)2, [](GLint loc, size_t size, void* data) {glUniform2uiv(loc, size, (uint32_t*)data); } },
		{ (size_t)3, [](GLint loc, size_t size, void* data) {glUniform3uiv(loc, size, (uint32_t*)data); } },
		{ (size_t)4, [](GLint loc, size_t size, void* data) {glUniform4uiv(loc, size, (uint32_t*)data); } }
	};
};

class OpenGLMaterial : public Material {
public:
	OpenGLMaterial(detail::ShaderProgram sp) {
		this->sp = std::move(sp);
	}

	void activate() override {
		glUseProgram(sp.getId());
		// Activate settings
		OpenGLMaterialSettingDispatcher dispatcher;
		dispatcher.id = sp.getId();
		for (auto it = settings.begin(); it != settings.end(); ++it) {
			dispatcher.name = it->first;
			it->second->accept(dispatcher);
		}
	}

	void deactivate() override {
		glUseProgram(0);
	}

	void setProperty(std::string name, std::unique_ptr<BaseMaterialSetting> setting) override {
		settings[name] = std::move(setting);
	}

	BaseMaterialSetting* getProperty(std::string name) override {
		if (settings.count(name) != 0)
			return settings[name].get();
		else
			throw std::exception("Property does not exist!");
	}

protected:
	detail::ShaderProgram sp;
	std::map<std::string, std::unique_ptr<BaseMaterialSetting>> settings;
	
};

} // opengl
} // impl
} // graphics
} // tator

#endif