#pragma once

#include <memory>
#include <typeinfo>

#include "Goal.h"
#include "minecraft/world/entity/EntitySelector.h"

class PathNavigation;
class PathfinderMob;
class Path;
class AvoidPlayerGoal;
class Entity;

class AvoidPlayerGoalEntitySelector : public EntitySelector {
private:
    AvoidPlayerGoal* m_parent;

public:
    AvoidPlayerGoalEntitySelector(AvoidPlayerGoal* parent);
    bool matches(std::shared_ptr<Entity> entity) const;
};

class AvoidPlayerGoal : public Goal {
    friend class AvoidPlayerGoalEntitySelector;

private:
    PathfinderMob* mob;  // Owner of this goal
    double walkSpeedModifier, sprintSpeedModifier;
    std::weak_ptr<Entity> toAvoid;
    float maxDist;
    Path* path;
    PathNavigation* pathNav;
    const std::type_info& avoidType;
    EntitySelector* entitySelector;

public:
    AvoidPlayerGoal(PathfinderMob* mob, const std::type_info& avoidType,
                    float maxDist, double walkSpeedModifier,
                    double sprintSpeedModifier);
    ~AvoidPlayerGoal();

    virtual bool canUse();
    virtual bool canContinueToUse();
    virtual void start();
    virtual void stop();
    virtual void tick();
};