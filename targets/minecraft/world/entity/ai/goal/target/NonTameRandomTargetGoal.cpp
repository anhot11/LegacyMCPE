#include "NonTameRandomTargetGoal.h"

#include "minecraft/world/entity/TamableAnimal.h"
#include "minecraft/world/entity/ai/goal/target/NearestAttackableTargetGoal.h"

NonTameRandomTargetGoal::NonTameRandomTargetGoal(
    TamableAnimal* mob, const std::type_info& targetType, int randomInterval,
    bool mustSee)
    : NearestAttackableTargetGoal(mob, targetType, randomInterval, mustSee) {
    tamableMob = mob;
}

bool NonTameRandomTargetGoal::canUse() {
    return !tamableMob->isTame() && NearestAttackableTargetGoal::canUse();
}
