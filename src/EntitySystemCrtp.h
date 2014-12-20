#pragma once

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <vector>
#include "Constants.h"
#include "Entity.h"
#include "Component.h"

namespace ecs {

	class World;

	template<typename Derived>
	class EntitySystemCrtp {
	public:
		EntitySystemCrtp(World* world = nullptr) : world(world) {
		}

		virtual ~EntitySystemCrtp() = default;
		void insert(Entity& e) {
			if (isInterested(e) && !activeIds[e.id]) {

				actives.push_back(&e);
				activeIds[e.id] = true;
				added(e);
			}
		}
		void remove(Entity& e) {
			if (activeIds[e.id]) {
				activeIds[e.id] = false;
				// FIXME
//				auto it = std::find(actives.begin(), actives.end(), &e);
//				if (it != actives.end()) {
//					auto last = --actives.end();
//					assert(e.id == (*it)->id);
//					if (it != last) std::iter_swap(it, last);
//					actives.erase(last);
//					removed(e);
//				}
			}
		}
		virtual void update(Entity& e) {
			bool interested = isInterested(e);
			bool inSystem = activeIds[e.id];

			if (interested && !inSystem) {
				actives.push_back(&e);
				activeIds[e.id] = true;
				added(e);
			} else if (interested && inSystem) {
				updated(e);
			} else if (!interested && inSystem) {
				remove(e);
			}
		}
		virtual void begin() {};
		virtual void end() {};
		virtual void added(Entity& e);
		virtual void removed(Entity& e);
		virtual void updated(Entity& e);
		virtual bool isActive() {
			return isVoidSystem || !actives.empty();
		}

		virtual void processSystem() {
			if (!isActive()) return;

			begin();
			for (auto entity : actives) {
//				processEntity(*entity);
				static_cast<System*>(this)->processEntity(*entity);
			}
			end();
		}

		virtual void processEntity(Entity& e) = 0;

		virtual ComponentBits requiredAspect() {
			return ComponentBits();
		}

		virtual ComponentBits disallowedAspect() {
			return ComponentBits();
		}


		virtual void initialize();

		void configureAspect() {
			this->requiredComponents = this->requiredAspect();
			this->disallowedComponents = this->disallowedAspect();

			this->isVoidSystem = requiredComponents.none() && disallowedComponents.none();
		};
		uint systemBit = 0;

	protected:
		World* world;
		std::vector<Entity*> actives;
		EntityBits activeIds;

	private:
		ComponentBits requiredComponents;
		ComponentBits disallowedComponents;
		bool isVoidSystem = false;

		bool isInterested(const Entity& e)  {
//			if (isVoidSystem) return false;
//
//			auto& componentBits = world->components().getComponentBits(e);
//			if (requiredComponents != (componentBits & requiredComponents))
//				return false;
//
//			if ((disallowedComponents & componentBits).any())
//				return false;
//
			return true;
		}
	};
}
