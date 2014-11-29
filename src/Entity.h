#pragma once

#include "Constants.h"
#include <ostream>
#include <bitset>
#include <set>
#include <vector>
#include <memory>

namespace es {

	struct Entity {
		Entity(u_int16_t id) :  id(id) {}
		const u_int16_t id;
		ComponentBits componentBits;
		SystemBits systemBits;
		
	} typedef Entity;

	inline std::ostream &operator << (std::ostream &out, const es::Entity &e) {
		out << "Entity[" <<  e.id << "]:" + e.componentBits.to_string();
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

	} typedef Changed;
	
	inline std::ostream &operator << (std::ostream &out, const es::EntityStates &c) {
		out << "EntityStates[added: " <<  c.added.size()  << ", removed: " << c.removed.size() << ", changed: " << c.changed.size() << "]:";
		return out;
	}
}
