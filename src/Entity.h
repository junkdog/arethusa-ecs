#pragma once

#include "Constants.h"
#include <ostream>
#include <set>
#include <vector>
#include <memory>

namespace ecs {

	struct Entity {
	friend class ComponentManager;
	friend class EntityManager;
	friend class System;

	public:
		Entity& operator=(Entity other) {
			id = other.id;
			return *this;
		}
		Entity(u_int32_t id) :  id(id) {}

		int getId() const {
			return id;
		}
	private:

		u_int32_t id;
	};

	inline std::ostream &operator<<(std::ostream &out, const Entity &e) {
		out << "Entity[" <<  e.getId() << "]";
		return out;
	}


	static std::vector<Entity>& syncEntities(EntityBits& src, std::vector<Entity>& dest) {
		dest.clear();
		auto entityId = src.nextSetBit();
		while (entityId != -1) {
			dest.insert(dest.end(), entityId);
			entityId = src.nextSetBit(entityId + 1);
		}

		return dest;
	}

	struct EntityStates {
		EntityStates() {}

		EntityStates(const EntityStates& source) :
			added(source.added),
			removed(source.removed),
			changed(source.changed) {}

		EntityBits added;
		EntityBits removed;
		EntityBits changed;

		bool isEmpty() {
			return added.none() && removed.none() && changed.none();
		}

		void clear() {
			added.reset();
			removed.reset();
			changed.reset();
		}

		std::vector<Entity>& toEntityVector(EntityBits& src) {
			return syncEntities(src, data);
		}

		std::vector<Entity>& getAdded() {
			return toEntityVector(added);
		}

		std::vector<Entity>& getRemoved() {
			return toEntityVector(removed);
		}

		std::vector<Entity>& getChanged() {
			return toEntityVector(changed);
		}

	private:
		std::vector<Entity> data {};
	};
}
