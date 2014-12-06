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

typedef uint32_t uint; // osx fix

namespace ecs {

class Manager;
class EntityManager;
class SystemManager;
class ComponentManager;

class World {
	public:

		World();
		virtual ~World();

		Entity createEntity();
		void updateState(Entity& e);
		Entity getEntity(uint id);
		void deleteEntity(Entity& e);

		template <typename T, typename ... Args,
			typename std::enable_if<std::is_base_of<Manager, T>::value>::type* = nullptr>
		T& setManager(Args && ... args) {

			auto manager = std::unique_ptr<T>(new T(this, std::forward<Args>(args) ...));
			T& managerRef = *manager;
			managerIndices[typeid(T)] = nextManagerIndex++;
			managers.push_back(std::move(manager));
			return static_cast<T&>(managerRef);
		}

		template <typename T,
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
		std::vector<std::unique_ptr<Manager>> managers;
		std::unordered_map<std::type_index, uint> managerIndices;
		uint nextManagerIndex = 0;

		ComponentManager* componentManager;
		EntityManager* entityManager;
		SystemManager* systemManager;
};

}
