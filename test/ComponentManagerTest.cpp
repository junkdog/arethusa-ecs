#include <src/ComponentManager.h>
#include "gtest/gtest.h"
#include "arethusa.h"

struct Position : public es::Component {
    Position(float x, float y) : x(x), y(y) {
    }

    float x, y;
 };

struct Velocity : public es::Component {
    Velocity(float x, float y) : x(x), y(y) {
    }

    float x, y;
 };

 struct Sprite : public es::Component {
    Sprite(int id) : id(id) {
    }

    int id;
 };

TEST(ComponentManager, AddRetrieveComponents) {
    es::World world;
    world.initialize();

    es::Entity& e = world.createEntity();
    es::Entity& e2 = world.createEntity();
    es::Entity& e3 = world.createEntity();

    auto& cm = world.components();
    cm.set<Position>(e, 1.0, 2.0);
    cm.set<Velocity>(e, 3.0, 4.0);
    cm.set<Sprite>(e, 1);
    cm.set<Sprite>(e2, 2);
    cm.set<Position>(e3, 5.0, 6.0);
    cm.set<Sprite>(e3, 3);

    world.process();

    ASSERT_FLOAT_EQ(cm.get<Position>(e).x, 1.0);
    ASSERT_FLOAT_EQ(cm.get<Position>(e).y, 2.0);

    ASSERT_FLOAT_EQ(cm.get<Velocity>(e).x, 3.0);
    ASSERT_FLOAT_EQ(cm.get<Velocity>(e).y, 4.0);

    ASSERT_FLOAT_EQ(cm.get<Position>(e3).x, 5.0);
    ASSERT_FLOAT_EQ(cm.get<Position>(e3).y, 6.0);

    ASSERT_EQ(cm.get<Sprite>(e).id, 1);
    ASSERT_EQ(cm.get<Sprite>(e2).id, 2);
    ASSERT_EQ(cm.get<Sprite>(e3).id, 3);
}

TEST(ComponentManager, ComponentBits) {
    es::World world;
    world.initialize();

    es::Entity& e = world.createEntity();
    es::Entity& e2 = world.createEntity();
    es::Entity& e3 = world.createEntity();

    auto& cm = world.components();
    cm.set<Position>(e, 1.0, 2.0);
    cm.set<Velocity>(e, 3.0, 4.0);
    cm.set<Sprite>(e, 1);
    cm.set<Sprite>(e2, 2);
    cm.set<Position>(e3, 5.0, 6.0);
    cm.set<Sprite>(e3, 3);

    world.process();

    // sanity check
    ASSERT_NE((cm.componentBits<Position, Sprite>()), (cm.componentBits<Sprite>()));

    ASSERT_EQ((cm.componentBits<Position, Sprite, Velocity>()), cm.getComponentBits(e));
    ASSERT_EQ((cm.componentBits<Sprite>()), cm.getComponentBits(e2));
    ASSERT_EQ((cm.componentBits<Position, Sprite>()), cm.getComponentBits(e3));
}

TEST(ComponentManager, Components) {
    es::World world;
    world.initialize();

    es::Entity& e = world.createEntity();
    es::Entity& e2 = world.createEntity();
    es::Entity& e3 = world.createEntity();

    auto& cm = world.components();
    cm.set<Position>(e, 1.0, 2.0);
    cm.set<Velocity>(e, 3.0, 4.0);
    cm.set<Sprite>(e, 1);
    cm.set<Sprite>(e2, 2);
    cm.set<Position>(e3, 5.0, 6.0);
    cm.set<Sprite>(e3, 3);

    world.process();

    // sanity check
    ASSERT_EQ(1, (cm.get<Position>(e).x));
    ASSERT_EQ(2, (cm.get<Position>(e).y));
    ASSERT_EQ(3, (cm.get<Velocity>(e).x));
    ASSERT_EQ(4, (cm.get<Velocity>(e).y));
    ASSERT_EQ(5, (cm.get<Position>(e3).x));
    ASSERT_EQ(6, (cm.get<Position>(e3).y));

    ASSERT_EQ(1, (cm.get<Sprite>(e).id));
    ASSERT_EQ(2, (cm.get<Sprite>(e2).id));
    ASSERT_EQ(3, (cm.get<Sprite>(e3).id));
}
