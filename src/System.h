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

	public:
		System(World* world) : world(world) {}
		virtual ~System() = default;

		void insert(Entity e);
		void remove(Entity e);
		void update(Entity e);

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
		ComponentBits requiredComponents;
		ComponentBits disallowedComponents;
		bool isVoidSystem = false;
		bool isInterested(Entity e);
};
}
