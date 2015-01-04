#include "gtest/gtest.h"
#include "arethusa.h"

TEST(Entity, Equality) {
    ecs::World world;
    world.initialize();

    auto e1 = world.createEntity().getEntity();
    auto e2 = ecs::Entity {e1.getId()};
    ASSERT_EQ(e1, e2);
}


