#pragma once

#include "Entity.h"
#include "World.h"


namespace ecs {

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
	EntityBits active{};
	std::vector<int> recycled;
};
}
