#include <algorithm>
#include "Constants.h"
#include "Entity.h"
#include "System.h"
#include "ComponentManager.h"
#include "World.h"

namespace ecs {

//void System::processSystem() {
//	if (!isActive()) return;
//
//	begin();
//	for (auto entity : actives) {
//		processEntity(*entity);
//	}
//	end();
//}



ComponentBits System::requiredAspect() {
	return ComponentBits();
}

ComponentBits System::disallowedAspect() {
	return ComponentBits();
}

void System::initialize() {}

void System::insert(Entity e) {
	if (isInterested(e) && !activeIds[e.id]) {

		actives.push_back(e);
		activeIds[e.id] = true;
		added(e);
	}
}

void System::remove(Entity e) {
	if (activeIds[e.id]) {
		activeIds[e.id] = false;
		actives.erase(actives.end());
//		auto it = std::find(actives.begin(), actives.end(), e);
//		if (it != actives.end()) {
//			auto last = --actives.end();
//			assert(e.id == (it)->id);
//			if (it != last) std::iter_swap(it, last);
//			actives.erase(e);
//			removed(e);
//		}
	}
//	if (activeIds[e.id]) {
//
//	}
}


void System::update(Entity e) {
	bool interested = isInterested(e);
	bool inSystem = activeIds[e.id];

	if (interested && !inSystem) {
		actives.push_back(e);
		activeIds[e.id] = true;
		added(e);
	} else if (interested && inSystem) {
		updated(e);
	} else if (!interested && inSystem) {
		remove(e);
	}
}

bool System::isActive() {
	return isVoidSystem || !actives.empty();
}

bool System::isInterested(Entity e) {
	if (isVoidSystem) return false;

	auto& componentBits = world->components().getComponentBits(e);
	if (requiredComponents != (componentBits & requiredComponents))
		return false;

	if ((disallowedComponents & componentBits).any())
		return false;

	return true;
}

}

