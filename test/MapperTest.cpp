#include "gtest/gtest.h"
#include "arethusa.h"


 struct Sprite : public ecs::Component {
    Sprite(int id = 0) : id(id) {
    }

    int id;
 };

TEST(Mapper, HasComponent) {
    ecs::World world;
    world.initialize();

    ecs::Mapper<Sprite> sprite(&world);

    auto ee1 = world.createEntity();
    ee1.set<Sprite>(1);
    auto e1 = ee1.getEntity();
    auto e2 = world.createEntity().getEntity();
    auto e3 = world.createEntity().getEntity();
    auto ee4 = world.createEntity();
    ee4.set<Sprite>(1);
    auto e4 = ee4.getEntity();

    ASSERT_TRUE(sprite.has(e1));
    ASSERT_FALSE(sprite.has(e2));
    ASSERT_FALSE(sprite.has(e3));
    ASSERT_TRUE(sprite.has(e4));
}

