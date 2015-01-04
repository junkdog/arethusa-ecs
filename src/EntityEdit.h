#pragma once

#include "Constants.h"
#include "ComponentManager.h"
#include <memory>

namespace ecs {

	enum class EntityState {
		CHANGE = 0, CREATE = 1, DELETE = 2};

	class EntityEdit : private NoCopy {
	friend class EditProcessor;

	public:
		EntityEdit(ComponentManager* cm, Entity e, EntityState state = EntityState::CHANGE)
			: cm(cm), entity(e), componentBits(&(cm->getComponentBits(e))), state(state) {}
		~EntityEdit() = default;

		template<typename C, typename ... Args, typename enable_if_component<C>::type* = nullptr>
		C& set(Args&& ... args) {
			auto& store = cm->store<C>();
			auto cid = store.index();

			(*componentBits)[cid] = true;
			store.getComponents()[entity.getId()] = C(std::forward<Args>(args) ...);

			store.entities()[entity.getId()] = true;
			return store.getComponents()[entity.getId()];
		}

		template<typename C, typename enable_if_component<C>::type* = nullptr>
		void unset() {
			auto& store = cm->store<C>();
			u_int16_t cid = store.index();
			(*componentBits)[cid] = false;
			store.getComponents()[entity.getId()] = {};
			store.entities()[entity.getId()] = false;
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
};
