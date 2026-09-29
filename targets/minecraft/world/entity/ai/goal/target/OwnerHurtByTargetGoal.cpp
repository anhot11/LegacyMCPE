#include "OwnerHurtByTargetGoal.h"

#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/PathfinderMob.h"
#include "minecraft/world/entity/TamableAnimal.h"
#include "minecraft/world/entity/ai/goal/target/TargetGoal.h"

OwnerHurtByTargetGoal::OwnerHurtByTargetGoal(TamableAnimal* tameAnimal)
    : TargetGoal(tameAnimal, 32, false) {
    this->tameAnimal = tameAnimal;
    timestamp = 0;
    setRequiredControlFlags(TargetGoal::TargetFlag);
}

bool OwnerHurtByTargetGoal::canUse() {
    if (!tameAnimal->isTame()) return false;
    std::shared_ptr<LivingEntity> owner =
        std::dynamic_pointer_cast<LivingEntity>(tameAnimal->getOwner());
    if (owner == nullptr) return false;
    ownerLastHurtBy = std::weak_ptr<LivingEntity>(owner->getLastHurtByMob());
    int ts = owner->getLastHurtByMobTimestamp();

    std::shared_ptr<LivingEntity> locked = ownerLastHurtBy.lock();
    return ts != timestamp && canAttack(locked, false) &&
           tameAnimal->wantsToAttack(locked, owner);
}

void OwnerHurtByTargetGoal::start() {
    mob->setTarget(ownerLastHurtBy.lock());

    std::shared_ptr<LivingEntity> owner =
        std::dynamic_pointer_cast<LivingEntity>(tameAnimal->getOwner());
    if (owner != nullptr) {
        timestamp = owner->getLastHurtByMobTimestamp();
    }

    TargetGoal::start();
}