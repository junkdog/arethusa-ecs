#pragma once

#include <iostream>
#include <bitset>
#include <vector>
#include <typeinfo>
#include <cstring>

namespace ecs {

	const int MAX_COMPONENTS = 16;
	const int MAX_ENTITIES = 2048;

	using ComponentBits = std::bitset<MAX_COMPONENTS>;
	using EntityBits = std::bitset<MAX_ENTITIES>;
}

