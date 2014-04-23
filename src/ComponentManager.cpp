#include "Constants.h"
#include "ComponentManager.h"

namespace es {

void ComponentManager::clear(Entity& e) {
	CLOG("removing all components for " << e);

	auto& components = e.componentBits;
	for (uint componentBit = 0; components.size() > componentBit; componentBit++) {
		if (components[componentBit])
			componentEntityTable[componentBit][e.id] = nullptr;
	}
	components.reset();
}

}
