#include <algorithm>
#include "Constants.h"
#include "EditProcessor.h"
#include <memory>

namespace ecs {
EntityEdit& EditProcessor::edit(Entity e) {
	return findEdit(e, EntityState::CHANGED);
}

EntityEdit& EditProcessor::create(Entity e) {
	return findEdit(e, EntityState::CREATED);
}

EntityEdit& EditProcessor::remove(Entity e) {
	return findEdit(e, EntityState::DELETED);
}

EntityEdit& EditProcessor::findEdit(Entity e, EntityState newState) {
	if (editedIds[e.getId()]) {
		auto edit = std::find_if(edited.begin(), edited.end(),
			[=](const EntityEdit& edit) -> bool {return edit.entity.getId() == e.getId();});

		assert(edited.end() != edit);
		edit->state = std::max(edit->state, newState);
		return *edit;
	} else {
		editedIds[e.getId()] = true;
		edited.emplace_back(cm, e, newState);
		return edited.back();
	}
}

EntityStates& EditProcessor::getStateChanges() {
	states.clear();
	for (EntityEdit& e : edited) {
		switch (e.state) {
			case EntityState::CREATED:
				states.added.emplace_back(e.getEntity());
				break;
			case EntityState::DELETED:
				states.deleted.emplace_back(e.getEntity());
				break;
			case EntityState::CHANGED:
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
