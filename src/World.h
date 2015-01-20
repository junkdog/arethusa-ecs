#pragma once

#include <cassert>
#include <algorithm>
#include <cstdint>
#include <typeinfo>
#include <memory>
#include <vector>
#include "Entity.h"
#include "SystemManager.h"
#include "Util.h"

namespace ecs {

class Manager;
class EntityManager;
class SystemManager;
class ComponentManager;
class EntityEdit;

class World {
	friend class ComponentManager;
	friend class EntityManager;
	friend class SystemManager;

  public:
	World();
	~World() = default;

	EntityEdit& createEntity();
	EntityEdit& edit(Entity e);
	Entity getEntity(uint id);
	void deleteEntity(Entity e);

	template<typename T, typename ... Args,
		typename std::enable_if<std::is_base_of<Manager, T>::value>::type* = nullptr>
	T& setManager(Args&& ... args) {

		auto manager = make_unique<T>(this, std::forward<Args>(args) ...);
		managers.push_back(std::move(manager));
		return static_cast<T&>(*managers.back());
	}

	template<typename T,
		typename std::enable_if<std::is_base_of<Manager, T>::value>::type* = nullptr>
	T& getManager() {
		auto found = std::find_if(managers.cbegin(), managers.cend(),
			[&](const std::unique_ptr<Manager>& m) -> bool {
				return typeid(*m.get()) == typeid(T);
			});

		assert(managers.cend() != found);
		return static_cast<T&>(**found);
	}

	void informManagers(EntityStates& newStates);
	void initialize();
	void process();

	ComponentManager& components();
	SystemManager& systems();
	float delta = 0;

  private:
	std::unique_ptr<EditProcessor> edits;
	std::vector<std::unique_ptr<Manager>> managers;

	ComponentManager* componentManager;
	EntityManager* entityManager;
	SystemManager* systemManager;

	void updateEntityStates();
};

}
