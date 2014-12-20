#include "gtest/gtest.h"
#include "arethusa.h"
#include <iostream>

using namespace ecs;

struct Position : public ecs::Component {
    Position(float x = 0, float y = 0) : x(x), y(y) {
    }

    float x, y;
};


class PositionSystem :  public ecs::EntitySystem<PositionSystem> {
public:
    PositionSystem(ecs::World* world) : EntitySystem(world) {}
    virtual ~PositionSystem() = default;


    void processEntity(ecs::Entity e) {}

    ecs::ComponentBits requiredAspect() {
        return world->components().componentBits<Position>();
    }
};


TEST(EntitySystem, ProcessSystem) {
    ecs::World world;
    PositionSystem& ps = world.systems().set<PositionSystem>();
    world.initialize();

    ecs::Entity e = world.createEntity();
    world.components().set<Position>(e);
    ecs::Entity e2 = world.createEntity();
    ecs::Entity e3 = world.createEntity();
    world.components().set<Position>(e3);
    ecs::Entity e4 = world.createEntity();
    ecs::Entity e5 = world.createEntity();
    world.components().set<Position>(e5);

    ASSERT_EQ(0, ps.getActiveCount());
    world.process();
    ASSERT_EQ(3, ps.getActiveCount());
}


TEST(EntitySystem, EntityDeleted) {
    ecs::World world;
    PositionSystem& ps = world.systems().set<PositionSystem>();
    world.initialize();

    ecs::Entity e = world.createEntity();
    world.components().set<Position>(e);
    ecs::Entity e2 = world.createEntity();
    world.components().set<Position>(e2);
    ecs::Entity e3 = world.createEntity();
    world.components().set<Position>(e3);

    world.process();
    ASSERT_EQ(3, ps.getActiveCount());

    world.deleteEntity(e2);
    world.process();
    ASSERT_EQ(2, ps.getActiveCount());

    world.deleteEntity(e);
    world.deleteEntity(e3);
    world.process();
    ASSERT_EQ(0, ps.getActiveCount());
}
