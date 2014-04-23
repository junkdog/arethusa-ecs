#include "Constants.h"
#include "Entity.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"
#include <memory>
#include <vector>

namespace es {

	void EntityManager::initialize() {

	}

	Entity& EntityManager::createEntity() {
		uint16_t id;
		if (recycled.size() > 0) {
			id = recycled.front();
			recycled.pop_front();
		} else {
			id = nextEntityId++;
		}

		std::unique_ptr<Entity> entity = std::unique_ptr<Entity>{new Entity{id}};

		Entity &ref = *entity;
		entities[ref.id] = std::move(entity);
		entityChanges.added.insert(&ref);
		return ref;
	}

	void EntityManager::updateState(Entity& e) {
		entityChanges.changed.insert(&e);
	}

	Entity* EntityManager::getEntity(uint id) {
		return entities[id].get();
	}

	Entity& EntityManager::entity(uint id) {
		return *(entities[id].get());
	}

	void EntityManager::kill(Entity& e) {
		entityChanges.removed.insert(&e);
	}

	void EntityManager::process() {
		if (entityChanges.isEmpty())
			return;

		LOG(entityChanges);
		EntityStates oldState(entityChanges);
		entityChanges.clear();

		world->systems().inform(oldState);
		world->informManagers(oldState);

		// postponing clearing the components of deleted entities
		// in case a system wants to act on any entities' components.
		for (auto e : oldState.removed) {
			world->components().clear(*e);
			recycled.push_back(e->id);
			entities[e->id] = nullptr;
		}
	}
}

