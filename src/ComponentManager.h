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

		template<typename C, typename ... Args, typename enable_if_component<C>::type* = nullptr>
		C& set(Entity& e, Args&& ... args) {
			auto cid = store.index<C>();

			entityComponentBits[e.id].set(cid, true);
			store.getComponents<C>()[e.id] = C(std::forward<Args>(args) ...);

			return store.getComponents<C>()[e.id];
		}


		template<typename C, typename enable_if_component<C>::type* = nullptr>
		void remove(Entity& e) {

			u_int16_t cid = store.index<C>();
			entityComponentBits[e.id].set(cid, false);
			store.getComponents<C>()[e.id] = {};
		}

		template<typename C, typename enable_if_component<C>::type* = nullptr>
		C& get(const Entity& e) {
			return store.getComponents<C>()[e.id];
		}

		template<typename C, typename enable_if_component<C>::type* = nullptr>
		ComponentBits componentBits() {

			ComponentBits aspect;
			aspect[store.index<C>()] = true;
			return aspect;
		}

		template<typename C1, typename C2, typename ... Cn>
		ComponentBits componentBits() {
			return componentBits<C1>() | componentBits<C2, Cn...>();
		}

		template<typename C, typename enable_if_component<C>::type* = nullptr>
		std::vector<C>& getComponents() {
			return store.getComponents<C>();
		}

		void clear(const Entity e);

		ComponentBits& getComponentBits(const Entity& e);

	private:
		Store store;
		std::vector<ComponentBits> entityComponentBits;
	};
}
