#include <algorithm>
#include "Constants.h"
#include "EditProcessor.h"

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
		if (this->editedIds[e.getId()]) {
			auto edit = std::find_if(this->edited.begin(), this->edited.end(),
				[=](const EntityEdit& edit) -> bool { return edit.entity.getId() == e.getId(); });

			assert(this->edited.end() != edit);
			edit->state = std::max(edit->state, newState);
			return *edit;
		} else {
			this->editedIds[e.getId()] = true;
			this->edited.emplace_back(this->cm, e, newState);
			return this->edited.back();
		}
	}

	void EditProcessor::process() {
		editedIds.reset();
		edited.clear();
	}

//	bool EditProcessor::IsMatchedEntityEdit(int entityId) {
//		return false;
//	}
}
