#pragma once

#include <vector>
#include <list>
#include "Constants.h"
#include "Entity.h"
#include "EntityStates.h"
#include "Manager.h"
#include "World.h"
#include "EditProcessor.h"


namespace ecs {

	struct Entity;
	class World;

	class EntityManager : public Manager {
		public:

		EntityManager(World* world);
		virtual ~EntityManager() = default;

		virtual void initialize();
		EntityEdit& createEntity();
		Entity getEntity(uint id);
		void kill(const Entity e);
		void process(EntityStates& states);

		int getActiveCount();

	private:
		u_int32_t nextEntityId = 0;
		EntityBits active {};
		std::vector<int> recycled;
	};
}
