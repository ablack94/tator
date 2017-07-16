// Andrew Black
// July 07, 2017
#pragma once
#ifndef HG_TATOR_GRAPHICS_MATERIAL_ABSTRACTMATERIALSETTINGDISPATCHER_HPP
#define HG_TATOR_GRAPHICS_MATERIAL_ABSTRACTMATERIALSETTINGDISPATCHER_HPP

namespace tator {
namespace graphics {
namespace material {

class BaseNumericMaterialSetting;

class AbstractMaterialSettingDispatcher {
public:
	virtual void dispatch(BaseNumericMaterialSetting &setting) = 0;
};

} // material
} // graphics
} // tator

#endif
