#include "RestrictOpenDoorGoal.h"

#include "minecraft/util/Mth.h"
#include "minecraft/world/entity/PathfinderMob.h"
#include "minecraft/world/entity/ai/navigation/PathNavigation.h"
#include "minecraft/world/entity/ai/village/DoorInfo.h"
#include "minecraft/world/entity/ai/village/Village.h"
#include "minecraft/world/entity/ai/village/Villages.h"
#include "minecraft/world/level/Level.h"

RestrictOpenDoorGoal::RestrictOpenDoorGoal(PathfinderMob* mob) {
    this->mob = mob;
}

bool RestrictOpenDoorGoal::canUse() {
    if (mob->level->isDay()) return false;
    std::shared_ptr<Village> village = mob->level->villages->getClosestVillage(
        Mth::floor(mob->x), Mth::floor(mob->y), Mth::floor(mob->z), 16);
    if (village == nullptr) return false;
    std::shared_ptr<DoorInfo> _doorInfo = village->getClosestDoorInfo(
        Mth::floor(mob->x), Mth::floor(mob->y), Mth::floor(mob->z));
    if (_doorInfo == nullptr) return false;
    doorInfo = _doorInfo;
    return _doorInfo->distanceToInsideSqr(Mth::floor(mob->x),
                                          Mth::floor(mob->y),
                                          Mth::floor(mob->z)) < 1.5 * 1.5;
}

bool RestrictOpenDoorGoal::canContinueToUse() {
    if (mob->level->isDay()) return false;
    std::shared_ptr<DoorInfo> _doorInfo = doorInfo.lock();
    if (_doorInfo == nullptr) return false;
    return !_doorInfo->removed &&
           _doorInfo->isInsideSide(Mth::floor(mob->x), Mth::floor(mob->z));
}

void RestrictOpenDoorGoal::start() {
    mob->getNavigation()->setCanOpenDoors(false);
    mob->getNavigation()->setCanPassDoors(false);
}

void RestrictOpenDoorGoal::stop() {
    mob->getNavigation()->setCanOpenDoors(true);
    mob->getNavigation()->setCanPassDoors(true);
    doorInfo = std::weak_ptr<DoorInfo>();
}

void RestrictOpenDoorGoal::tick() {
    std::shared_ptr<DoorInfo> _doorInfo = doorInfo.lock();
    if (_doorInfo) _doorInfo->incBookingCount();
}