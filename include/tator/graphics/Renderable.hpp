// Andrew Black
// March 08, 2017

#ifndef __TATOR_GRAPHICS_RENDERABLE_HPP
#define __TATOR_GRAPHICS_RENDERABLE_HPP

namespace tator {
namespace graphics {

class Renderable {
public:
	virtual void buffer() = 0;
	virtual void bind() = 0;
	virtual void draw() = 0;
protected:
	Renderable() {}
	~Renderable() {}
};

}
}

#endif