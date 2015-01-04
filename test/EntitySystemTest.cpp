#include "gtest/gtest.h"
#include "arethusa.h"

using namespace ecs;

struct Position : public ecs::Component {
	Position(float x = 0, float y = 0) : x(x), y(y) {}

	float x, y;
};

struct Velocity : public ecs::Component {
	Velocity(float x = 0, float y = 0) : x(x), y(y) {}

	float x, y;
};

struct Sprite : public ecs::Component {
	Sprite(int id = 0) : id(id) {
	}

	int id;
};

class PositionSystem : public ecs::EntitySystem<PositionSystem> {
  public:
	PositionSystem(ecs::World* world)
		: EntitySystem(world, world->components().componentBits<Position>()) {}

	virtual ~PositionSystem() = default;

	void processEntity(__attribute__((__unused__)) const ecs::Entity e) {}
};

class VelocitySystem : public ecs::EntitySystem<VelocitySystem> {
  public:
	VelocitySystem(ecs::World* world)
		: EntitySystem(world, world->components().componentBits<Position, Velocity>()) {}

	virtual ~VelocitySystem() = default;

	void processEntity(__attribute__((__unused__)) ecs::Entity e) {}
};

class VelToPosSystem : public ecs::EntitySystem<VelToPosSystem> {
  public:
	VelToPosSystem(ecs::World* world)
		: EntitySystem(world, world->components().componentBits<Velocity>()) {}

	virtual ~VelToPosSystem() = default;

	void processEntity(const ecs::Entity e) {
		world->edit(e).set<Position>();
		world->edit(e).unset<Velocity>();
	}
};

class MapperSystem : public ecs::EntitySystem<MapperSystem> {
  public:
	MapperSystem(ecs::World* world) :
		EntitySystem(world, world->components().componentBits<Sprite>()),
		sprite(world) {}

	virtual ~MapperSystem() {};

	ecs::Mapper<Sprite> sprite;

	void processEntity(const ecs::Entity e) {
		sprite[e].id++;
	}
};

class DeleterSystem : public ecs::EntitySystem<DeleterSystem> {
  public:
	DeleterSystem(ecs::World* world) :
		EntitySystem(world, world->components().componentBits<Position>()) {}
	virtual ~DeleterSystem() = default;

	void processEntity(const ecs::Entity e) {
		if (tick == 0)
			return;

		if (tick == 1 && e.getId() % 2 == 0) {
			world->deleteEntity(e);
			auto eNew = world->createEntity().getEntity();
			auto& cb = world->components().getComponentBits(eNew);
			ASSERT_FALSE(cb.any());
		} else if (tick > 1) {
			world->deleteEntity(e);
			auto eNew = world->createEntity().getEntity();
			auto& cb = world->components().getComponentBits(eNew);
			ASSERT_FALSE(cb.any());
		}
	}

	void end() override {
		tick++;
	}

	void removed(__attribute__((__unused__)) ecs::Entity e) {
		removedCount++;
	}

	int tick = 0;
	int removedCount = 0;
};

TEST(EntitySystem, ProcessSystem) {
	ecs::World world;
	PositionSystem& ps = world.systems().set<PositionSystem>();
	VelocitySystem& vs = world.systems().set<VelocitySystem>();
	world.initialize();

	ecs::EntityEdit& e = world.createEntity();
	e.set<Position>();
	world.createEntity();
	ecs::EntityEdit& e3 = world.createEntity();
	e3.set<Position>();
	e3.set<Velocity>();
	world.createEntity();
	ecs::EntityEdit& e5 = world.createEntity();
	e5.set<Position>();

	ASSERT_EQ(0u, ps.getActiveCount());
	ASSERT_EQ(0u, vs.getActiveCount());
	world.process();
	ASSERT_EQ(3u, ps.getActiveCount());
	ASSERT_EQ(1u, vs.getActiveCount());

	ecs::EntityEdit& e6 = world.createEntity();
	e6.set<Position>();
	world.process();
	ASSERT_EQ(4u, ps.getActiveCount());
	ASSERT_EQ(1u, vs.getActiveCount());
}


TEST(EntitySystem, EntityDeleted) {
	ecs::World world;
	PositionSystem& ps = world.systems().set<PositionSystem>();
	world.initialize();

	ecs::EntityEdit& ee = world.createEntity();
	auto e = ee.getEntity();
	ee.set<Position>();
	ecs::EntityEdit& ee2 = world.createEntity();
	auto e2 = ee2.getEntity();
	ee2.set<Position>();
	ecs::EntityEdit& ee3 = world.createEntity();
	auto e3 = ee3.getEntity();
	ee3.set<Position>();

	world.process();
	ASSERT_EQ(3u, ps.getActiveCount());

	world.deleteEntity(e2);
	world.process();
	ASSERT_EQ(2u, ps.getActiveCount());

	world.deleteEntity(e);
	world.deleteEntity(e3);
	world.process();
	ASSERT_EQ(0u, ps.getActiveCount());
}

TEST(EntitySystem, EntityDeletedInSystem) {
	ecs::World world;
	world.systems().set<VelToPosSystem>();
	PositionSystem& ps = world.systems().set<PositionSystem>();
	DeleterSystem& ds = world.systems().set<DeleterSystem>();
	world.initialize();

	world.createEntity().set<Position>();
	world.createEntity().set<Velocity>();
	world.createEntity();
	world.createEntity();
	world.createEntity().set<Position>();
	world.createEntity().set<Position>();

	world.process();
	ASSERT_EQ(4u, ds.getActiveCount());
	ASSERT_EQ(4u, ps.getActiveCount());
	ASSERT_EQ(0, ds.removedCount);

	world.process();
	ASSERT_EQ(2u, ds.getActiveCount());
	ASSERT_EQ(2u, ps.getActiveCount());
	ASSERT_EQ(2, ds.removedCount);

	world.process();
	ASSERT_EQ(0u, ds.getActiveCount());
	ASSERT_EQ(0u, ps.getActiveCount());
	ASSERT_EQ(4, ds.removedCount);
}

TEST(EntitySystem, EntityRemoved) {
	ecs::World world;
	PositionSystem& ps = world.systems().set<PositionSystem>();
	world.initialize();

	ecs::EntityEdit& e = world.createEntity();
	e.set<Position>();
	auto entity = e.getEntity();
	ecs::EntityEdit& e2 = world.createEntity();
	e2.set<Position>();
	auto entity2 = e2.getEntity();
	ecs::EntityEdit& e3 = world.createEntity();
	e3.set<Position>();
	auto entity3 = e3.getEntity();

	world.process();
	ASSERT_EQ(3u, ps.getActiveCount());

	world.edit(entity2).unset<Position>();
	world.process();
	ASSERT_EQ(2u, ps.getActiveCount());

	world.edit(entity).unset<Position>();
	world.edit(entity3).unset<Position>();
	world.process();
	ASSERT_EQ(0u, ps.getActiveCount());
}


TEST(EntitySystem, Mappers) {
	ecs::World world;
	world.systems().set<MapperSystem>();
	world.initialize();

	ecs::ComponentManager& cm = world.components();
	auto& ee1 = world.createEntity();
	auto e1 = ee1.getEntity();
	ee1.set<Sprite>(1);
	auto& ee2 = world.createEntity();
	auto e2 = ee2.getEntity();
	ee2.set<Sprite>(2);

	ASSERT_EQ(1, cm.get<Sprite>(e1).id);
	ASSERT_EQ(2, cm.get<Sprite>(e2).id);

	world.process();
	world.process();
	world.process();

	ASSERT_EQ(4, cm.get<Sprite>(e1).id);
	ASSERT_EQ(5, cm.get<Sprite>(e2).id);
}
