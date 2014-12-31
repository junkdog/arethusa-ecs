#pragma once

#include <vector>
#include "Constants.h"
#include "EntityStates.h"
#include "EntityEdit.h"


namespace ecs {
class EditProcessor : private NoCopy {
friend class World;
friend class EntityManager;

  public:
	EditProcessor(ComponentManager* cm) : cm(cm) {}
	~EditProcessor() = default;
	void process();
	EntityStates& getStateChanges();

  private:
	EntityBits editedIds;
	std::vector<EntityEdit> edited;
	ComponentManager* cm;
	EntityStates states;

	EntityEdit edit(Entity e);
	EntityEdit create(Entity e);
	void remove(Entity e);

	EntityEdit& findEdit(Entity e, EntityState newState);
};
}
