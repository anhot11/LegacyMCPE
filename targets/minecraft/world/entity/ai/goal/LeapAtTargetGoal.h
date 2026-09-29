#pragma once

#include <memory>

#include "Goal.h"

class LivingEntity;
class Mob;

class LeapAtTargetGoal : public Goal {
private:
    Mob* mob;  // Owner of this goal
    std::weak_ptr<LivingEntity> target;
    float yd;

public:
    LeapAtTargetGoal(Mob* mob, float yd);

    virtual bool canUse();
    virtual bool canContinueToUse();
    virtual void start();
};