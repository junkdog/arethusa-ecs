#include "gtest/gtest.h"
#include "arethusa.h"

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
    ecs::EntityEdit e = world.createEntity();
    world.process();

    ASSERT_EQ(1, em.getActiveCount());

    world.deleteEntity(e.getEntity());
    world.process();

    ASSERT_EQ(0, em.getActiveCount());
}
