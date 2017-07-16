// Andrew Black
// July 02, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_MATERIAL_HPP
#define HG_TATOR_GRAPHICS_MATERIAL_HPP

#include "tator/graphics/material/BaseMaterialSetting.hpp"
using tator::graphics::material::BaseMaterialSetting;

#include <memory>

namespace tator {
namespace graphics {
namespace material {

class Material {
public:
	Material() = default;
	virtual ~Material() = default;

	virtual void activate() = 0;
	virtual void deactivate() = 0;

	template <typename T, typename ...Args>
	void setProperty(std::string name, Args... args) {
		setProperty(name, std::make_unique<BaseMaterialSetting>(std::forward<Args>(...args)));
	}

	virtual void setProperty(std::string name, std::unique_ptr<BaseMaterialSetting> setting) = 0;
	virtual BaseMaterialSetting* getProperty(std::string name) = 0;
};

} // material
} // graphics
} // tator

#endif