#include "Constants.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "Util.h"

namespace ecs {

World::World() {
	systemManager = &setManager<SystemManager>();
	entityManager = &setManager<EntityManager>();
	componentManager = &setManager<ComponentManager>();
	setManager<ComponentManager>();
	edits = make_unique<EditProcessor>(componentManager);
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

EntityEdit& World::createEntity() {
	return entityManager->createEntity();
}

Entity World::getEntity(uint id) {
	return entityManager->getEntity(id);
}

void World::deleteEntity(Entity e) {
	entityManager->kill(e);
}

ComponentManager& World::components() {
	return *componentManager;
}

void World::informManagers(EntityStates& newStates) {
	for (auto manager = managers.begin(); manager != managers.end(); ++manager) {
		for (auto e : newStates.added)
			manager->get()->added(e);
		for (auto e : newStates.changed)
			manager->get()->updated(e);
		for (auto e : newStates.deleted)
			manager->get()->removed(e);
	}
}

SystemManager& World::systems() {
	return *systemManager;
}

void World::process() {
	updateEntityStates();
	systemManager->process(); // runs all systems
}

void World::updateEntityStates() {
	entityManager->process(edits->getStateChanges());
}

EntityEdit& World::edit(Entity e) {
	return edits->edit(e);
}

}


