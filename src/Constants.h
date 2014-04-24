#ifndef ESCONFIG_H_
#define ESCONFIG_H_

#include <iostream>
#include <bitset>
#include <typeinfo>
#include <cstring>

#define __FILE_X__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define DEBUG
#ifdef DEBUG

#define LOG(var) \
	std::cout << __FILE_X__ << "(" << __LINE__ << "):  " << var << std::endl

#define ERROR(var) \
	std::cout << "ERROR " << __FILE_X__ << "(" << __LINE__ << "):  " << var << std::endl

#define SDL_ERROR(var) \
	std::string error = var; \
	error += ": "; \
	error += SDL_GetError(); \
	std::cout << "ERROR " << __FILE_X__ << "(" << __LINE__ << "):  " << error << std::endl

#define CLOG(var) \
	std::cout << __FILE_X__ << "(" << __LINE__ << "): [" <<  typeid(*this).name() << "] " << var << std::endl

#else
#define LOG(var)
#define ERROR(var)
#define SDL_ERROR(var)
#define CLOG(var)
#endif

namespace es {

	const int MAX_MANAGERS = 16;
	const int MAX_SYSTEMS = 16;
	const int MAX_COMPONENTS = 16;
	const int MAX_ENTITIES = 2048;

	using ComponentBits = std::bitset<MAX_COMPONENTS>;
	using SystemBits = std::bitset<MAX_SYSTEMS>;
}



#endif
