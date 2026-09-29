#pragma once

#include <memory>

#include "TargetGoal.h"

class VillagerGolem;
class LivingEntity;

class DefendVillageTargetGoal : public TargetGoal {
private:
    VillagerGolem* golem;  // Owner of this goal
    std::weak_ptr<LivingEntity> potentialTarget;

public:
    DefendVillageTargetGoal(VillagerGolem* golem);

    bool canUse();
    void start();
};