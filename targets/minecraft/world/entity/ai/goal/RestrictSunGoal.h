#pragma once

#include "Goal.h"

class PathfinderMob;

class RestrictSunGoal : public Goal {
private:
    PathfinderMob* mob;

public:
    RestrictSunGoal(PathfinderMob* mob);

    bool canUse();
    void start();
    void stop();
};