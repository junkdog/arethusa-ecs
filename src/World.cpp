#include "Constants.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "EntityEdit.h"
#include "EditProcessor.h"

namespace ecs {

World::World() {
	systemManager = &setManager<SystemManager>();
	entityManager = &setManager<EntityManager>();
	componentManager = &setManager<ComponentManager>();
	edits = std::make_unique<EditProcessor>(componentManager);
}

void World::initialize() {

	// skip initializing SystemManager until all other
	// managers have been initialized, so that EntitySystems
	// can interact with managers reliably.
	assert(managerIndices[typeid(SystemManager)] == 0);
	for (uint i = 1; managers.size() > i; i++) {
		managers[i]->initialize();
	}
	systemManager->initialize();
}

Entity World::createEntity() {
	return entityManager->createEntity();
}

Entity World::getEntity(uint id) {
	return entityManager->getEntity(id);
}

//void World::updateState(Entity e) {
//	entityManager->updateState(e);
//}

void World::deleteEntity(Entity e) {
	entityManager->kill(e);
}

ComponentManager& World::components() {
	return *componentManager;
}

void World::informManagers(EntityStates& newStates) {
	for (auto manager = managers.begin(); manager != managers.end(); ++manager) {
		for (auto e : newStates.getAdded())
			manager->get()->added(e);
		for (auto e : newStates.getChanged())
			manager->get()->updated(e);
		for (auto e : newStates.getRemoved())
			manager->get()->removed(e);
	}
}

SystemManager& World::systems() {
	return *systemManager;
}

void World::process() {
	entityManager->process(); // notifies systems/managers about changed entities
	systemManager->process(); // runs all systems
}

EntityEdit World::edit(Entity e) {
	return edits->edit(e);
}
}


