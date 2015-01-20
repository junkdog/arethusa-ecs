#include "Entity.h"

namespace ecs {
u_int32_t Entity::getId() const {
	return id;
}

bool Entity::operator==(const Entity& rhs) const {
	return id == rhs.id;
}

inline std::ostream& operator<<(std::ostream& out, const Entity& e) {
	out << "Entity[" << e.getId() << "]";
	return out;
}
}
