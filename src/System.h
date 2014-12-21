#pragma once

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <vector>
#include "Constants.h"
#include "Entity.h"
#include "Component.h"

namespace ecs {

class World;

class System {
friend class SystemManager;
	public:
		System(World* world) : world(world) {}
		virtual ~System() = default;

		virtual void begin() {};
		virtual void end() {};
		virtual void added(__attribute__((__unused__)) Entity e) {};
		virtual void removed(__attribute__((__unused__)) Entity e) {};
		virtual void updated(__attribute__((__unused__)) Entity e) {};

		virtual bool isActive();

		void processEntity(__attribute__((__unused__)) Entity e) {};
		virtual void processSystem() = 0;

		virtual ComponentBits requiredAspect();
		virtual ComponentBits disallowedAspect();


		virtual void initialize();
		void configureAspect() {
			this->requiredComponents = this->requiredAspect();
			this->disallowedComponents = this->disallowedAspect();

			this->isVoidSystem = requiredComponents.none() && disallowedComponents.none();
		};

	protected:
		World* world;
		std::vector<Entity> actives;
		EntityBits activeIds;

	private:
		bool activeNeedsRebuilding;
		ComponentBits requiredComponents;
		ComponentBits disallowedComponents;
		bool isVoidSystem = false;
		unsigned int systemBit = 0;

		void insert(std::vector<Entity>& entities);
		void remove(std::vector<Entity>& entities);
		void update(std::vector<Entity>& entities);
		void syncActiveEntities();

		bool isInterested(Entity e);
		void insert(Entity e);
		void remove(Entity e);
		void update(Entity e);
};
}
