// Andrew Black
// July 02, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_DATA_VERTEXATTRIBUTEINFO_HPP
#define HG_TATOR_GRAPHICS_DATA_VERTEXATTRIBUTEINFO_HPP

#include <array>

namespace tator {
namespace graphics {
namespace data {

class BaseVertexAttributeInfo {
public:
	BaseVertexAttributeInfo() {
		this->type_size = 0;
		this->is_signed = false;
		this->is_integral = false;
		this->total_size = 0;
		this->dim_x = 0;
		this->dim_y = 0;
	}
	
	BaseVertexAttributeInfo(size_t type_size, bool is_signed, bool is_integral, size_t dim_x, size_t dim_y) {
		this->type_size = type_size;
		this->is_signed = is_signed;
		this->is_integral = is_integral;
		this->total_size = (type_size * (dim_x * dim_y));
		this->dim_x = dim_x;
		this->dim_y = dim_y;
	}

	template <typename T, size_t x, size_t y>
	static std::unique_ptr<BaseVertexAttributeInfo> createVertexAttributeInfo() {
		return std::make_unique<BaseVertexAttributeInfo>(sizeof(T), std::is_signed<T>(), std::is_integral<T>(), x, y);
	}

	bool isTypeSigned() { return is_signed; }
	bool isTypeIntegral() { return is_integral; }
	size_t getTypeSize() { return type_size; }
	size_t getTotalSize() { return total_size; }
	std::array<size_t, 2> getDimensions() { return std::array<size_t, 2>{dim_x, dim_y}; }

protected:
	bool is_signed;
	bool is_integral;
	size_t type_size;
	size_t total_size;
	size_t dim_x, dim_y; // x dimensions, y dimensions
};

template <typename T, size_t x, size_t y>
class VertexAttributeInfo : public BaseVertexAttributeInfo {
public:
	typedef T value_type;

	VertexAttributeInfo() :
		BaseVertexAttributeInfo(sizeof(T), std::is_signed<T>(), std::is_integral<T>(), x, y) { }
};

} // data
} // graphics
} // tator
#endif