#include "MoveIndoorsGoal.h"

#include <optional>

#include "java/Random.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/entity/PathfinderMob.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/navigation/PathNavigation.h"
#include "minecraft/world/entity/ai/util/RandomPos.h"
#include "minecraft/world/entity/ai/village/DoorInfo.h"
#include "minecraft/world/entity/ai/village/Village.h"
#include "minecraft/world/entity/ai/village/Villages.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/dimension/Dimension.h"
#include "minecraft/world/phys/Vec3.h"

MoveIndoorsGoal::MoveIndoorsGoal(PathfinderMob* mob) {
    insideX = insideZ = -1;

    this->mob = mob;
    setRequiredControlFlags(Control::MoveControlFlag);
}

bool MoveIndoorsGoal::canUse() {
    if ((mob->level->isDay() && !mob->level->isRaining()) ||
        mob->level->dimension->hasCeiling)
        return false;
    if (mob->getRandom()->nextInt(50) != 0) return false;
    if (insideX != -1 && mob->distanceToSqr(insideX, mob->y, insideZ) < 2 * 2)
        return false;
    std::shared_ptr<Village> village = mob->level->villages->getClosestVillage(
        Mth::floor(mob->x), Mth::floor(mob->y), Mth::floor(mob->z), 14);
    if (village == nullptr) return false;
    std::shared_ptr<DoorInfo> _doorInfo = village->getBestDoorInfo(
        Mth::floor(mob->x), Mth::floor(mob->y), Mth::floor(mob->z));
    doorInfo = _doorInfo;
    return _doorInfo != nullptr;
}

bool MoveIndoorsGoal::canContinueToUse() {
    return !mob->getNavigation()->isDone();
}

void MoveIndoorsGoal::start() {
    insideX = -1;
    std::shared_ptr<DoorInfo> _doorInfo = doorInfo.lock();
    if (_doorInfo == nullptr) {
        doorInfo = std::weak_ptr<DoorInfo>();
        return;
    }
    if (mob->distanceToSqr(_doorInfo->getIndoorX(), _doorInfo->y,
                           _doorInfo->getIndoorZ()) > 16 * 16) {
        Vec3 towards(_doorInfo->getIndoorX() + 0.5, _doorInfo->getIndoorY(),
                     _doorInfo->getIndoorZ() + 0.5);
        auto pos = RandomPos::getPosTowards(
            std::dynamic_pointer_cast<PathfinderMob>(mob->shared_from_this()),
            14, 3, &towards);
        if (pos.has_value())
            mob->getNavigation()->moveTo(pos->x, pos->y, pos->z, 1.0f);
    } else
        mob->getNavigation()->moveTo(_doorInfo->getIndoorX() + 0.5,
                                     _doorInfo->getIndoorY(),
                                     _doorInfo->getIndoorZ() + 0.5, 1.0f);
}

void MoveIndoorsGoal::stop() {
    std::shared_ptr<DoorInfo> _doorInfo = doorInfo.lock();
    if (_doorInfo == nullptr) {
        doorInfo = std::weak_ptr<DoorInfo>();
        return;
    }

    insideX = _doorInfo->getIndoorX();
    insideZ = _doorInfo->getIndoorZ();
    doorInfo = std::weak_ptr<DoorInfo>();
}
