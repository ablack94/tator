// Andrew Black
// July 02, 2017

#ifndef HG_TATOR_GRAPHICS_RENDERINGENGINE_HPP
#define HG_TATOR_GRAPHICS_RENDERINGENGINE_HPP

#include "IRenderable.hpp"
using tator::graphics::IRenderable;

namespace tator {
namespace graphics {

class RenderingEngine {
public:
	virtual void draw(IRenderable& renderable) = 0;
};

} // graphics
} // tator

#endif