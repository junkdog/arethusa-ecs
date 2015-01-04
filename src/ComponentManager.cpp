#include "Constants.h"
#include "ComponentManager.h"

namespace ecs {

void ComponentManager::clear(const Entity e) {
	auto& components = entityComponentBits[e.id];

	auto index = components.nextSetBit();
	while (index != -1) {
		stores[index]->remove(e);
		stores[index]->entities()[e.getId()] = false;
		index = components.nextSetBit(index + 1);
	}
	components.reset();
}

ComponentBits& ComponentManager::getComponentBits(const Entity e) {
	return entityComponentBits[e.id];
}
}
