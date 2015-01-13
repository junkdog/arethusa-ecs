#pragma once

#include <algorithm>
#include <cassert>
#include <vector>
#include "Manager.h"
#include "Component.h"
#include "Store.h"
#include "Util.h"

namespace ecs {

class World;

class ComponentManager : public Manager {

	template<typename C, typename enable_if_component<C>::type*>
	friend class Mapper;
	friend class EntityEdit;
	friend class EntityManager;

  public:

	ComponentManager(World* world) : Manager(world) {
		entityComponentBits.resize(INITIAL_ENTITY_COUNT);
	}
	virtual ~ComponentManager() = default;

	template<typename C, typename enable_if_component<C>::type* = nullptr>
	C& get(const Entity& e) {
		return store<C>().getComponents()[e.id];
	}

	template<typename C, typename enable_if_component<C>::type* = nullptr>
	ComponentBits componentBits() {
		return {store<C>().index()};
	}

	template<typename C1, typename C2, typename ... Cn>
	ComponentBits componentBits() {
		return componentBits<C1>() | componentBits<C2, Cn...>();
	}

	template<typename C, typename enable_if_component<C>::type* = nullptr>
	std::vector<C>& getComponents() {
		return store<C>().getComponents();
	}

	void clear(const Entity e);

	ComponentBits& getComponentBits(const Entity e);

	template<typename C, typename enable_if_component<C>::type* = nullptr>
	Store<C>& store() {
		auto found = std::find_if(stores.begin(), stores.end(),
			[&](std::unique_ptr<BaseStore>& bs) -> bool {
				return typeid(*bs.get()) == typeid(Store<C>);
			});

		if (found != stores.end()) {
			return static_cast<Store<C>&>(*found->get());
		} else {
			auto componentStore = make_unique<Store<C>>(nextComponentId);
			stores.push_back(std::move(componentStore));
			nextComponentId++;

			return static_cast<Store<C>&>(*(stores.back().get()));
		}
	}

  private:
	std::vector<std::unique_ptr<BaseStore>> stores;
	std::vector<ComponentBits> entityComponentBits;
	u_int16_t nextComponentId = 0;

	void resize(u_int32_t size) {
		entityComponentBits.resize(size);
		for (auto& store : stores) {
			store.get()->resize(size);
		}
	}
};
}
