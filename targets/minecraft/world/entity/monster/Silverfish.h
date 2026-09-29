#pragma once

#include "Monster.h"
#include "java/Class.h"
#include "minecraft/world/entity/MobType.h"

class Entity;
class Level;

class Silverfish : public Monster {
public:
    eINSTANCEOF GetType() { return eTYPE_SILVERFISH; }
    static Entity* create(Level* level) { return new Silverfish(level); }

private:
    int lookForFriends;

public:
    Silverfish(Level* level);

protected:
    virtual void registerAttributes();
    virtual bool makeStepSound();
    virtual std::shared_ptr<Entity> findAttackTarget();

    virtual int getAmbientSound();
    virtual int getHurtSound();
    virtual int getDeathSound();

public:
    virtual bool hurt(DamageSource* source, float dmg);

protected:
    virtual void checkHurtTarget(std::shared_ptr<Entity> target, float d);
    virtual void playStepSound(int xt, int yt, int zt, int t);
    virtual int getDeathLoot();

public:
    virtual void tick();

protected:
    virtual void serverAiStep();

public:
    virtual float getWalkTargetValue(int x, int y, int z);

protected:
    virtual bool isDarkEnoughToSpawn();

public:
    virtual bool canSpawn();
    virtual MobType getMobType();
};