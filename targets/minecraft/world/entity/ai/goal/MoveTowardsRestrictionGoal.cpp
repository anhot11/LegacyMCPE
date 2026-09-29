#include "MoveTowardsRestrictionGoal.h"

#include <memory>
#include <optional>

#include "minecraft/Pos.h"
#include "minecraft/world/entity/PathfinderMob.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/navigation/PathNavigation.h"
#include "minecraft/world/entity/ai/util/RandomPos.h"
#include "minecraft/world/phys/Vec3.h"

MoveTowardsRestrictionGoal::MoveTowardsRestrictionGoal(PathfinderMob* mob,
                                                       double speedModifier) {
    wantedX = wantedY = wantedZ = 0.0;

    this->mob = mob;
    this->speedModifier = speedModifier;
    setRequiredControlFlags(Control::MoveControlFlag);
}

bool MoveTowardsRestrictionGoal::canUse() {
    if (mob->isWithinRestriction()) return false;
    Pos* toward = mob->getRestrictCenter();
    Vec3 towards(toward->x, toward->y, toward->z);
    auto pos = RandomPos::getPosTowards(
        std::dynamic_pointer_cast<PathfinderMob>(mob->shared_from_this()), 16,
        7, &towards);
    if (!pos.has_value()) return false;
    wantedX = pos->x;
    wantedY = pos->y;
    wantedZ = pos->z;
    return true;
}

bool MoveTowardsRestrictionGoal::canContinueToUse() {
    return !mob->getNavigation()->isDone();
}

void MoveTowardsRestrictionGoal::start() {
    mob->getNavigation()->moveTo(wantedX, wantedY, wantedZ, speedModifier);
}
