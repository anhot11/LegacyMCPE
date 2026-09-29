#pragma once

#include <memory>

#include "TargetGoal.h"

class TamableAnimal;
class LivingEntity;

class OwnerHurtByTargetGoal : public TargetGoal {
private:
    TamableAnimal* tameAnimal;  // Owner of this goal
    std::weak_ptr<LivingEntity> ownerLastHurtBy;
    int timestamp;

public:
    OwnerHurtByTargetGoal(TamableAnimal* tameAnimal);

    bool canUse();
    void start();
};