#pragma once

#include <vector>
#include "Constants.h"
#include "Component.h"

// TODO: save pointer to store in component manager
namespace ecs {


template<typename T, typename enable_if_component<T>::type* = nullptr>
class Store : NoCopy {
friend class ComponentManager;
  public:
	std::vector<T>& getComponents() {
		return components;
	}

	EntityBits& entities() {
		return _entities;
	}

	u_int16_t index() const {
		return id;
	}

  private:
	std::vector<T> components {};
	const u_int16_t id;
	EntityBits _entities {};

	Store(u_int16_t id) : id(id) {
		components.resize(MAX_COMPONENTS);
	}
	~Store() = default;
};

}
