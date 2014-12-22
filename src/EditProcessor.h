#pragma once

#include <vector>
//#include "World.h"
#include "Constants.h"
#include "EntityEdit.h"

// todo: track created vs changedd
// todo: save entities in vector
namespace ecs {
//	class EntityEdit;
//	enum class EntityState;
	class World;
	class EditProcessor {

	public:
		EditProcessor(ComponentManager* cm) : cm(cm) {}
		~EditProcessor() = default;
		EntityEdit& edit(Entity e);
		EntityEdit& create(Entity e);
		EntityEdit& remove(Entity e);
		void process();

	private:
		EntityBits editedIds;
		std::vector<EntityEdit> edited;
		ComponentManager* cm;

		EntityEdit& findEdit(Entity e, EntityState newState);
	};
}
