#include "arethusa.h"
#include "EntitySystem.h"
#include <memory>
#include <cstdint>
#include <iostream>

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

//class PositionSystem : public ecs::EntitySystem<PositionSystem> {
//public:
//	PositionSystem(ecs::World* world) : EntitySystem(world) {}
//	~PositionSystem() = default;
//
//
////	virtual ecs::ComponentBits requiredAspect() override {
////		return world->components().componentBits<Position>();
////	}
//
//	void processEntity(ecs::Entity& e) {
//		std::cout << "\tprocessEntity" << std::endl;
//	}
//};

class PositionSystem :  public ecs::EntitySystem<PositionSystem> {
public:
	PositionSystem(ecs::World* world) : EntitySystem(world) {}
	virtual ~PositionSystem() {};


	void processEntity(ecs::Entity e) {
		std::cout << "\tprocessEntity" << std::endl;
	}

	ecs::ComponentBits requiredAspect() {
		return world->components().componentBits<Position>();
	}
};

void hmm() {
	ecs::World world;
	PositionSystem& ps = world.systems().set<PositionSystem>();
	world.initialize();

	ecs::Entity e = world.createEntity();
	world.components().set<Position>(e);
	ecs::Entity e2 = world.createEntity();
	ecs::Entity e3 = world.createEntity();
	world.components().set<Position>(e3);
	ecs::Entity e4 = world.createEntity();
	ecs::Entity e5 = world.createEntity();
	world.components().set<Position>(e5);

	world.process();

	world.process();

}

