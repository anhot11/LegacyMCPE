#pragma once

class RangedAttackMob {
public:
    virtual void performRangedAttack(std::shared_ptr<LivingEntity> target,
                                     float power) = 0;
};