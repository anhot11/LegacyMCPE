#include "SitGoal.h"

#include <memory>

#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/TamableAnimal.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/goal/FollowOwnerGoal.h"
#include "minecraft/world/entity/ai/navigation/PathNavigation.h"

SitGoal::SitGoal(TamableAnimal* mob) {
    _wantToSit = false;

    this->mob = mob;
    setRequiredControlFlags(Control::JumpControlFlag |
                            Control::MoveControlFlag);
}

bool SitGoal::canUse() {
    if (!mob->isTame()) return false;
    if (mob->isInWater()) return false;
    if (!mob->onGround) return false;

    std::shared_ptr<LivingEntity> owner =
        std::dynamic_pointer_cast<LivingEntity>(mob->getOwner());
    if (owner == nullptr) return true;  // owner not on level

    if (mob->distanceToSqr(owner) < FollowOwnerGoal::TeleportDistance *
                                        FollowOwnerGoal::TeleportDistance &&
        owner->getLastHurtByMob() != nullptr)
        return false;

    return _wantToSit;
}

void SitGoal::start() {
    mob->getNavigation()->stop();
    mob->setSitting(true);
}

void SitGoal::stop() { mob->setSitting(false); }

void SitGoal::wantToSit(bool _wantToSit) { this->_wantToSit = _wantToSit; }
