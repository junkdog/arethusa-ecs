#include <algorithm>
#include "Constants.h"
#include "EditProcessor.h"

namespace ecs {
EntityEdit& EditProcessor::edit(Entity e) {
	return findEdit(e, EntityState::CHANGE);
}

EntityEdit& EditProcessor::create(Entity e) {
	return findEdit(e, EntityState::CREATE);
}

void EditProcessor::remove(Entity e) {
	findEdit(e, EntityState::DELETE);
}

EntityEdit& EditProcessor::findEdit(Entity e, EntityState newState) {
	if (editedIds[e.getId()]) {
		auto edit = std::find_if(edited.begin(), edited.end(),
			[&](std::unique_ptr<EntityEdit>& edit) -> bool {return edit->entity == e;});

		assert(edited.end() != edit);
		edit->get()->state = std::max(edit->get()->state, newState);
		return *edit->get();
	} else {
		editedIds[e.getId()] = true;
		auto ee = make_unique<EntityEdit>(cm, e, newState);
		edited.push_back(std::move(ee));
		return *edited.back();
	}
}

EntityStates& EditProcessor::getStateChanges() {
	states.clear();
	for (std::unique_ptr<EntityEdit>& e : edited) {
		switch (e->state) {
			case EntityState::CREATE:
				states.added.emplace_back(e->getEntity());
				break;
			case EntityState::DELETE:
				states.deleted.emplace_back(e->getEntity());
				break;
			case EntityState::CHANGE:
				states.changed.emplace_back(e->getEntity());
				break;
			default:
				assert(false);
				break;
		}
	}

	editedIds.reset();
	edited.clear();

	return states;
}
}
