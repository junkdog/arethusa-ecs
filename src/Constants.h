#pragma once

#include "Bits.h"

namespace ecs {

	struct Entity;

	const int INITIAL_ENTITY_COUNT = 2048;

	using ComponentBits = Bits;
	using EntityBits = Bits;
}

