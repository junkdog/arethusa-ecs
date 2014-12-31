#include <algorithm>
#include "Constants.h"
#include "EditProcessor.h"
#include <memory>

namespace ecs {
EntityEdit EditProcessor::edit(Entity e) {
	return findEdit(e, EntityState::CHANGE);
}

EntityEdit EditProcessor::create(Entity e) {
	return findEdit(e, EntityState::CREATE);
}

void EditProcessor::remove(Entity e) {
	findEdit(e, EntityState::DELETE);
}

EntityEdit& EditProcessor::findEdit(Entity e, EntityState newState) {
	if (editedIds[e.getId()]) {
		auto edit = std::find_if(edited.begin(), edited.end(),
			[=](const EntityEdit& edit) -> bool {return edit.entity == e;});

		assert(edited.end() != edit);
		edit->state = std::max(edit->state, newState);
		return *edit;
	} else {
		editedIds[e.getId()] = true;
//		EntityEdit ee {cm, e, newState};
//		edited.push_back(ee);
		edited.emplace_back(cm, e, newState);
		return edited.back();
	}
}

EntityStates& EditProcessor::getStateChanges() {
	states.clear();
	for (EntityEdit& e : edited) {
		switch (e.state) {
			case EntityState::CREATE:
				states.added.emplace_back(e.getEntity());
				break;
			case EntityState::DELETE:
				states.deleted.emplace_back(e.getEntity());
				break;
			case EntityState::CHANGE:
				states.changed.emplace_back(e.getEntity());
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
