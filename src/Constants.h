#pragma once

#include <iostream>
#include <bitset>
#include <typeinfo>
#include <cstring>

namespace es {

	const int MAX_MANAGERS = 16;
	const int MAX_SYSTEMS = 16;
	const int MAX_COMPONENTS = 16;
	const int MAX_ENTITIES = 2048;

	using ComponentBits = std::bitset<MAX_COMPONENTS>;
	using SystemBits = std::bitset<MAX_SYSTEMS>;
	using EntityBits = std::bitset<MAX_ENTITIES>;
}

