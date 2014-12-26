#include "gtest/gtest.h"
#include "arethusa.h"
#include "../src/World.h"

class ObserverManager : public ecs::Manager {
  public:
    bool wasInitialized = false;
    bool wasAdded = false;
    bool wasRemoved = false;
    bool wasUpdated = false;

    ObserverManager(ecs::World* world) : Manager(world) {}
    virtual void initialize() {
        wasInitialized = true;
    }

    virtual void added(__attribute__((__unused__)) ecs::Entity e) override {
        wasAdded = true;
    }

    virtual void removed(__attribute__((__unused__)) ecs::Entity e) override {
        wasRemoved = true;
    }

    virtual void updated(__attribute__((__unused__)) ecs::Entity e) override {
        wasUpdated = true;
    }
};

TEST(Manager, Observers) {
    ecs::World w;
    ObserverManager& om = w.setManager<ObserverManager>();
    w.initialize();
    ASSERT_TRUE(om.wasInitialized);
    ASSERT_FALSE(om.wasAdded);
    ASSERT_FALSE(om.wasRemoved);
    ASSERT_FALSE(om.wasUpdated);

    auto& e = w.createEntity().getEntity();
    w.process();
    ASSERT_TRUE(om.wasAdded);
    ASSERT_FALSE(om.wasRemoved);
    ASSERT_FALSE(om.wasUpdated);

    w.edit(e);
    w.process();
    ASSERT_TRUE(om.wasUpdated);
    ASSERT_FALSE(om.wasRemoved);

    w.deleteEntity(e);
    w.process();
    ASSERT_TRUE(om.wasRemoved);
}
