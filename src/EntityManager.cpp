#include "Constants.h"
#include "Entity.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"

namespace es {

	EntityManager::EntityManager(World *world) : Manager(world) {
		entities.reserve(MAX_COMPONENTS);
	}


	void EntityManager::initialize() {}

	Entity& EntityManager::createEntity() {
		uint32_t id;
		if (recycled.size() > 0) {
			id = recycled.front();
			recycled.pop_front();
		} else {
			id = nextEntityId++;
		}

		entities[id] = Entity {id};
		entityChanges.added.insert(&entities[id]);
		active[id] = true;
		return entities[id];
	}

	void EntityManager::updateState(Entity& e) {
		entityChanges.changed.insert(&e);
	}

	Entity& EntityManager::getEntity(uint id) {
		return entities[id];
	}

	void EntityManager::kill(Entity& e) {
		entityChanges.removed.insert(&e);
	}

	void EntityManager::process() {
		if (entityChanges.isEmpty())
			return;

		EntityStates oldState(entityChanges);
		entityChanges.clear();

		world->systems().inform(oldState);
		world->informManagers(oldState);

		// postponing clearing the components of deleted entities
		// in case a system wants to act on any entities' components.
		for (auto e : oldState.removed) {
			world->components().clear(*e);
			recycled.push_back(e->id);
			entities[e->id] = Entity {0};
		}
	}

	int EntityManager::getActiveCount() {
		return entities.size();
	}
}

