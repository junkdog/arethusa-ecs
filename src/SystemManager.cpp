#include <memory>
#include "Constants.h"
#include "SystemManager.h"

namespace ecs {

void SystemManager::initialize() {
	for (auto system = systems.begin(); system != systems.end(); ++system) {
		system->get()->configureAspect();
		system->get()->initialize();
	}
}

void SystemManager::process() {
	for (auto system = systems.begin(); system != systems.end(); ++system) {
		system->get()->processSystem();
	}
}

// Must copy EntityStates in case entities change
// when intercepted by systems (and managers)
void SystemManager::inform(EntityStates& entitiesChanged) {
	for (auto system = systems.begin(); system != systems.end(); ++system) {
		for (auto e : entitiesChanged.added)
			system->get()->insert(e);
		for (auto e : entitiesChanged.changed)
			system->get()->update(e);
		for (auto e : entitiesChanged.removed)
			system->get()->remove(e);
	}
}

}

