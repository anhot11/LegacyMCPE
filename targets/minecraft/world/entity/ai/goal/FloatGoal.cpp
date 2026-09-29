#include "FloatGoal.h"

#include "java/Random.h"
#include "minecraft/world/entity/Mob.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/control/JumpControl.h"
#include "minecraft/world/entity/ai/navigation/PathNavigation.h"

FloatGoal::FloatGoal(Mob* mob) {
    this->mob = mob;
    setRequiredControlFlags(Control::JumpControlFlag);
    mob->getNavigation()->setCanFloat(true);
}

bool FloatGoal::canUse() { return (mob->isInWater() || mob->isInLava()); }

void FloatGoal::tick() {
    if (mob->getRandom()->nextFloat() < 0.8f) mob->getJumpControl()->jump();
}