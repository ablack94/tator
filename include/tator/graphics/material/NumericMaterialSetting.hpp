// Andrew Black
// July 07, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_MATERIAL_NUMERICMATERIALSETTING_HPP
#define HG_TATOR_GRAPHICS_MATERIAL_NUMERICMATERIALSETTING_HPP

#include "BaseMaterialSetting.hpp"
using tator::graphics::material::BaseMaterialSetting;

#include "AbstractMaterialSettingDispatcher.hpp"
using tator::graphics::material::AbstractMaterialSettingDispatcher;

#include <string>
#include <sstream>

namespace tator {
namespace graphics {
namespace material {

class NumericMaterialSettingInfo {
public:
	size_t components;
	size_t element_size;
	size_t total_size; // components * element_size
	bool isSigned, isIntegral;
	void* buffer;

	std::string toString() {
		std::stringstream ss;
		ss << "{Components=" << components
			<< ",ElementSize=" << element_size
			<< ",TotalSize=" << total_size
			<< ",isSigned=" << isSigned
			<< ",IsIntegral=" << isIntegral
			<< ",Buffer=" << buffer
			<< "}";
		return ss.str();
	}
};

class BaseNumericMaterialSetting : public BaseMaterialSetting {
public:
	virtual NumericMaterialSettingInfo getDetails() = 0;
};

// Template for 1 or more components
template <typename T, size_t N = 1>
class NumericMaterialSetting : public BaseNumericMaterialSetting {
public:
	NumericMaterialSetting() {
		std::fill_n(data, N, T());
		this->details = {
			N, sizeof(T), (sizeof(T)*N),
			std::is_signed<T>(), std::is_integral<T>(),
			this->data.data()
		};
	}

	NumericMaterialSetting(std::array<T, N> data) {
		this->data = data;
		this->details = {
			N, sizeof(T), (sizeof(T)*N),
			std::is_signed<T>(), std::is_integral<T>(),
			this->data.data()
		};
	}

	NumericMaterialSettingInfo& getDetails() override {
		return details;
	}

	void accept(AbstractMaterialSettingDispatcher &dispatcher) override {
		dispatcher.dispatch(*this);
	}
protected:
	std::array<T, N> data;
	NumericMaterialSettingInfo details;
};

// Template specialization for a single component
template <typename T>
class NumericMaterialSetting<T, 1> : public BaseNumericMaterialSetting {
public:
	NumericMaterialSetting() {
		this->data = T();
		this->details = {
			1, sizeof(T), sizeof(T),
			std::is_signed<T>(), std::is_integral<T>(),
			&this->data
		};
	}

	NumericMaterialSetting(T value) {
		this->data = value;
		this->details = {
			1, sizeof(T), sizeof(T),
			std::is_signed<T>(), std::is_integral<T>(),
			&this->data
		};
	}

	NumericMaterialSettingInfo& getDetails() override {
		return details;
	}

	void accept(AbstractMaterialSettingDispatcher &dispatcher) override {
		dispatcher.dispatch(*this);
	}
protected:
	T data;
	NumericMaterialSettingInfo details;
};

} // material
} // graphics
} // tator

#endif
