#pragma once

#include <vector>
#include "Constants.h"
#include "EntityStates.h"
#include "EntityEdit.h"


namespace ecs {
class EditProcessor : private NoCopy {

  public:
	EditProcessor(ComponentManager* cm) : cm(cm) {}

	~EditProcessor() = default;
	EntityEdit& edit(Entity e);
	EntityEdit& create(Entity e);
	EntityEdit& remove(Entity e);
	void process();
	EntityStates getStateChanges();

  private:
	EntityBits editedIds;
	std::vector<EntityEdit> edited;
	ComponentManager* cm;
	EntityStates states;

	EntityEdit& findEdit(Entity e, EntityState newState);
};
}
