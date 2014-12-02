#include "Constants.h"
#include "ComponentManager.h"

namespace ecs {

	void ComponentManager::clear(Entity &e) {
		auto &components = entityComponentBits[e.id];
		for (uint componentBit = 0; components.size() > componentBit; componentBit++) {
//			if (components[componentBit]) {
//				store.getEntities<T>()[e.id] = false;
//				store.getComponents<T>() = {};
		}
		components.reset();
	}

	ComponentBits& ComponentManager::getComponentBits(const Entity &e) {
		return entityComponentBits[e.id];
	}
}
