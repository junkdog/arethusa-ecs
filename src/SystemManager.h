#pragma once

#include <algorithm>
#include <cassert>
#include <unordered_map>
#include "Constants.h"
#include "System.h"
#include "Manager.h"
#include "EditProcessor.h"
#include "Util.h"

namespace ecs {

template<typename T>
using is_system = std::is_base_of<System, T>;

template<typename T>
using enable_if_system = std::enable_if<is_system<T>::value>;

template<typename T>
using enable_if_system_t = typename enable_if_system<T>::type*;

class SystemManager : public Manager {
  public:
	SystemManager(World* world) : Manager(world) {}

	~SystemManager() = default;
	void process();
	void inform(EntityStates& entitiesChangedChanged);

	template<typename T, typename ... Args, enable_if_system_t<T> = nullptr>
	T& set(Args&& ... args) {
		auto system = make_unique<T>(world, std::forward<Args>(args) ...);
		systems.push_back(std::move(system));

		return static_cast<T&>(*systems.back());
	}

	template<typename T, enable_if_system_t<T> = nullptr>
	T& get() {
		auto foundSystem = std::find_if(systems.cbegin(), systems.cend(),
		[&](const std::unique_ptr<System>& es) -> bool {
			return typeid(*es.get()) == typeid(T);
		});

		assert(foundSystem != systems.cend());
		return static_cast<T&>(**foundSystem);
	}

	virtual void initialize() override;

  private:
	std::vector<std::unique_ptr<System>> systems;
};

}
