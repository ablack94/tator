// Andrew Black
// July 04, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_IRENDERABLE_HPP
#define HG_TATOR_GRAPHICS_IRENDERABLE_HPP

#include "tator/graphics/material/Material.hpp"
using tator::graphics::material::Material;

#include "tator/graphics/data/IVertexData.hpp"
using tator::graphics::data::IVertexData;

namespace tator {
namespace graphics {

class IRenderable {
public:
	virtual IVertexData* getVertexData() = 0;
	virtual Material* getMaterial() = 0;
};

class Renderable : public IRenderable {
public:
	Renderable(IVertexData* data, Material* material) {
		this->data = data;
		this->material = material;
	}

	IVertexData* getVertexData() override { return data; }
	Material* getMaterial() override { return material; }

protected:
	IVertexData *data;
	Material* material;
};

} // graphics
} // tator

#endif