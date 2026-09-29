#include "DefendVillageTargetGoal.h"

#include "java/Random.h"
#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/PathfinderMob.h"
#include "minecraft/world/entity/ai/goal/target/TargetGoal.h"
#include "minecraft/world/entity/ai/village/Village.h"
#include "minecraft/world/entity/animal/VillagerGolem.h"

DefendVillageTargetGoal::DefendVillageTargetGoal(VillagerGolem* golem)
    : TargetGoal(golem, false, true) {
    this->golem = golem;
    setRequiredControlFlags(TargetGoal::TargetFlag);
}

bool DefendVillageTargetGoal::canUse() {
    std::shared_ptr<Village> village = golem->getVillage();
    if (village == nullptr) return false;
    potentialTarget = std::weak_ptr<LivingEntity>(village->getClosestAggressor(
        std::dynamic_pointer_cast<LivingEntity>(golem->shared_from_this())));
    std::shared_ptr<LivingEntity> potTarget = potentialTarget.lock();
    if (!canAttack(potTarget, false)) {
        // look for bad players
        if (mob->getRandom()->nextInt(20) == 0) {
            potentialTarget = village->getClosestBadStandingPlayer(
                std::dynamic_pointer_cast<LivingEntity>(
                    golem->shared_from_this()));
            return canAttack(potTarget, false);
        }
        return false;
    } else {
        return true;
    }
}

void DefendVillageTargetGoal::start() {
    golem->setTarget(potentialTarget.lock());
    TargetGoal::start();
}