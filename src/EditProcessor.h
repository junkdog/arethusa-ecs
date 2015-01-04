#pragma once

#include <memory>
#include <vector>
#include "Constants.h"
#include "EntityStates.h"
#include "NoCopy.h"
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
	std::vector<std::unique_ptr<EntityEdit>> edited {};
	ComponentManager* cm;
	EntityStates states;

	EntityEdit& edit(Entity e);
	EntityEdit& create(Entity e);
	void remove(Entity e);

	EntityEdit& findEdit(Entity e, EntityState newState);
};
}
