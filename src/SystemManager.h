#pragma once

#include <algorithm>
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

class SystemManager : public Manager {
  public:
	SystemManager(World* world) : Manager(world) {}

	~SystemManager() = default;
	void process();
	void inform(EntityStates& entitiesChangedChanged);

	template<typename T, typename ... Args, typename enable_if_system<T>::type* = nullptr>
	T& set(Args&& ... args) {
		auto system = make_unique<T>(world, std::forward<Args>(args) ...);
		T& systemRef = *system;

		systems.push_back(std::move(system));

		return systemRef;
	}

	template<typename T, typename enable_if_system<T>::type* = nullptr>
	T& get() {
		auto foundSystem = std::__find_if(systems.cbegin(), systems.cend(),
		[&](std::unique_ptr<System>& es) -> bool {
			return typeid(*es.get()) == typeid(T);
		});

		return static_cast<T&>(*foundSystem.get());
	}

	virtual void initialize() override;

  private:
	std::vector<std::unique_ptr<System>> systems;
};

}
