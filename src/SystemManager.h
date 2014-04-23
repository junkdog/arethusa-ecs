#ifndef SYSTEMMANAGER_H_
#define SYSTEMMANAGER_H_

#include <unordered_map>
#include "Constants.h"
#include "EntitySystem.h"
#include "World.h"

namespace es {

class SystemManager : public Manager {

	public:
		SystemManager(World* world) : Manager(world) {
			systems.reserve(MAX_COMPONENTS);
		};

		virtual ~SystemManager() {};
		void process();
		void inform(EntityStates& entitiesChangedChanged);

		template <typename T, typename ... Args,
			typename std::enable_if<std::is_base_of<EntitySystem, T>::value>::type* = nullptr>
		T& set(Args && ... args) {
			LOG("adding system " << typeid(T).name());

			auto system = std::unique_ptr<T>(new T(world, std::forward<Args>(args) ...));
			uint systemBit = (uint)systems.size();
			system->systemBit = systemBit;
			T& systemRef = *system;

			systems.push_back(std::move(system));
			systemMap[typeid(T)] = systemBit;

			return static_cast<T&>(systemRef);
		}

		template <typename T,
			typename std::enable_if<std::is_base_of<EntitySystem, T>::value>::type* = nullptr>
		T& get() {
			auto system = systems[systemMap[typeid(T)]].get();
			return static_cast<T&>(*system);
		}

		virtual void initialize() override;

	private:
		std::vector<std::unique_ptr<EntitySystem>> systems;
		std::unordered_map<std::type_index, uint> systemMap;
};

}

#endif
