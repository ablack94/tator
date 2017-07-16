// Andrew Black
// July 02, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_DATA_VERTEXATTRIBUTE_HPP
#define HG_TATOR_GRAPHICS_DATA_VERTEXATTRIBUTE_HPP

#include <string>
#include <memory>

#include "tator/graphics/data/VertexAttributeInfo.hpp"
using tator::graphics::data::BaseVertexAttributeInfo;
using tator::graphics::data::VertexAttributeInfo;

namespace tator {
namespace graphics {
namespace data {

class BaseVertexAttribute {
public:
	BaseVertexAttribute(std::string name, BaseVertexAttributeInfo info) {
		this->name = name;
		this->attribute_info = info;
	}

	std::string getName() { return name; }
	BaseVertexAttributeInfo& getAttributeInfo() {
		return attribute_info;
	}
	virtual std::vector<char> getData() = 0;
	virtual size_t getLength() = 0;

protected:
	std::string name;
	BaseVertexAttributeInfo attribute_info;
};

template <typename T, size_t x, size_t y>
class VertexAttribute : public BaseVertexAttribute {
public:
	typedef T value_type;
	typedef std::array<T, x*y> row_type;

	VertexAttribute() :
		BaseVertexAttribute("", VertexAttributeInfo<T, x, y>()) { }

	VertexAttribute(std::string name) :
		BaseVertexAttribute(name, VertexAttributeInfo<T, x, y>()) { }

	void addRow(row_type row) {
		rows.push_back(row);
	}

	std::vector<char> getData() {
		size_t width = sizeof(T) * (x*y);
		std::vector<char> raw_data(getLength() * width);
		char* pos = raw_data.data();
		for (size_t i = 0; i < rows.size(); ++i) {
			memcpy(pos, rows[i].data(), width);
			pos += width;
		}
		return raw_data;
	}

	size_t getLength() override {
		return rows.size();
	}

protected:
	std::vector<row_type> rows;
};

} // data
} // graphics
} // tator
#endif