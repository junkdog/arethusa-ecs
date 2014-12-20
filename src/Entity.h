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




	struct EntityStates {
		EntityStates() {
//			added.reserve(MAX_ENTITIES);
//			removed.reserve(MAX_ENTITIES);
//			changed.reserve(MAX_ENTITIES);
		}

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

		std::vector<Entity>& syncEntities(EntityBits& src) {
			data.clear();
			auto index = src.nextSetBit();
			while (index != -1) {
				data.insert(data.end(), index);
				index = src.nextSetBit(index + 1);
			}

			return data;
		}

		std::vector<Entity>& getAdded() {
			return syncEntities(added);
		}

		std::vector<Entity>& getRemoved() {
			return syncEntities(removed);
		}

		std::vector<Entity>& getChanged() {
			return syncEntities(changed);
		}

	private:
		std::vector<Entity> data {};
	};
	
	inline std::ostream &operator << (std::ostream &out, const EntityStates &c) {
//		out << "EntityStates[added: " <<  c.added.size()  << ", removed: " << c.removed.size() << ", changed: " << c.changed.size() << "]:";
		return out;
	}

}
