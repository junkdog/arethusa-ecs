#include <cassert>
#include "Constants.h"
#include "Entity.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"

namespace ecs {

	EntityManager::EntityManager(World* world) : Manager(world) {
		recycled.reserve(MAX_ENTITIES);
	}

	void EntityManager::initialize() {}

	Entity EntityManager::createEntity() {
		uint32_t id;
		if (recycled.size() > 0) {
			id = recycled.back();
			recycled.pop_back();
		} else {
			id = nextEntityId++;
		}

		entityChanges.added.push_back({id});
		active[id] = true;
		return {id};
	}

	void EntityManager::updateState(const Entity e) {
		entityChanges.changed.push_back(e);
	}

	Entity EntityManager::getEntity(uint id) {
		assert(active[id]);
		return {id};
	}

	void EntityManager::kill(const Entity e) {
		entityChanges.removed.push_back(e);
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
		for (const auto e : oldState.removed) {
			world->components().clear(e);
			recycled.push_back(e.id);
			active[e.id] = false;
		}
	}

	int EntityManager::getActiveCount() {
		return active.count();
	}
}

