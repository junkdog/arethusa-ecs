#pragma once

#include <cassert>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <vector>
#include <unordered_map>
#include "Manager.h"
#include "Component.h"
#include "Store.h"

namespace ecs {

	class World;

	class ComponentManager : public Manager {

	friend class Mapper;

	public:

		ComponentManager(World* world) : Manager(world) {
			entityComponentBits.resize(MAX_ENTITIES);
		}

		virtual ~ComponentManager() = default;

		template<typename T, typename ... Args, typename enable_if_component<T>::type* = nullptr>
		T& set(Entity& e, Args&& ... args) {
			auto cid = componentId<T>();

			entityComponentBits[e.id].set(cid, true);
			store.getComponents<T>()[e.id] = T(std::forward<Args>(args) ...);

			return store.getComponents<T>()[e.id];
		}


		template<typename T, typename enable_if_component<T>::type* = nullptr>
		void remove(Entity& e) {

			assert(componentToIndex.count(typeid(T)) == 1);

			u_int16_t cid = componentId<T>();
			entityComponentBits[e.id].set(cid, false);
			store.getComponents<T>() = {};
		}

		template<typename T, typename enable_if_component<T>::type* = nullptr>
		T& get(const Entity& e) {
			return store.getComponents<T>()[e.id];
		}

		template<typename C, typename enable_if_component<C>::type* = nullptr>
		ComponentBits componentBits() {

			ComponentBits aspect;

			auto& Component = typeid(C);
			componentId<C>();

			assert(componentToIndex.count(Component) == 1);
			aspect[componentToIndex[Component]] = true;
			return aspect;;
		}

		template<typename C1, typename C2, typename ... Cn>
		ComponentBits componentBits() {
			return componentBits<C1>() | componentBits<C2, Cn...>();
		}

		template<typename T, typename enable_if_component<T>::type* = nullptr>
		std::vector<T>& getComponents() {
			return store.getComponents<T>();
		}

		void clear(Entity& e);

		ComponentBits& getComponentBits(const Entity& e);

	private:

		u_int16_t nextComponentTypeId = 0;
		std::unordered_map<std::type_index, u_int16_t> componentToIndex;

		Store store;
		std::vector<ComponentBits> entityComponentBits;

		template<typename C, typename enable_if_component<C>::type* = nullptr>
		u_int16_t componentId() {
			auto& Component = typeid(C);
			if (componentToIndex.count(Component) == 0) {
				componentToIndex[Component] = nextComponentTypeId++;
			}
			return componentToIndex[Component];
		}

	};


}
