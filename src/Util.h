#pragma once

#include <typeinfo>
#include <typeindex>

namespace ecs {
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&& ... args) {
	return std::unique_ptr<T>{new T(std::forward<Args>(args)...)};
}

template<typename T>
const std::type_info& template_typeid() {
	return typeid(T);
}

}
