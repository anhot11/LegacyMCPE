#include "SwellGoal.h"

#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/navigation/PathNavigation.h"
#include "minecraft/world/entity/ai/sensing/Sensing.h"
#include "minecraft/world/entity/monster/Creeper.h"

SwellGoal::SwellGoal(Creeper* creeper) {
    target = std::weak_ptr<LivingEntity>();

    this->creeper = creeper;
    setRequiredControlFlags(Control::MoveControlFlag);
}

bool SwellGoal::canUse() {
    std::shared_ptr<LivingEntity> target = creeper->getTarget();
    return creeper->getSwellDir() > 0 ||
           (target != nullptr && (creeper->distanceToSqr(target) < 3 * 3));
}

void SwellGoal::start() {
    creeper->getNavigation()->stop();
    target = std::weak_ptr<LivingEntity>(creeper->getTarget());
}

void SwellGoal::stop() { target = std::weak_ptr<LivingEntity>(); }

void SwellGoal::tick() {
    if (target.lock() == nullptr) {
        creeper->setSwellDir(-1);
        return;
    }

    if (creeper->distanceToSqr(target.lock()) > 7 * 7) {
        creeper->setSwellDir(-1);
        return;
    }

    if (!creeper->getSensing()->canSee(target.lock())) {
        creeper->setSwellDir(-1);
        return;
    }

    creeper->setSwellDir(1);
}
