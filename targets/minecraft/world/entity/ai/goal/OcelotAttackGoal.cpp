#include "OcelotAttackGoal.h"

#include <algorithm>

#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/Mob.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/control/LookControl.h"
#include "minecraft/world/entity/ai/navigation/PathNavigation.h"
#include "minecraft/world/entity/animal/Ocelot.h"
#include "minecraft/world/phys/AABB.h"

OcelotAttackGoal::OcelotAttackGoal(Mob* mob) {
    target = std::weak_ptr<LivingEntity>();
    attackTime = 0;
    speed = 0;
    trackTarget = false;

    this->mob = mob;
    this->level = mob->level;
    setRequiredControlFlags(Control::MoveControlFlag |
                            Control::LookControlFlag);
}

bool OcelotAttackGoal::canUse() {
    std::shared_ptr<LivingEntity> bestTarget = mob->getTarget();
    if (bestTarget == nullptr) return false;
    target = std::weak_ptr<LivingEntity>(bestTarget);
    return true;
}

bool OcelotAttackGoal::canContinueToUse() {
    if (target.lock() == nullptr || !target.lock()->isAlive()) return false;
    if (mob->distanceToSqr(target.lock()) > 15 * 15) return false;
    return !mob->getNavigation()->isDone() || canUse();
}

void OcelotAttackGoal::stop() {
    target = std::weak_ptr<Mob>();
    mob->getNavigation()->stop();
}

void OcelotAttackGoal::tick() {
    mob->getLookControl()->setLookAt(target.lock(), 30, 30);

    double meleeRadiusSqr = (mob->bbWidth * 2) * (mob->bbWidth * 2);
    double distSqr = mob->distanceToSqr(target.lock()->x, target.lock()->bb.y0,
                                        target.lock()->z);

    double speedModifier = Ocelot::WALK_SPEED_MOD;
    if (distSqr > meleeRadiusSqr && distSqr < 4 * 4)
        speedModifier = Ocelot::SPRINT_SPEED_MOD;
    else if (distSqr < 15 * 15)
        speedModifier = Ocelot::SNEAK_SPEED_MOD;

    mob->getNavigation()->moveTo(target.lock(), speedModifier);

    attackTime = std::max(attackTime - 1, 0);

    if (distSqr > meleeRadiusSqr) return;
    if (attackTime > 0) return;
    attackTime = 20;
    mob->doHurtTarget(target.lock());
}
