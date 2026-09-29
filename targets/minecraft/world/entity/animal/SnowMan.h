#pragma once

#include "Golem.h"
#include "java/Class.h"
#include "minecraft/world/entity/monster/RangedAttackMob.h"

class Entity;
class Level;

class SnowMan : public Golem, public RangedAttackMob {
public:
    eINSTANCEOF GetType() { return eTYPE_SNOWMAN; }
    static Entity* create(Level* level) { return new SnowMan(level); }

public:
    SnowMan(Level* level);
    virtual bool useNewAi();

protected:
    virtual void registerAttributes();

public:
    virtual void aiStep();

protected:
    virtual int getDeathLoot();
    virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);

public:
    virtual void performRangedAttack(std::shared_ptr<LivingEntity> target,
                                     float power);
};