#pragma once

#include <cassert>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <vector>
#include <unordered_map>
#include "Manager.h"
#include "Component.h"
//#include "Mapper.h"
//#include "World.h"

class World;

namespace es {

class ComponentManager : public Manager {
	friend class Mapper;
	public:

		ComponentManager(World* world) : Manager(world) {
			LOG("Initializing component manager.");
			if (componentEntityTable.size() < MAX_COMPONENTS) {
				componentEntityTable.resize(MAX_COMPONENTS);
				for (auto& componentTable : componentEntityTable) {
					componentTable.resize(MAX_ENTITIES);
				}
			}
			entityComponentBits.reserve(MAX_ENTITIES);
		}
		virtual ~ComponentManager() = default;

		template <typename T, typename ... Args,
			typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		T& set(Entity &e,  Args && ... args) {
			std::unique_ptr<T> c = std::unique_ptr<T>(new T(std::forward<Args>(args) ...));
			T& componentRef = *c;

			ensureRegistered<T>();
			CLOG("adding component " << typeid(T).name() << " to entity[" << e.id << "]");

			int typeIndex = componentToIndex[typeid(T)];
			componentEntityTable[typeIndex][e.id] = std::move(c);

			entityComponentBits[e.id].set(typeIndex, true);

			return componentRef;
		}


		template <typename T, typename ... Args,
			typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		void remove(Entity &e) {

			CLOG("removing component " << typeid(T).name() << " for entity[" << e.id << "]");

			assert(componentToIndex.count(typeid(T)) == 1);

			u_int16_t typeIndex = componentToIndex[typeid(T)];
			componentEntityTable[typeIndex][e.id] = nullptr;

			entityComponentBits[e.id].set(typeIndex, false);
		}

		template <typename T,
			typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		T& get(const Entity &e) {

			ensureRegistered<T>();

			int typeIndex = componentToIndex[typeid(T)];
			if (entityComponentBits[e.id][typeIndex]) {
				auto component = componentEntityTable[typeIndex][e.id].get();
				return static_cast<T&>(*component);
			} else {
				CLOG("Attempted to retrieve NULL " << typeid(T).name() << " for " << e.id);
				auto component = componentEntityTable[typeIndex][e.id].get();
				return static_cast<T&>(*component);
			}
		}

		template <typename C,
			typename std::enable_if<std::is_base_of<Component, C>::value>::type* = nullptr>
		ComponentBits componentBits() {

			std::bitset<MAX_COMPONENTS> aspect;

			auto& Component = typeid(C);
			ensureRegistered<C>();

			assert(componentToIndex.count(Component) == 1);
			aspect[componentToIndex[Component]] = true;
			return aspect;;
		}

		template <typename C1, typename C2, typename ... Cn>
		ComponentBits componentBits() {}

		template <typename T,
			typename std::enable_if<std::is_base_of<Component, T>::value>::type* = nullptr>
		std::vector<std::unique_ptr<Component>>& componentTable() {

			ensureRegistered<T>();

			int typeIndex = componentToIndex[typeid(T)];
			return componentEntityTable[typeIndex];
		}

		void clear(Entity& e);
		ComponentBits& getComponentBits(const Entity& e);

	private:

		std::vector<std::vector<std::unique_ptr<Component>>> componentEntityTable;
		u_int16_t nextComponentTypeId = 0;
		std::unordered_map<std::type_index, u_int16_t> componentToIndex;

		// not used
		std::vector<ComponentBits> entityComponentBits;

		template <typename C,
			typename std::enable_if<std::is_base_of<Component, C>::value>::type* = nullptr>
		void ensureRegistered() {

			auto& Component = typeid(C);
			if (componentToIndex.count(Component) == 0) {
				CLOG("registering component type " << nextComponentTypeId << "="<< typeid(C).name());
				componentToIndex[Component] = nextComponentTypeId++;
			}
		}

};



}
