#include "gtest/gtest.h"
#include "arethusa.h"
#include <iostream>

using namespace ecs;

struct Position : public ecs::Component {
    Position(float x = 0, float y = 0) : x(x), y(y) {}

    float x, y;
};

struct Velocity : public ecs::Component {
    Velocity(float x = 0, float y = 0) : x(x), y(y) {}

    float x, y;
};

struct Sprite : public ecs::Component {
    Sprite(int id = 0) : id(id) {
    }

    int id;
};

class PositionSystem :  public ecs::EntitySystem<PositionSystem> {
public:
    PositionSystem(ecs::World* world)
        : EntitySystem(world, world->components().componentBits<Position>()) {}
    virtual ~PositionSystem() = default;

    void processEntity(__attribute__((__unused__))  ecs::Entity e) {}
};

class VelocitySystem :  public ecs::EntitySystem<VelocitySystem> {
public:
    VelocitySystem(ecs::World* world)
        : EntitySystem(world, world->components().componentBits<Position, Velocity>()) {}
    virtual ~VelocitySystem() = default;

    void processEntity(__attribute__((__unused__)) ecs::Entity e) {}
};

class MapperSystem :  public ecs::EntitySystem<MapperSystem> {
public:
    MapperSystem(ecs::World* world) :
            EntitySystem(world, world->components().componentBits<Sprite>()),
            sprite(world) {}
    virtual ~MapperSystem() {};

    ecs::Mapper<Sprite> sprite;
    void processEntity(ecs::Entity e) {
        sprite[e].id++;
    }
};


TEST(EntitySystem, ProcessSystem) {
    ecs::World world;
    PositionSystem& ps = world.systems().set<PositionSystem>();
    VelocitySystem& vs = world.systems().set<VelocitySystem>();
    world.initialize();

    ecs::Entity e = world.createEntity();
    world.components().set<Position>(e);
    world.createEntity();
    ecs::Entity e3 = world.createEntity();
    world.components().set<Position>(e3);
    world.components().set<Velocity>(e3);
    world.createEntity();
    ecs::Entity e5 = world.createEntity();
    world.components().set<Position>(e5);

    ASSERT_EQ(0u, ps.getActiveCount());
    ASSERT_EQ(0u, vs.getActiveCount());
    world.process();
    ASSERT_EQ(3u, ps.getActiveCount());
    ASSERT_EQ(1u, vs.getActiveCount());

    ecs::Entity e6 = world.createEntity();
    world.components().set<Position>(e6);
    world.process();
    ASSERT_EQ(4u, ps.getActiveCount());
    ASSERT_EQ(1u, vs.getActiveCount());
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
    ASSERT_EQ(3u, ps.getActiveCount());

    world.deleteEntity(e2);
    world.process();
    ASSERT_EQ(2u, ps.getActiveCount());

    world.deleteEntity(e);
    world.deleteEntity(e3);
    world.process();
    ASSERT_EQ(0u, ps.getActiveCount());
}


TEST(EntitySystem, Mappers) {
    ecs::World world;
    world.systems().set<MapperSystem>();
    world.initialize();

    ecs::ComponentManager& cm = world.components();
    auto e1 = world.createEntity();
    cm.set<Sprite>(e1, 1);
    auto e2 = world.createEntity();
    cm.set<Sprite>(e2, 2);

    ASSERT_EQ(1, cm.get<Sprite>(e1).id);
    ASSERT_EQ(2, cm.get<Sprite>(e2).id);

    world.process();
    world.process();
    world.process();

    ASSERT_EQ(4, cm.get<Sprite>(e1).id);
    ASSERT_EQ(5, cm.get<Sprite>(e2).id);
}
