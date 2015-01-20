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
	u_int32_t getId() const;
	bool operator==(const Entity& rhs) const;

  private:
	u_int32_t id;
};




}
