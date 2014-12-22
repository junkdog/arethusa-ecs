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

    auto e1 = world.createEntity();
    e1.set<Sprite>(1);
    auto e2 = world.createEntity().getEntity();
    auto e3 = world.createEntity().getEntity();
    auto e4 = world.createEntity();
    e4.set<Sprite>(1);

    ASSERT_TRUE(sprite.has(e1.getEntity()));
    ASSERT_FALSE(sprite.has(e2));
    ASSERT_FALSE(sprite.has(e3));
    ASSERT_TRUE(sprite.has(e4.getEntity()));
}

