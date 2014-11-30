#include "arethusa.h"
#include <memory>
#include <cstdint>

struct Position : public es::Component {
	Position(float x, float y) : x(x), y(y) {
	}

	float x, y;
};

struct Velocity : public es::Component {
	Velocity(float x, float y) : x(x), y(y) {
	}

	float x, y;
};

struct Sprite : public es::Component {
	Sprite(int id) : id(id) {
	}

	int id;
};

void hmm() {
	es::World world;
	world.initialize();

	es::Entity& e = world.createEntity();
	es::Entity& e2 = world.createEntity();
	es::Entity& e3 = world.createEntity();

	auto& cm = world.components();
	cm.set<Position>(e, 1.0, 2.0);
	cm.set<Velocity>(e, 3.0, 4.0);
	cm.set<Sprite>(e, 1);
	cm.set<Sprite>(e2, 2);
	cm.set<Position>(e3, 5.0, 6.0);
	cm.set<Sprite>(e3, 3);

	world.process();

	cm.get<Position>(e).x;
	cm.get<Position>(e).y;

	cm.get<Velocity>(e).x;
	cm.get<Velocity>(e).y;

	cm.get<Sprite>(e).id;
}
