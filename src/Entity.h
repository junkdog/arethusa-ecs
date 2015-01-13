#pragma once

#include "Constants.h"
#include <ostream>

namespace ecs {

struct Entity {
	friend class ComponentManager;
	friend class EntityManager;
	friend class System;

  public:
	Entity(u_int32_t id) : id(id) {}
	~Entity() = default;

	u_int32_t getId() const {
		return id;
	}

	bool operator==(const Entity& rhs) const {
		return id == rhs.id;
	}

  private:
	u_int32_t id;
};

inline std::ostream& operator<<(std::ostream& out, const Entity& e) {
	out << "Entity[" << e.getId() << "]";
	return out;
}

}
