#pragma once

#include <memory>
#include <vector>
#include "Constants.h"
#include "Component.h"
#include "ComponentManager.h"
#include "Entity.h"
#include "World.h"

namespace es {

	template<typename C,
			typename std::enable_if<std::is_base_of<Component, C>::value>::type * = nullptr>
	class Mapper {


	public:
		Mapper(World *world) {
			ComponentManager &cm = world->components();
			entityComponentBits = cm.entityComponentBits;
			auto& ct = cm.componentTable<C>();
			components = &ct;
			componentBit = world->components().componentBits<C>();
		}

		virtual ~Mapper() = default;

		C &get(Entity &e) {
			Component &c = *components->at(e.id).get();
			return static_cast<C &>(c);
		}

		C &operator[](Entity &e) {
			Component &c = *components->at(e.id).get();
			return static_cast<C &>(c);
		}

		bool has(Entity &e) {
			return componentBit == (entityComponentBits[e.id] & componentBit);
		}

	private:

		std::vector<std::unique_ptr<es::Component>> *components;
		std::bitset<MAX_COMPONENTS> componentBit;
		std::vector<ComponentBits>& entityComponentBits;
	};
}
