#include "gtest/gtest.h"
#include "arethusa.h"

struct Blaj : public ecs::Component {
	Blaj(int id = 0) : id(id) {}

	int id;
};

TEST(World, InitializeWorld) {
	ecs::World world;
	world.initialize();
}

TEST(World, RetrieveSystems) {
	ecs::World world;
	world.initialize();

	world.getManager<ecs::SystemManager>();
	world.getManager<ecs::ComponentManager>();
	world.getManager<ecs::EntityManager>();
}

TEST(World, SizeTest) {
	ASSERT_EQ((sizeof(uint32_t)), (sizeof(ecs::Entity)));
}

TEST(World, CreateDeleteEntity) {
	ecs::World world;
	world.initialize();

	ecs::EntityManager& em = world.getManager<ecs::EntityManager>();

	ASSERT_EQ(0, em.getActiveCount());
	ecs::EntityEdit& ee = world.createEntity();
	auto e = ee.getEntity();
	world.process();

	ASSERT_EQ(1, em.getActiveCount());

	world.deleteEntity(e);
	world.process();

	ASSERT_EQ(0, em.getActiveCount());
}

TEST(World, GrowEntityCount) {
	ecs::World world;
	world.initialize();

	auto& ee1 = world.createEntity();
	ee1.set<Blaj>(420);
	ecs::Entity e1 = ee1.getEntity();

	world.process();

	ASSERT_EQ(420, world.components().get<Blaj>(e1).id);

	for (auto i = 0; 3078 > i; i++) {
		world.createEntity().set<Blaj>(i);
	}

	auto& ee2 = world.createEntity();
	ee2.set<Blaj>(8192);
	ecs::Entity e2 = ee2.getEntity();

	world.process();

	ASSERT_EQ(420, world.components().get<Blaj>(e1).id);
	ASSERT_EQ(8192, world.components().get<Blaj>(e2).id);
}
