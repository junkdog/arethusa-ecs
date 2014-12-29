#pragma once

#include <cstdint>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <vector>
#include "Manager.h"
#include "Constants.h"
#include "Entity.h"
#include "Component.h"
#include "SystemManager.h"
#include "EditProcessor.h"
#include "Util.h"

typedef uint32_t uint; // osx fix

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
		T& managerRef = *manager;
		managerIndices[typeid(T)] = nextManagerIndex++;
		managers.push_back(std::move(manager));
		return static_cast<T&>(managerRef);
	}

	template<typename T,
		typename std::enable_if<std::is_base_of<Manager, T>::value>::type* = nullptr>
	T& getManager() {
		uint managerIndex = managerIndices[typeid(T)];
		return static_cast<T&>(*managers[managerIndex]);
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
	std::unordered_map<std::type_index, uint> managerIndices;

	uint nextManagerIndex = 0;
	ComponentManager* componentManager;
	EntityManager* entityManager;
	SystemManager* systemManager;

	void updateEntityStates();
};

}
