#pragma once

#include "Entity.h"
#include "NoCopy.h"

namespace ecs {

class World;

class Manager : private NoCopy {
  public:
	Manager(World* world) : world(world) {}
	virtual ~Manager() = default;
	virtual void initialize();
	virtual void added(Entity e);
	virtual void removed(Entity e);
	virtual void updated(Entity e);

  protected:
	World* world;
};

}
