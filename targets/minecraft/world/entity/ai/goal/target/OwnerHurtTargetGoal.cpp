#include "OwnerHurtTargetGoal.h"

#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/PathfinderMob.h"
#include "minecraft/world/entity/TamableAnimal.h"
#include "minecraft/world/entity/ai/goal/target/TargetGoal.h"

OwnerHurtTargetGoal::OwnerHurtTargetGoal(TamableAnimal* tameAnimal)
    : TargetGoal(tameAnimal, 32, false) {
    this->tameAnimal = tameAnimal;
    setRequiredControlFlags(TargetGoal::TargetFlag);
    timestamp = 0;
}

bool OwnerHurtTargetGoal::canUse() {
    if (!tameAnimal->isTame()) return false;
    std::shared_ptr<LivingEntity> owner =
        std::dynamic_pointer_cast<LivingEntity>(tameAnimal->getOwner());
    if (owner == nullptr) return false;
    ownerLastHurt = std::weak_ptr<LivingEntity>(owner->getLastHurtMob());
    int ts = owner->getLastHurtMobTimestamp();
    std::shared_ptr<LivingEntity> locked = ownerLastHurt.lock();
    return ts != timestamp && canAttack(locked, false) &&
           tameAnimal->wantsToAttack(locked, owner);
}

void OwnerHurtTargetGoal::start() {
    mob->setTarget(ownerLastHurt.lock());

    std::shared_ptr<LivingEntity> owner =
        std::dynamic_pointer_cast<LivingEntity>(tameAnimal->getOwner());
    if (owner != nullptr) {
        timestamp = owner->getLastHurtMobTimestamp();

        TargetGoal::start();
    }
}