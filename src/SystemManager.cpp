#include <memory>
#include "Constants.h"
#include "SystemManager.h"

namespace ecs {

void SystemManager::initialize() {
	for (auto& system : systems) {
		system->configureAspect();
		system->initialize();
	}
}

void SystemManager::process() {
	for (auto& system : systems) {
		system->processSystem();
	}
}

void SystemManager::inform(EntityStates& entitiesChanged) {
	for (auto& system : systems) {
		system->insert(entitiesChanged.getAdded());
		system->update(entitiesChanged.getChanged());
		system->remove(entitiesChanged.getRemoved());
		system->syncActiveEntities();
	}
}

}

