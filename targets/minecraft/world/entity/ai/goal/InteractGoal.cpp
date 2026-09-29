#include "InteractGoal.h"

#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/goal/LookAtPlayerGoal.h"

class Mob;

InteractGoal::InteractGoal(Mob* mob, const std::type_info& lookAtType,
                           float lookDistance)
    : LookAtPlayerGoal(mob, lookAtType, lookDistance) {
    setRequiredControlFlags(Control::LookControlFlag |
                            Control::MoveControlFlag);
}

InteractGoal::InteractGoal(Mob* mob, const std::type_info& lookAtType,
                           float lookDistance, float probability)
    : LookAtPlayerGoal(mob, lookAtType, lookDistance, probability) {
    setRequiredControlFlags(Control::LookControlFlag |
                            Control::MoveControlFlag);
}