// Andrew Black
// July 02, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_MATERIALFACTORY_HPP
#define HG_TATOR_GRAPHICS_MATERIALFACTORY_HPP

#include <memory>

#include "tator/graphics/material/Material.hpp"
using tator::graphics::material::Material;

namespace tator {
namespace graphics {
namespace material {

class AbstractMaterialFactory {
public:
	AbstractMaterialFactory() = default;
	virtual ~AbstractMaterialFactory() = default;
	virtual std::shared_ptr<Material> getDefaultMaterial() = 0;
};

} // material
} // graphics
} // tator

#endif