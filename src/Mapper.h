#pragma once

#include <memory>
#include <vector>
#include "Constants.h"
#include "Component.h"
#include "ComponentManager.h"
#include "Entity.h"
#include "World.h"

namespace ecs {

template<typename C, typename enable_if_component<C>::type* = nullptr>
class Mapper {

  public:
	Mapper(World* world) {
		ComponentManager& cm = world->components();
		components = &cm.store<C>().getComponents();
		componentBit = cm.componentBits<C>().nextSetBit();
		entityComponentBits = &cm.entityComponentBits;
	}
	~Mapper() = default;

	C& get(Entity e) {
		return components->at(e.getId());
	}

	C& operator[](Entity e) {
		return components->at(e.getId());
	}

	bool has(Entity e) {
		return entityComponentBits->at(e.getId())[componentBit];
	}

  private:
	std::vector<C>* components;
	u_int16_t componentBit;
	std::vector<ComponentBits>* entityComponentBits;
};
}
