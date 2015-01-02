#pragma once

#include <vector>
#include "Constants.h"
#include "Component.h"

// TODO: save pointer to store in component manager
namespace ecs {

class BaseStore : NoCopy {
  public:
	virtual ~BaseStore() = default;
};

template<typename T, typename... Args>
std::unique_ptr<BaseStore> make_store(Args&& ... args) {
	return std::unique_ptr<BaseStore>{new T(std::forward<Args>(args)...)};
}

template<typename T, typename enable_if_component<T>::type* = nullptr>
class Store : public BaseStore { //};, NoCopy {
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

  private:
	std::vector<T> components {};
	const u_int16_t id;
	EntityBits _entities {};

//	Store<T>& operator=(const Store<T>&& rhs) {
//		components = std::move(rhs.components);
//		id = rhs.id;
//	}
};

}
