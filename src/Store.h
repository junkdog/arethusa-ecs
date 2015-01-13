#pragma once

#include <vector>
#include "Component.h"

// TODO: save pointer to store in component manager
namespace ecs {

class BaseStore : NoCopy {
  public:
	virtual ~BaseStore() = default;
	virtual void remove(const Entity e) = 0;
	virtual EntityBits& entities() = 0;
};

template<typename T, typename enable_if_component<T>::type* = nullptr>
class Store : public BaseStore {
friend class ComponentManager;
  public:
	Store(u_int16_t id) : id(id) {
		components.resize(MAX_ENTITIES);
	}
	virtual ~Store() = default;

	std::vector<T>& getComponents() {
		return components;
	}

	EntityBits& entities() {
		return _entities;
	}

	u_int16_t index() const {
		return id;
	}

	void remove(const Entity e) {
		_entities[e.getId()] = false;
	}

  private:
	std::vector<T> components {};
	const u_int16_t id;
	EntityBits _entities {};
};
}
