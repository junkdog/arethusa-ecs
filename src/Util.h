#pragma once

#include <typeinfo>
#include <typeindex>
#include <memory>

namespace ecs {
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&& ... args) {
	return std::unique_ptr<T>{new T(std::forward<Args>(args)...)};
}

unsigned int rightmostBit(unsigned int value);
unsigned int leftmostBit(unsigned int value);

}
