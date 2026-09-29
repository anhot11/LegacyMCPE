#include "PanicGoal.h"

#include <memory>
#include <optional>

#include "java/Random.h"
#include "minecraft/world/entity/PathfinderMob.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/navigation/PathNavigation.h"
#include "minecraft/world/entity/ai/util/RandomPos.h"
#include "minecraft/world/phys/Vec3.h"

PanicGoal::PanicGoal(PathfinderMob* mob, double speedModifier) {
    this->mob = mob;
    this->speedModifier = speedModifier;
    setRequiredControlFlags(Control::MoveControlFlag);
}

bool PanicGoal::canUse() {
    if (mob->getLastHurtByMob() == nullptr && !mob->isOnFire()) return false;

    // 4jcraft: stop entities from being eternally scared (referenced from
    // smartcmd/MinecraftConsoles #519)
    const int hurtTimeout = mob->getLastHurtByMobTimestamp();
    static thread_local Random random;
    const int panicDuration = random.nextInt(41) + 60;
    if (mob->tickCount - hurtTimeout > panicDuration) return false;

    auto pos = RandomPos::getPos(
        std::dynamic_pointer_cast<PathfinderMob>(mob->shared_from_this()), 5,
        4);
    if (!pos.has_value()) return false;
    posX = pos->x;
    posY = pos->y;
    posZ = pos->z;
    return true;
}

void PanicGoal::start() {
    mob->getNavigation()->moveTo(posX, posY, posZ, speedModifier);
}

bool PanicGoal::canContinueToUse() { return !mob->getNavigation()->isDone(); }
