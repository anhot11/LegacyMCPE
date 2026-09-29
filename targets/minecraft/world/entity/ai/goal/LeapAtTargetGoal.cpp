#include "LeapAtTargetGoal.h"

#include <math.h>

#include "java/Random.h"
#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/Mob.h"
#include "minecraft/world/entity/ai/control/Control.h"

LeapAtTargetGoal::LeapAtTargetGoal(Mob* mob, float yd) {
    target = std::weak_ptr<LivingEntity>();

    this->mob = mob;
    this->yd = yd;
    setRequiredControlFlags(Control::JumpControlFlag |
                            Control::MoveControlFlag);
}

bool LeapAtTargetGoal::canUse() {
    target = std::weak_ptr<LivingEntity>(mob->getTarget());
    if (target.lock() == nullptr) return false;
    double d = mob->distanceToSqr(target.lock());
    if (d < 2 * 2 || d > 4 * 4) return false;
    if (!mob->onGround) return false;
    if (mob->getRandom()->nextInt(5) != 0) return false;
    return true;
}

bool LeapAtTargetGoal::canContinueToUse() {
    return target.lock() != nullptr && !mob->onGround;
}

void LeapAtTargetGoal::start() {
    // TODO: move to control?
    double xdd = target.lock()->x - mob->x;
    double zdd = target.lock()->z - mob->z;
    float dd = sqrt(xdd * xdd + zdd * zdd);
    mob->xd += (xdd / dd * 0.5f) * 0.8f + mob->xd * 0.2f;
    mob->zd += (zdd / dd * 0.5f) * 0.8f + mob->zd * 0.2f;
    mob->yd = yd;
}
