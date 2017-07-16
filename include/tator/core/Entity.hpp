// Andrew Black
// May 13, 2017
#ifndef _TATOR_CORE_ENTITY_HPP
#define _TATOR_CORE_ENTITY_HPP

#include <vector>
using std::vector;

#include <cstdint>

#include "tator/core/Component.hpp"
using tator::core::Component;

namespace tator {
namespace core {

class Entity {
public:
	Entity() {
		this->id = 0; // invalid
	}

	Entity(int64_t id) {
		this->id = id;
	}

	void addComponent(Component* component) {
		this->components.push_back(component);
	}

	int64_t getId() { return this->id; }

protected:
	int64_t id;
	vector<Component*> components;
};

} // core
} // tator

#endif