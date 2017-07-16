// Andrew Black
// July 07, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_MATERIAL_BASEMATERIALSETTING_HPP
#define HG_TATOR_GRAPHICS_MATERIAL_BASEMATERIALSETTING_HPP

namespace tator {
namespace graphics {
namespace material {

class AbstractMaterialSettingDispatcher;

class BaseMaterialSetting {
public:
	virtual void accept(AbstractMaterialSettingDispatcher &dispatcher) = 0;
};

} // material
} // graphics
} // tator

#endif
