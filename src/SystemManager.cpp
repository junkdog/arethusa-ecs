#include <memory>
#include "Constants.h"
#include "SystemManager.h"

namespace ecs {

void SystemManager::initialize() {
	for (auto& system : systems) {
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
		system->insert(entitiesChanged.added);
		system->update(entitiesChanged.changed);
		system->remove(entitiesChanged.deleted);
		system->syncActiveEntities();
	}
}

}

