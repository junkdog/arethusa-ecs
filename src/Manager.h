#pragma once

#include "Entity.h"

namespace ecs {

class World;

class Manager {
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
