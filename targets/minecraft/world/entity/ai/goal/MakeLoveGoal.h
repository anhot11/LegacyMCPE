#pragma once

#include <memory>

#include "Goal.h"

class Villager;
class Village;
class Level;

class MakeLoveGoal : public Goal {
private:
    Villager* villager;  // Owner of this goal
    std::weak_ptr<Villager> partner;
    Level* level;
    int loveMakingTime;
    std::weak_ptr<Village> village;

public:
    MakeLoveGoal(Villager* villager);

    bool canUse();
    void start();
    void stop();
    bool canContinueToUse();
    void tick();

private:
    bool villageNeedsMoreVillagers();
    void breed();

public:
    // 4J Added override to update ai elements when loading entity from
    // schematics
    virtual void setLevel(Level* level) { this->level = level; }
};