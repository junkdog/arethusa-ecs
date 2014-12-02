#pragma once

namespace es {
	struct Component {};

	template <typename C>
	using is_component = std::is_base_of<Component, C>;

	template <typename C>
	using enable_if_component = std::enable_if<is_component<C>::value>;
}
