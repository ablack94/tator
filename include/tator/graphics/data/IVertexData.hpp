// Andrew Black
// July 02, 2017

#pragma once
#ifndef HG_TATOR_GRAPHICS_IVERTEXDATA_HPP
#define HG_TATOR_GRAPHICS_IVERTEXDATA_HPP

#include <vector>
#include <array>
#include <memory>

#include "tator/graphics/data/VertexAttribute.hpp"
using tator::graphics::data::BaseVertexAttribute;

#include "tator/graphics/data/VertexDataHint.hpp"
using tator::graphics::data::VertexDataHint;

namespace tator {
namespace graphics {
namespace data {

class IVertexData {
public:
	virtual VertexDataHint getVertexDataHint() = 0;
	virtual std::vector<BaseVertexAttribute*> getAttributes() = 0;
	virtual std::vector<char> getAttributeData(size_t idx) = 0;
	virtual std::vector<std::vector<char>> getAttributesData() {
		std::vector<std::vector<char>> data;
		auto v = getAttributes();
		for (size_t i = 0; i < v.size(); ++i) {
			data.push_back(getAttributeData(i));
		}
		return data;
	}
};

} // data
} // graphics
} // tator


#endif