#pragma once

#include <memory>
#include <vector>
#include "Constants.h"
#include "Component.h"
#include "ComponentManager.h"
#include "Entity.h"
#include "World.h"

namespace ecs {

	template<typename C, typename enable_if_component<C>::type* = nullptr>
	class Mapper {


	public:
		Mapper(World *world) {
			ComponentManager& cm = world->components();
			entityComponentBits = cm.entityComponentBits;
			components = cm.store.getComponents()<C>();
			componentBit = cm.componentBits<C>();
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

//		bool has(Entity &e) {
//			return componentBit == (entityComponentBits[e.id] & componentBit);
//		}

	private:

		std::vector<C>& components;
		ComponentBits& componentBit;
	};
}
