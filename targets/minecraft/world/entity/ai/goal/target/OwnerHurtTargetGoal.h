#pragma once

#include <memory>

#include "TargetGoal.h"

class TamableAnimal;
class LivingEntity;

class OwnerHurtTargetGoal : public TargetGoal {
private:
    TamableAnimal* tameAnimal;  // Owner of this goal
    std::weak_ptr<LivingEntity> ownerLastHurt;
    int timestamp;

public:
    OwnerHurtTargetGoal(TamableAnimal* tameAnimal);

    bool canUse();
    void start();
};