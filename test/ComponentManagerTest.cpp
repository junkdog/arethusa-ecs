#include "gtest/gtest.h"
#include "arethusa.h"

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

TEST(ComponentManager, AddRetrieveComponents) {
    ecs::World world;
    world.initialize();

    ecs::EntityEdit ee = world.createEntity();
    ecs::EntityEdit ee2 = world.createEntity();
    ecs::EntityEdit ee3 = world.createEntity();
    auto e = ee.getEntity();
    auto e2 = ee2.getEntity();
    auto e3 = ee3.getEntity();

    auto& cm = world.components();
    ee.set<Position>(1.0, 2.0);
    ee.set<Velocity>(3.0, 4.0);
    ee.set<Sprite>(1);
    ee2.set<Sprite>(2);
    ee3.set<Position>(5.0, 6.0);
    ee3.set<Sprite>(3);

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
    ecs::World world;
    world.initialize();

    ecs::EntityEdit ee = world.createEntity();
    ecs::EntityEdit ee2 = world.createEntity();
    ecs::EntityEdit ee3 = world.createEntity();
    auto e = ee.getEntity();
    auto e2 = ee2.getEntity();
    auto e3 = ee3.getEntity();

    auto& cm = world.components();
    ee.set<Position>(1.0, 2.0);
    ee.set<Velocity>(3.0, 4.0);
    ee.set<Sprite>(1);
    ee2.set<Sprite>(2);
    ee3.set<Position>(5.0, 6.0);
    ee3.set<Sprite>(3);

    world.process();

    // sanity check
    ASSERT_NE((cm.componentBits<Position, Sprite>()), (cm.componentBits<Sprite>()));

    ASSERT_EQ((cm.componentBits<Position, Sprite, Velocity>()), cm.getComponentBits(e));
    ASSERT_EQ((cm.componentBits<Sprite>()), cm.getComponentBits(e2));
    ASSERT_EQ((cm.componentBits<Position, Sprite>()), cm.getComponentBits(e3));
}

TEST(ComponentManager, ReadComponents) {
    ecs::World world;
    world.initialize();

    ecs::EntityEdit e = world.createEntity();
    ecs::EntityEdit e2 = world.createEntity();
    ecs::EntityEdit e3 = world.createEntity();

    auto& cm = world.components();
    e.set<Position>(1.0, 2.0);
    e.set<Velocity>(3.0, 4.0);
    e.set<Sprite>(1);
    e2.set<Sprite>(2);
    e3.set<Position>(5.0, 6.0);
    e3.set<Sprite>(3);

    world.process();

    // sanity check
    ASSERT_EQ(1, (cm.get<Position>(e.getEntity()).x));
    ASSERT_EQ(2, (cm.get<Position>(e.getEntity()).y));
    ASSERT_EQ(3, (cm.get<Velocity>(e.getEntity()).x));
    ASSERT_EQ(4, (cm.get<Velocity>(e.getEntity()).y));
    ASSERT_EQ(5, (cm.get<Position>(e3.getEntity()).x));
    ASSERT_EQ(6, (cm.get<Position>(e3.getEntity()).y));

    ASSERT_EQ(1, (cm.get<Sprite>(e.getEntity()).id));
    ASSERT_EQ(2, (cm.get<Sprite>(e2.getEntity()).id));
    ASSERT_EQ(3, (cm.get<Sprite>(e3.getEntity()).id));
}

TEST(ComponentManager, WriteComponents) {
    ecs::World world;
    world.initialize();

    ecs::EntityEdit e = world.createEntity();
    ecs::EntityEdit e2 = world.createEntity();
    ecs::EntityEdit e3 = world.createEntity();

    auto& cm = world.components();
    e.set<Position>(1.0, 2.0);
    e.set<Velocity>(3.0, 4.0);
    e.set<Sprite>(1);
    e2.set<Sprite>(2);
    e3.set<Position>(5.0, 6.0);
    e3.set<Sprite>(3);

    world.process();

    e.set<Position>(2.0, 3.0);
    e.set<Velocity>(4.0, 5.0);
    e.set<Sprite>(2);
    e2.set<Sprite>(3);
    e3.set<Position>(6.0, 7.0);
    e3.set<Sprite>(4);

    ASSERT_EQ(2, (cm.get<Position>(e.getEntity()).x));
    ASSERT_EQ(3, (cm.get<Position>(e.getEntity()).y));
    ASSERT_EQ(4, (cm.get<Velocity>(e.getEntity()).x));
    ASSERT_EQ(5, (cm.get<Velocity>(e.getEntity()).y));
    ASSERT_EQ(6, (cm.get<Position>(e3.getEntity()).x));
    ASSERT_EQ(7, (cm.get<Position>(e3.getEntity()).y));

    ASSERT_EQ(2, (cm.get<Sprite>(e.getEntity()).id));
    ASSERT_EQ(3, (cm.get<Sprite>(e2.getEntity()).id));
    ASSERT_EQ(4, (cm.get<Sprite>(e3.getEntity()).id));
}
