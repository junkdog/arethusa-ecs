#pragma once

#include <vector>
#include "Constants.h"
#include "Component.h"

namespace ecs {
class Store {
  public:

	template<typename T, typename enable_if_component<T>::type* = nullptr>
	std::vector<T>& getComponents() {
		static std::vector<T> components;
		components.resize(MAX_COMPONENTS);
		return components;
	}

//		template<typename T, typename enable_if_component<T>::type* = nullptr>
//		std::bitset& getEntities() {
//			static std::bitset<MAX_ENTITIES> entityBits;
//			return entityBits;
//		}

	template<typename T, typename enable_if_component<T>::type* = nullptr>
	u_int16_t index() {
		static const u_int16_t id = nextComponentId++;
		return id;
	}

  private:

	u_int16_t nextComponentId = 0;
};

}
