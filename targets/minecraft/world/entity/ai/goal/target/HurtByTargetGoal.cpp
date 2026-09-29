#include "HurtByTargetGoal.h"

#include <memory>
#include <vector>

#include "minecraft/world/entity/PathfinderMob.h"
#include "minecraft/world/entity/ai/goal/target/TargetGoal.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/phys/AABB.h"

class Entity;

HurtByTargetGoal::HurtByTargetGoal(PathfinderMob* mob, bool alertSameType)
    : TargetGoal(mob, false) {
    this->alertSameType = alertSameType;
    setRequiredControlFlags(TargetGoal::TargetFlag);
    timestamp = 0;
}

bool HurtByTargetGoal::canUse() {
    int ts = mob->getLastHurtByMobTimestamp();
    return ts != timestamp && canAttack(mob->getLastHurtByMob(), false);
}

void HurtByTargetGoal::start() {
    mob->setTarget(mob->getLastHurtByMob());
    timestamp = mob->getLastHurtByMobTimestamp();

    if (alertSameType) {
        double within = getFollowDistance();
        AABB mob_bb =
            AABB(mob->x, mob->y, mob->z, mob->x + 1, mob->y + 1, mob->z + 1)
                .grow(within, 4, within);
        std::vector<std::shared_ptr<Entity> >* nearby =
            mob->level->getEntitiesOfClass(typeid(*mob), &mob_bb);
        for (auto it = nearby->begin(); it != nearby->end(); ++it) {
            std::shared_ptr<PathfinderMob> other =
                std::dynamic_pointer_cast<PathfinderMob>(*it);
            if (this->mob->shared_from_this() == other) continue;
            if (other->getTarget() != nullptr) continue;
            if (other->isAlliedTo(mob->getLastHurtByMob()))
                continue;  // don't target allies
            other->setTarget(mob->getLastHurtByMob());
        }
        delete nearby;
    }

    TargetGoal::start();
}
