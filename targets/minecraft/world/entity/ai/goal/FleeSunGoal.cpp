#include "FleeSunGoal.h"

#include <optional>

#include "java/Random.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/entity/PathfinderMob.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/navigation/PathNavigation.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/phys/AABB.h"
#include "minecraft/world/phys/Vec3.h"

FleeSunGoal::FleeSunGoal(PathfinderMob* mob, double speedModifier) {
    this->mob = mob;
    this->speedModifier = speedModifier;
    this->level = mob->level;
    setRequiredControlFlags(Control::MoveControlFlag);
}

bool FleeSunGoal::canUse() {
    if (!level->isDay()) return false;
    if (!mob->isOnFire()) return false;
    if (!level->canSeeSky(Mth::floor(mob->x), (int)mob->bb.y0,
                          Mth::floor(mob->z)))
        return false;

    auto pos = getHidePos();
    if (!pos.has_value()) return false;
    wantedX = pos->x;
    wantedY = pos->y;
    wantedZ = pos->z;
    return true;
}

bool FleeSunGoal::canContinueToUse() { return !mob->getNavigation()->isDone(); }

void FleeSunGoal::start() {
    mob->getNavigation()->moveTo(wantedX, wantedY, wantedZ, speedModifier);
}

std::optional<Vec3> FleeSunGoal::getHidePos() {
    Random* random = mob->getRandom();
    for (int i = 0; i < 10; i++) {
        int xt = Mth::floor(mob->x + random->nextInt(20) - 10);
        int yt = Mth::floor(mob->bb.y0 + random->nextInt(6) - 3);
        int zt = Mth::floor(mob->z + random->nextInt(20) - 10);
        if (!level->canSeeSky(xt, yt, zt) &&
            mob->getWalkTargetValue(xt, yt, zt) < 0)
            return Vec3(xt, yt, zt);
    }
    return std::nullopt;
}
