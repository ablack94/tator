// Andrew Black
// May 13, 2017
#ifndef _TATOR_CORE_ENTITYMANAGER_HPP
#define _TATOR_CORE_ENTITYMANAGER_HPP

#include <vector>
#include <map>
#include <memory>

#include "tator/core/Entity.hpp"
using tator::core::Entity;

#include "tator/core/Component.hpp"
using tator::core::Component;

namespace tator {
namespace core {

class EntityManager {
public:
	EntityManager() {
		this->next_id = 1;
	}

	std::shared_ptr<Entity> createEntity() {
		auto entity = std::make_shared<Entity>(this->next_id++);
		entities[entity->getId()] = entity;
		return entity;
	}

protected:
	uint64_t next_id;
	std::map<uint64_t, std::shared_ptr<Entity>> entities;
	//std::vector<std::shared_ptr<Entity>> entities;
};

} // core
} // tator

#endif