#pragma once

#include <memory>
#include <typeinfo>

#include "TargetGoal.h"
#include "minecraft/world/entity/EntitySelector.h"

class NearestAttackableTargetGoal;
class Entity;
class LivingEntity;
class PathfinderMob;

// Anonymous class from NearestAttackableTargetGoal
class SubselectEntitySelector : public EntitySelector {
private:
    EntitySelector* m_subselector;
    NearestAttackableTargetGoal* m_parent;

public:
    SubselectEntitySelector(NearestAttackableTargetGoal* parent,
                            EntitySelector* subselector);
    ~SubselectEntitySelector();
    bool matches(std::shared_ptr<Entity> entity) const;
};

class NearestAttackableTargetGoal : public TargetGoal {
    friend class SubselectEntitySelector;

public:
    class DistComp {
    private:
        Entity* source;

    public:
        DistComp(Entity* source);

        bool operator()(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2);
    };

private:
    const std::type_info& targetType;
    int randomInterval;
    DistComp* distComp;
    EntitySelector* selector;
    std::weak_ptr<LivingEntity> target;

public:
    NearestAttackableTargetGoal(PathfinderMob* mob,
                                const std::type_info& targetType,
                                int randomInterval, bool mustSee,
                                bool mustReach = false,
                                EntitySelector* entitySelector = nullptr);

    virtual ~NearestAttackableTargetGoal();

    virtual bool canUse();
    void start();
};