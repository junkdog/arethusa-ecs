#pragma once

#include <bitset>
#include <vector>
#include <list>
#include "Constants.h"
#include "Entity.h"
#include "Manager.h"
#include "World.h"


namespace ecs {

	struct Entity;
	class World;

	class EntityManager : public Manager {
		public:

		EntityManager(World* world);
		virtual ~EntityManager() = default;

		virtual void initialize();
		Entity createEntity();
		Entity getEntity(uint id);
		void updateState(const Entity e);
		void kill(const Entity e);
		void process();

		int getActiveCount();

	private:
		u_int32_t nextEntityId = 0;
		EntityBits active {};
		std::vector<int> recycled;

		// per processing round
		EntityStates entityChanges {};
	};
}
