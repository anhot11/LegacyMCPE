#include "LookAtPlayerGoal.h"

#include "java/Random.h"
#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/entity/Mob.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/control/LookControl.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/phys/AABB.h"

LookAtPlayerGoal::LookAtPlayerGoal(Mob* mob, const std::type_info& lookAtType,
                                   float lookDistance)
    : lookAtType(lookAtType) {
    this->mob = mob;
    this->lookDistance = lookDistance;
    probability = 0.02f;
    setRequiredControlFlags(Control::LookControlFlag);

    lookTime = 0;
}

LookAtPlayerGoal::LookAtPlayerGoal(Mob* mob, const std::type_info& lookAtType,
                                   float lookDistance, float probability)
    : lookAtType(lookAtType) {
    this->mob = mob;
    this->lookDistance = lookDistance;
    this->probability = probability;
    setRequiredControlFlags(Control::LookControlFlag);

    lookTime = 0;
}

bool LookAtPlayerGoal::canUse() {
    if (mob->getRandom()->nextFloat() >= probability) return false;

    if (mob->getTarget() != nullptr) {
        lookAt = mob->getTarget();
    }
    if (lookAtType == typeid(Player)) {
        lookAt =
            mob->level->getNearestPlayer(mob->shared_from_this(), lookDistance);
    } else {
        AABB mob_bb = mob->bb.grow(lookDistance, 3, lookDistance);
        lookAt = std::weak_ptr<Entity>(mob->level->getClosestEntityOfClass(
            lookAtType, &mob_bb, mob->shared_from_this()));
    }
    return lookAt.lock() != nullptr;
}

bool LookAtPlayerGoal::canContinueToUse() {
    if (lookAt.lock() == nullptr || !lookAt.lock()->isAlive()) return false;
    if (mob->distanceToSqr(lookAt.lock()) > lookDistance * lookDistance)
        return false;
    return lookTime > 0;
}

void LookAtPlayerGoal::start() {
    lookTime = 40 + mob->getRandom()->nextInt(40);
}

void LookAtPlayerGoal::stop() { lookAt = std::weak_ptr<Entity>(); }

void LookAtPlayerGoal::tick() {
    mob->getLookControl()->setLookAt(
        lookAt.lock()->x, lookAt.lock()->y + lookAt.lock()->getHeadHeight(),
        lookAt.lock()->z, 10, mob->getMaxHeadXRot());
    --lookTime;
}
