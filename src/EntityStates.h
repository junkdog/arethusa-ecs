#pragma once

#include <vector>
#include "Constants.h"

namespace ecs {
	struct EntityStates {
		EntityStates() {}

		EntityStates(const EntityStates& source) :
				added(source.added),
				deleted(source.deleted),
				changed(source.changed) {}

		bool isEmpty() {
			return added.size() && deleted.size() && changed.size();
		}

		void clear() {
			added.clear();
			deleted.clear();
			changed.clear();
		}

		std::vector<Entity> added {};
		std::vector<Entity> deleted {};
		std::vector<Entity> changed {};
	};
}
