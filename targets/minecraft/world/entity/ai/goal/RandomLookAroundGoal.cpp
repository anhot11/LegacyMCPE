#include "RandomLookAroundGoal.h"

#include <math.h>

#include <numbers>

#include "java/Random.h"
#include "minecraft/world/entity/Mob.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/control/LookControl.h"

RandomLookAroundGoal::RandomLookAroundGoal(Mob* mob) {
    relX = relZ = 0.0;
    lookTime = 0;

    this->mob = mob;
    setRequiredControlFlags(Control::MoveControlFlag |
                            Control::LookControlFlag);
}

bool RandomLookAroundGoal::canUse() {
    return mob->getRandom()->nextFloat() < 0.02f;
}

bool RandomLookAroundGoal::canContinueToUse() { return lookTime >= 0; }

void RandomLookAroundGoal::start() {
    double rnd = 2 * std::numbers::pi * mob->getRandom()->nextDouble();
    relX = cos(rnd);
    relZ = sin(rnd);
    lookTime = 20 + mob->getRandom()->nextInt(20);
}

void RandomLookAroundGoal::tick() {
    --lookTime;
    mob->getLookControl()->setLookAt(mob->x + relX,
                                     mob->y + mob->getHeadHeight(),
                                     mob->z + relZ, 10, mob->getMaxHeadXRot());
}