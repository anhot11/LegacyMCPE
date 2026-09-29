#pragma once

#include <memory>
#include <typeinfo>

#include "Goal.h"

class Mob;
class Level;
class Entity;

class LookAtPlayerGoal : public Goal {
private:
    Mob* mob;  // Owner of this goal

protected:
    std::weak_ptr<Entity> lookAt;

private:
    float lookDistance;
    int lookTime;
    float probability;
    const std::type_info& lookAtType;

public:
    LookAtPlayerGoal(Mob* mob, const std::type_info& lookAtType,
                     float lookDistance);
    LookAtPlayerGoal(Mob* mob, const std::type_info& lookAtType,
                     float lookDistance, float probability);
    virtual ~LookAtPlayerGoal() {}

    virtual bool canUse();
    virtual bool canContinueToUse();
    virtual void start();
    virtual void stop();
    virtual void tick();
};