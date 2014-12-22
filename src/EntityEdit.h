#pragma once

#include "Constants.h"
#include "ComponentManager.h"
#include "EditProcessor.h"

namespace ecs {

	enum class EntityState {CHANGED = 0, CREATED = 1, DELETED = 2};

	class EntityEdit {
	friend class EditProcessor;

	public:
		EntityEdit(ComponentManager* cm, Entity e, EntityState state = EntityState::CHANGED) : cm(cm), entity(e), state(state) {
			componentBits = &(cm->getComponentBits(e));
		}
		~EntityEdit() = default;

		template<typename C, typename ... Args, typename enable_if_component<C>::type* = nullptr>
		C& set(Args&& ... args) {
			auto cid = cm->store.index<C>();

			*componentBits[cid] = true;
			cm->store.getComponents<C>()[entity.getId()] = C(std::forward<Args>(args) ...);

			return cm->store.getComponents<C>()[entity.getId()];
		}

		template<typename C, typename enable_if_component<C>::type* = nullptr>
		void unset() {
			u_int16_t cid = cm->store.index<C>();
			(*componentBits)[cid] = false;
			cm->store.getComponents<C>()[entity.getId()] = {};
		}

	private:
		ComponentManager* cm;
		Entity entity;
		ComponentBits* componentBits;
		EntityState state;
	};
}
