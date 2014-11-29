#pragma once

#include "Entity.h"

namespace es {

class World;

class Manager {
	public:
		Manager(World* world) : world(world) {}
		virtual ~Manager() {}
		virtual void initialize() {}
		virtual void added(Entity& e);
		virtual void removed(Entity& e);
		virtual void updated(Entity& e);

	protected:
		World* world;
};

}
