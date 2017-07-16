// Andrew Black
// July 02, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_DATA_VERTEXDATA_HPP
#define HG_TATOR_GRAPHICS_DATA_VERTEXDATA_HPP

#include <vector>
#include <memory>

#include "tator/graphics/data/IVertexData.hpp"
using tator::graphics::data::IVertexData;

#include "tator/graphics/data/VertexDataHint.hpp"
using tator::graphics::data::VertexDataHint;

namespace tator {
namespace graphics {
namespace data {

class VertexData : public IVertexData {
public:
	VertexData() {
		this->hint = VertexDataHint::STATIC;
	}

	VertexData(VertexDataHint hint) {
		this->hint = hint;
	}

	template <typename T, size_t x, size_t y>
	VertexAttribute<T, x, y>* addAttribute() {
		return addAttribute<T, x, y>("");
	}

	template <typename T, size_t x, size_t y>
	VertexAttribute<T, x, y>* addAttribute(std::string name) {
		std::unique_ptr<VertexAttribute<T, x, y>> temp = std::make_unique<VertexAttribute<T, x, y>>(name);
		//std::unique_ptr<VertexAttribute<T, x, y>> temp(new VertexAttribute<T, x, y>(name));
		VertexAttribute<T, x, y>* ptr = temp.get();
		attributes.push_back(std::move(temp));
		return ptr;
	}

	bool isValid() {
		if (attributes.size() == 0) return true;
		size_t len, prev = attributes[0]->getLength();
		for (size_t i = 1; i < attributes.size(); ++i) {
			len = attributes[i]->getLength();
			if (prev != len) return false;
			prev = len;
		}
		return true;
	}

	VertexDataHint getVertexDataHint() override {
		return hint;
	}

	std::vector<BaseVertexAttribute*> getAttributes()  override {
		std::vector<BaseVertexAttribute*> temp(attributes.size());
		for (size_t i = 0; i < temp.size(); ++i) {
			temp[i] = attributes[i].get();
		}
		return temp;
	}

	std::vector<char> getAttributeData(size_t idx) override {
		return attributes[idx]->getData();
	}

	std::vector<std::vector<char>> getAttributesData() override {
		std::vector<std::vector<char>> temp(attributes.size());
		for (size_t i = 0; i < temp.size(); ++i) {
			temp.push_back(getAttributeData(i));
		}
		return temp;
	}

protected:
	VertexDataHint hint;
	std::vector<std::unique_ptr<BaseVertexAttribute>> attributes;
};

} // data
} // graphics
} // tator

#endif