#pragma once

#include <vector>
#include <list>
#include "Constants.h"
#include "Entity.h"
#include "Manager.h"
#include "World.h"


namespace es {

	struct Entity;
	class World;

	class EntityManager : public Manager {
		public:

		EntityManager(World* world);
		virtual ~EntityManager() = default;

		virtual void initialize();
		Entity& createEntity();
		Entity* getEntity(uint id);
		void updateState(Entity& e);
		Entity& entity(uint id);
		void kill(Entity& e);
		void process();

		int getActiveCount();

	private:
		u_int16_t nextEntityId = 0;
		std::vector<std::unique_ptr<Entity>> entities {};
		std::list<u_int16_t> recycled;

		// per processing round
		EntityStates entityChanges {};
	};
}
