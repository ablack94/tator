// Andrew Black
// July 04, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_IMPL_OPENGL_OPENGLMATERIALFACTORY_HPP
#define HG_TATOR_GRAPHICS_IMPL_OPENGL_OPENGLMATERIALFACTORY_HPP

#include "tator/graphics/material/Material.hpp"
using tator::graphics::material::Material;

#include "tator/graphics/material/AbstractMaterialFactory.hpp"
using tator::graphics::material::AbstractMaterialFactory;

#include "OpenGLMaterial.hpp"
using tator::graphics::impl::opengl::OpenGLMaterial;
using tator::graphics::impl::opengl::detail::ShaderProgram;
using tator::graphics::impl::opengl::detail::Shader;

#include <memory>

namespace tator {
namespace graphics {
namespace impl {
namespace opengl {

class OpenGLMaterialFactory : public AbstractMaterialFactory {
public:
	OpenGLMaterialFactory() = default;
	std::shared_ptr<Material> getDefaultMaterial() override {
		std::string default_vs_code =
			"#version 330 core\n"
			"layout (location=0) in vec3 position;\n"
			"layout (location=1) in vec3 color;\n"
			"out vec3 frag_color;\n"
			"void main()\n"
			"{\n"
			"gl_Position = vec4(position, 1.0f);\n"
			"frag_color = color;\n"
			"}\n";
		Shader vs = Shader::createVertexShader(default_vs_code);
		std::string default_fs_code =
			"#version 330 core\n"
			"in vec3 frag_color;\n"
			"out vec3 color;\n"
			"void main()\n"
			"{\n"
			"color = frag_color;\n"
			"}\n";
		Shader fs = Shader::createFragmentShader(default_fs_code);
		std::vector<Shader> shaders;
			shaders.push_back(std::move(vs));
			shaders.push_back(std::move(fs));
		ShaderProgram prog(std::move(shaders));
		OpenGLMaterial mat(std::move(prog));
		return std::make_shared<OpenGLMaterial>(std::move(mat));
	}
	
	std::shared_ptr<Material> createMaterial(std::string vs_code, std::string fs_code) {
		Shader vs = Shader::createVertexShader(vs_code);
		Shader fs = Shader::createFragmentShader(fs_code);
		std::vector<Shader> shaders;
		shaders.push_back(std::move(vs));
		shaders.push_back(std::move(fs));
		ShaderProgram prog(std::move(shaders));
		OpenGLMaterial mat(std::move(prog));
		return std::make_shared<OpenGLMaterial>(std::move(mat));
	}

};

} // opengl
} // impl
} // graphics
} // tator
#endif