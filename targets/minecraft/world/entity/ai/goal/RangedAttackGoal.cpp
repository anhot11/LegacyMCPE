#include "RangedAttackGoal.h"

#include "minecraft/util/Mth.h"
#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/Mob.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/control/LookControl.h"
#include "minecraft/world/entity/ai/navigation/PathNavigation.h"
#include "minecraft/world/entity/ai/sensing/Sensing.h"
#include "minecraft/world/entity/monster/RangedAttackMob.h"
#include "minecraft/world/phys/AABB.h"

void RangedAttackGoal::_init(RangedAttackMob* rangedMob, Mob* mob,
                             double speedModifier, int attackIntervalMin,
                             int attackIntervalMax, float attackRadius) {
    // if (!(mob instanceof LivingEntity))
    //{
    // throw new IllegalArgumentException("ArrowAttackGoal requires Mob
    // implements RangedAttackMob");
    // }
    rangedAttackMob = rangedMob;
    this->mob = mob;
    this->speedModifier = speedModifier;
    this->attackIntervalMin = attackIntervalMin;
    this->attackIntervalMax = attackIntervalMax;
    this->attackRadius = attackRadius;
    attackRadiusSqr = attackRadius * attackRadius;
    setRequiredControlFlags(Control::MoveControlFlag |
                            Control::LookControlFlag);

    target = std::weak_ptr<LivingEntity>();
    attackTime = -1;
    seeTime = 0;
}

RangedAttackGoal::RangedAttackGoal(RangedAttackMob* rangedMob, Mob* mob,
                                   double speedModifier, int attackInterval,
                                   float attackRadius) {
    _init(rangedMob, mob, speedModifier, attackInterval, attackInterval,
          attackRadius);
}

RangedAttackGoal::RangedAttackGoal(RangedAttackMob* rangedMob, Mob* mob,
                                   double speedModifier, int attackIntervalMin,
                                   int attackIntervalMax, float attackRadius) {
    _init(rangedMob, mob, speedModifier, attackIntervalMin, attackIntervalMax,
          attackRadius);
}

bool RangedAttackGoal::canUse() {
    std::shared_ptr<LivingEntity> bestTarget = mob->getTarget();
    if (bestTarget == nullptr) return false;
    target = std::weak_ptr<LivingEntity>(bestTarget);
    return true;
}

bool RangedAttackGoal::canContinueToUse() {
    return canUse() || !mob->getNavigation()->isDone();
}

void RangedAttackGoal::stop() {
    target = std::weak_ptr<LivingEntity>();
    seeTime = 0;
    attackTime = -1;
}

void RangedAttackGoal::tick() {
    // 4J: It's possible the target has gone since canUse selected it, don't do
    // tick if target is null
    if (target.lock() == nullptr) return;

    double targetDistSqr = mob->distanceToSqr(
        target.lock()->x, target.lock()->bb.y0, target.lock()->z);
    bool canSee = mob->getSensing()->canSee(target.lock());

    if (canSee) {
        seeTime++;
    } else {
        seeTime = 0;
    }

    if (targetDistSqr > attackRadiusSqr || seeTime < 20) {
        mob->getNavigation()->moveTo(target.lock(), speedModifier);
    } else {
        mob->getNavigation()->stop();
    }

    mob->getLookControl()->setLookAt(target.lock(), 30, 30);

    if (--attackTime == 0) {
        if (targetDistSqr > attackRadiusSqr || !canSee) return;

        float dist = Mth::sqrt(targetDistSqr) / attackRadius;
        float power = dist;
        if (power < 0.1f) power = 0.1f;
        if (power > 1) power = 1;

        rangedAttackMob->performRangedAttack(target.lock(), power);
        attackTime = Mth::floor(dist * (attackIntervalMax - attackIntervalMin) +
                                attackIntervalMin);
    } else if (attackTime < 0) {
        float dist = Mth::sqrt(targetDistSqr) / attackRadius;
        attackTime = Mth::floor(dist * (attackIntervalMax - attackIntervalMin) +
                                attackIntervalMin);
    }
}
