#pragma once

#include <vector>

namespace ecs {
struct EntityStates {
	EntityStates() {}

	EntityStates(const EntityStates& source) :
		added(source.added),
		deleted(source.deleted),
		changed(source.changed) {}

	bool isEmpty() {
		return added.empty() && deleted.empty() && changed.empty();
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
