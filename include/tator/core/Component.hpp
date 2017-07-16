// Andrew Black
// May 13, 2017
#ifndef _TATOR_CORE_COMPONENT_HPP
#define _TATOR_CORE_COMPONENT_HPP

#include <cstdint>

namespace tator {
namespace core {

class Component {
public:
	Component(uint64_t id, Entity& parent) : parent(parent) { }
	virtual void update() = 0;

protected:
	Entity& parent;
};

class ComponentFactory {
public:

};

} // core
} // tator

#endif