#include "Constants.h"
#include "EntityManager.h"

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
	assert(typeid(*managers[0]) == typeid(SystemManager));
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
	for (auto& manager : managers) {
		for (auto e : newStates.added)
			manager->added(e);
		for (auto e : newStates.changed)
			manager->updated(e);
		for (auto e : newStates.deleted)
			manager->removed(e);
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


