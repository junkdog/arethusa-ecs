#pragma once

#include "Constants.h"
#include <ostream>
#include <bitset>
#include <set>
#include <vector>
#include <memory>

namespace ecs {

	struct Entity {
	friend class ComponentManager;
	friend class EntityManager;
	friend class EntitySystem;

	public:
		~Entity() = default;

		Entity& operator=(Entity other) {
			std::swap(id, other.id);
			return *this;
		}

		int getId() const {
			return id;
		}

	private:
		Entity(u_int32_t id) :  id(id) {}

		u_int32_t id;
	};

	inline std::ostream &operator << (std::ostream &out, const Entity &e) {
		out << "Entity[" <<  e.getId() << "]";
		return out;
	}

	struct EntityStates {
		EntityStates() {}
		EntityStates(const EntityStates& source) :
			added(source.added),
			removed(source.removed),
			changed(source.changed) {}

		std::set<Entity*> added;
		std::set<Entity*> removed;
		std::set<Entity*> changed;

		bool isEmpty() {
			return added.empty() && removed.empty() && changed.empty();
		}

		void clear() {
			added.clear();
			removed.clear();
			changed.clear();
		}

	};
	
	inline std::ostream &operator << (std::ostream &out, const EntityStates &c) {
		out << "EntityStates[added: " <<  c.added.size()  << ", removed: " << c.removed.size() << ", changed: " << c.changed.size() << "]:";
		return out;
	}
}
