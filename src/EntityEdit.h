#pragma once

#include "Constants.h"
#include "ComponentManager.h"
#include "EditProcessor.h"

namespace ecs {

	enum class EntityState {
		CHANGE = 0, CREATE = 1, DELETE = 2};

	class EntityEdit {
	friend class EditProcessor;

	public:
		EntityEdit(ComponentManager* cm, Entity e, EntityState state = EntityState::CHANGE) : cm(cm), entity(e), state(state) {
			componentBits = &(cm->getComponentBits(e));
		}
		~EntityEdit() = default;

		template<typename C, typename ... Args, typename enable_if_component<C>::type* = nullptr>
		C& set(Args&& ... args) {
			auto cid = cm->store.index<C>();

			(*componentBits)[cid] = true;
			cm->store.getComponents<C>()[entity.getId()] = C(std::forward<Args>(args) ...);

			return cm->store.getComponents<C>()[entity.getId()];
		}

		template<typename C, typename enable_if_component<C>::type* = nullptr>
		void unset() {
			u_int16_t cid = cm->store.index<C>();
			(*componentBits)[cid] = false;
			cm->store.getComponents<C>()[entity.getId()] = {};
		}

		const Entity getEntity() {
			return entity;
		}

	private:
		ComponentManager* cm;
		Entity entity;
		ComponentBits* componentBits;
		EntityState state;
	};
}
