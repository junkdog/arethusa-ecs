#include "arethusa.h"
#include <memory>
#include <cstdint>

struct Position : public ecs::Component {
	Position(float x = 0, float y = 0) : x(x), y(y) {
	}

	float x, y;
};

struct Velocity : public ecs::Component {
	Velocity(float x = 0, float y = 0) : x(x), y(y) {
	}

	float x, y;
};

struct Sprite : public ecs::Component {
	Sprite(int id = 0) : id(id) {
	}

	int id;
};

void hmm() {
	ecs::World world;
	world.initialize();

	ecs::Entity& e = world.createEntity();
	ecs::Entity& e2 = world.createEntity();
	ecs::Entity& e3 = world.createEntity();

	auto& cm = world.components();
	cm.set<Position>(e, 1.0, 2.0);
	cm.set<Velocity>(e, 3.0, 4.0);
	cm.set<Sprite>(e, 1);
	cm.set<Sprite>(e2, 2);
	cm.set<Position>(e3, 5.0, 6.0);
	cm.set<Sprite>(e3, 3);

	world.process();

	cm.get<Position>(e3).x;

	cm.componentBits<Position, Sprite, Velocity>();
	cm.componentBits<Sprite>();
	cm.componentBits<Position, Sprite>();
}
