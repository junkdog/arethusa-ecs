#include <algorithm>
#include <cassert>
#include "Constants.h"
#include "Entity.h"
#include "EntitySystem.h"
#include "ComponentManager.h"
#include "World.h"

namespace ecs {

void EntitySystem::processSystem() {
	if (!isActive()) return;

	begin();
	for (auto entity : actives) {
		processEntity(*entity);
	}
	end();
}

ComponentBits EntitySystem::requiredAspect() {
	return ComponentBits();
}

ComponentBits EntitySystem::disallowedAspect() {
	return ComponentBits();
}

void EntitySystem::initialize() {}

void EntitySystem::insert(Entity& e) {
	if (isInterested(e) && !activeIds[e.id]) {

		actives.push_back(&e);
		activeIds[e.id] = true;
		added(e);
	}
}

void EntitySystem::remove(Entity& e) {
	if (activeIds[e.id]) {
		activeIds[e.id] = false;
		auto it = std::find(actives.begin(), actives.end(), &e);
		if (it != actives.end()) {
			auto last = --actives.end();
			assert(e.id == (*it)->id);
			if (it != last) std::iter_swap(it, last);
			actives.erase(last);
			removed(e);
		}
	}
}


void EntitySystem::update(Entity& e) {
	bool interested = isInterested(e);
	bool inSystem = activeIds[e.id];

	if (interested && !inSystem) {
		actives.push_back(&e);
		activeIds[e.id] = true;
		added(e);
	} else if (interested && inSystem) {
		updated(e);
	} else if (!interested && inSystem) {
		remove(e);
	}
}

bool EntitySystem::isActive() {
	return isVoidSystem || !actives.empty();
}

bool EntitySystem::isInterested(const Entity& e) {
	if (isVoidSystem) return false;

	auto& componentBits = world->components().getComponentBits(e);
	if (requiredComponents != (componentBits & requiredComponents))
		return false;

	if ((disallowedComponents & componentBits).any())
		return false;

	return true;
}

}

