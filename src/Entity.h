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

	inline std::ostream& operator<<(std::ostream &out, const Entity &e) {
		out << "Entity[" <<  e.getId() << "]";
		return out;
	}
}
