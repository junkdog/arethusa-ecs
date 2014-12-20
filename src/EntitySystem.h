#pragma once

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <vector>
#include "Constants.h"
#include "Entity.h"
#include "Component.h"
#include "System.h"

namespace ecs {

class World;

template<typename Derived>
class EntitySystem : public System {
	public:
		EntitySystem(World* world) : System(world) {}
		virtual ~EntitySystem() = default;

		virtual void added(__attribute__((__unused__)) Entity e) {};
		virtual void removed(__attribute__((__unused__)) Entity e) {};
		virtual void updated(__attribute__((__unused__)) Entity e) {};
		unsigned int getActiveCount() const {
			return actives.size();
		}

		void processEntity(Entity e);
		virtual void processSystem() {

			if (!isActive()) return;

			begin();
			for (auto entity : actives) {
				static_cast<Derived*>(this)->processEntity(entity);
			}
			end();
		}

		unsigned int systemBit = 0;
};
}
