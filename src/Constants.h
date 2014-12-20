#pragma once

#include <iostream>
#include <vector>
#include <typeinfo>
#include <cstring>
#include "Bits.h"

namespace ecs {

	struct Entity;

	const int MAX_COMPONENTS = 2048;
	const int MAX_ENTITIES = 2048;

	using ComponentBits = Bits;
	using EntityBits = Bits;
}

