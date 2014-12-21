#pragma once

#include <vector>
#include "World.h"
#include "Constants.h"

// todo: track created vs changedd
// todo: save entities in vector
namespace ecs {
	class InEditProcessor {

	public:
		InEditProcessor(World& world) : world(world) {}
		~InEditProcessor() = default;
		EntityEdit& edit();
		void process();

	private:
		EntityBits createdIds;
		EntityBits editedIds;
		vector<Entity> created;
		vector<Entity> changed;
		World world;
	};
}
