#pragma once

#include <typeinfo>

#include "LookAtPlayerGoal.h"

class Mob;

class InteractGoal : public LookAtPlayerGoal {
public:
    InteractGoal(Mob* mob, const std::type_info& lookAtType,
                 float lookDistance);
    InteractGoal(Mob* mob, const std::type_info& lookAtType, float lookDistance,
                 float probability);
};