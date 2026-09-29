#pragma once

#include "Enemy.h"
#include "java/Class.h"
#include "minecraft/world/entity/PathfinderMob.h"

class Level;
class CompoundTag;
class DamageSource;
class Entity;

class Monster : public PathfinderMob, public Enemy {
public:
    eINSTANCEOF GetType() { return eTYPE_MONSTER; }
    static Entity* create(Level* level) { return nullptr; }

public:
    Monster(Level* level);

    virtual void aiStep();
    virtual void tick();

protected:
    virtual std::shared_ptr<Entity> findAttackTarget();

public:
    virtual bool hurt(DamageSource* source, float dmg);
    virtual bool doHurtTarget(std::shared_ptr<Entity> target);

protected:
    virtual void checkHurtTarget(std::shared_ptr<Entity> target,
                                 float distance);

public:
    virtual float getWalkTargetValue(int x, int y, int z);

protected:
    virtual bool isDarkEnoughToSpawn();

public:
    virtual bool canSpawn();

protected:
    void registerAttributes();
};