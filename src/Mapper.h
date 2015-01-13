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
	Mapper(World* world) :
		store(world->components().store<C>()),
		components(store.getComponents()),
		componentBit(store.index()) {}
	~Mapper() = default;

	C& get(Entity e) {
		return components[e.getId()];
	}

	C& operator[](Entity e) {
		return components[e.getId()];
	}

	bool has(Entity e) {
		return store.entities()[e.getId()];
	}

  private:
	Store<C>& store;
	std::vector<C>& components;
	u_int16_t componentBit;
};
}
