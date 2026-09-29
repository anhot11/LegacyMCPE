#pragma once
#include "SurvivalMode.h"

class DemoMode : public SurvivalMode {
private:
    static const int DEMO_DAYS = 5;

    bool demoHasEnded;
    int demoEndedReminder;

public:
    DemoMode(Minecraft* minecraft);
    virtual void tick();

private:
    void outputDemoReminder();

public:
    using GameMode::useItem;
    using SurvivalMode::useItemOn;

    virtual void startDestroyBlock(int x, int y, int z, int face);
    virtual void continueDestroyBlock(int x, int y, int z, int face);
    virtual bool destroyBlock(int x, int y, int z, int face);
    virtual bool useItem(std::shared_ptr<Player> player, Level* level,
                         std::shared_ptr<ItemInstance> item);
    virtual bool useItemOn(std::shared_ptr<Player> player, Level* level,
                           std::shared_ptr<ItemInstance> item, int x, int y,
                           int z, int face);
    virtual void attack(std::shared_ptr<Player> player,
                        std::shared_ptr<Entity> entity);
};
