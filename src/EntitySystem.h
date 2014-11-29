#pragma once

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <vector>
#include "Constants.h"
#include "Entity.h"
#include "Component.h"

namespace es {

class World;

class EntitySystem {
	public:
		EntitySystem(World* world) : world(world) {}
		virtual ~EntitySystem() = default;

		virtual void insert(Entity& e);
		virtual void remove(Entity& e);
		virtual void update(Entity& e);

		virtual void begin() {};
		virtual void end() {};
		virtual void added(Entity &e);
		virtual void removed(Entity &e);
		virtual void updated(Entity &e);

		virtual bool isActive();

		virtual void processEntity(Entity &e) = 0;
		virtual void processSystem();

		virtual ComponentBits requiredAspect();
		virtual ComponentBits disallowedAspect();


		virtual void initialize();
		void configureAspect() {
			this->requiredComponents = this->requiredAspect();
			this->disallowedComponents = this->disallowedAspect();

			this->isVoidSystem = requiredComponents.none() && disallowedComponents.none();
		};
		uint systemBit = 0;

	protected:
		World* world;
		std::vector<Entity*> actives;
		std::bitset<MAX_ENTITIES> activeIds;

	private:
		ComponentBits requiredComponents;
		ComponentBits disallowedComponents;
		bool isVoidSystem = false;
		bool isInterested(const Entity& e);
};
}
