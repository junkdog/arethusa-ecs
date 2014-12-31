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

	EntityEdit EntityManager::createEntity() {
		uint32_t id;
		if (recycled.size() > 0) {
			id = recycled.back();
			recycled.pop_back();
		} else {
			id = nextEntityId++;
		}

		active[id] = true;
		return world->edits->create({id});
	}

//	void EntityManager::updateState(const Entity e) {
//		entityChanges.changed[e.id] = true;
//	}

	Entity EntityManager::getEntity(uint id) {
		assert(active[id]);
		return {id};
	}

	void EntityManager::kill(const Entity e) {
		world->edits->remove(e);
	}

	void EntityManager::process(EntityStates& states) {
		if (states.isEmpty())
			return;

		world->systems().inform(states);
		world->informManagers(states);

		// postponing clearing the components of deleted entities
		// in case a system wants to act on any entities' components.
		for (auto e : states.deleted) {
			world->components().clear(e);
			recycled.push_back(e.id);
			active[e.id] = false;
		}
	}

	int EntityManager::getActiveCount() {
		return active.count();
	}
}

