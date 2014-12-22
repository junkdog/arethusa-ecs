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

//class MapperSystem : public ecs::EntitySystem<MapperSystem> {
//public:
//	MapperSystem(ecs::World* world) : EntitySystem(world) {}
//	MapperSystem = default;
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

class MapperSystem :  public ecs::EntitySystem<MapperSystem> {
public:
	MapperSystem(ecs::World* world) :
		EntitySystem(world, world->components().componentBits<Sprite>()),
		sprite(world) {}
	virtual ~MapperSystem() {};

	ecs::Mapper<Sprite> sprite;
	void processEntity(ecs::Entity e) {
		sprite[e].id++;
	}
};

void hmm() {
	ecs::World world;
	world.initialize();

	ecs::Mapper<Sprite> sprite(&world);

	ecs::ComponentManager& cm = world.components();
	auto e1 = world.createEntity();
	cm.set<Sprite>(e1, 1);
	auto e2 = world.createEntity();

	world.process();

	sprite.has(e1);

	cm.unset<Sprite>(e1);
	world.updateState(e1);
}

