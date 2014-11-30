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

    auto& cm = world.components();
    cm.set<Position>(e, 1.0, 2.0);
    cm.set<Velocity>(e, 3.0, 4.0);
    cm.set<Sprite>(e, 1);

    world.process();

    ASSERT_FLOAT_EQ(cm.get<Position>(e).x, 1.0);
    ASSERT_FLOAT_EQ(cm.get<Position>(e).y, 2.0);

    ASSERT_FLOAT_EQ(cm.get<Velocity>(e).x, 3.0);
    ASSERT_FLOAT_EQ(cm.get<Velocity>(e).y, 4.0);

    ASSERT_EQ(cm.get<Sprite>(e).id, 1);
}



