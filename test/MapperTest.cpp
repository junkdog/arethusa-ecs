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

    ecs::ComponentManager& cm = world.components();
    auto e1 = world.createEntity();
    cm.set<Sprite>(e1, 1);
    auto e2 = world.createEntity();
    auto e3 = world.createEntity();
    auto e4 = world.createEntity();
    cm.set<Sprite>(e4, 1);

    ASSERT_TRUE(sprite.has(e1));
    ASSERT_FALSE(sprite.has(e2));
    ASSERT_FALSE(sprite.has(e3));
    ASSERT_TRUE(sprite.has(e4));
}

