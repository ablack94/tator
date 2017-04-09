// Andrew Black
// March 08, 2017

#ifndef __TATOR_GRAPHICS_MESH
#define __TATOR_GRAPHICS_MESH

#include <vector>

#include "tator/graphics/gl.hpp"

#include "tator/graphics/Renderable.hpp"
	using tator::graphics::Renderable;	

namespace tator {
namespace graphics {

class Mesh : public Renderable {
protected:
	Mesh() { }
	virtual ~Mesh() { }
}; 

} // graphics
} // tator

#endif