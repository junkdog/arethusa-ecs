 #include <src/ComponentManager.h>
#include "gtest/gtest.h"
#include "arethusa.h"

struct Ent {
    uint32_t id;
};

TEST(WorldTest, InitializeWorld) {
    es::World world;
    world.initialize();
}

TEST(WorldTest, RetrieveSystems) {
    es::World world;
    world.initialize();

    world.getManager<es::SystemManager>();
    world.getManager<es::ComponentManager>();
    world.getManager<es::EntityManager>();
}


TEST(WorldTest, SizeTest) {
    int size = sizeof(uint32_t);
    int sizeEnt = sizeof(Ent);
    ASSERT_EQ(size, sizeEnt);
}

TEST(WorldTest, CreateDeleteEntity) {
    es::World world;
    world.initialize();

    es::EntityManager& em = world.getManager<es::EntityManager>();

    ASSERT_EQ(0, em.getActiveCount());
    es::Entity e = world.createEntity();
    world.process();

    ASSERT_EQ(1, em.getActiveCount());

    world.deleteEntity(e);
    world.process();

    ASSERT_EQ(0, em.getActiveCount());
}

