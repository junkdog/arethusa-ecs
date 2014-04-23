#include <algorithm>
#include <cassert>
#include <vector>
#include "Constants.h"
#include "Entity.h"
#include "EntitySystem.h"

namespace es {

void EntitySystem::processSystem() {
	if (!isActive()) return;

	begin();
	for (auto entity : actives) {
		processEntity(*entity);
	}
	end();
}

ComponentBits es::EntitySystem::requiredAspect() {
	return ComponentBits();
}

ComponentBits es::EntitySystem::disallowedAspect() {
	return ComponentBits();
}

void EntitySystem::initialize() {}

void EntitySystem::insert(Entity& e) {
	if (isInterested(e) && !activeIds[e.id]) {
		CLOG("Inserting entity[" << e.id << "]");

		actives.push_back(&e);
		activeIds[e.id] = true;
		added(e);
	}
}

void EntitySystem::remove(Entity& e) {
	if (activeIds[e.id]) {
		CLOG("Removing entity[" << e.id << "]");

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
		CLOG("Adding entity[" << e.id << "] via update");

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

	if (requiredComponents != (e.componentBits & requiredComponents))
		return false;

	if ((disallowedComponents & e.componentBits).any())
		return false;

	return true;
}

void EntitySystem::added(Entity& e) {}
void EntitySystem::removed(Entity& e) {}
void EntitySystem::updated(Entity& e) {}

}

