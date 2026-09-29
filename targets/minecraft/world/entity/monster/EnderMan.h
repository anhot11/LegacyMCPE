#pragma once

#include <memory>

#include "Monster.h"
#include "java/Class.h"

class AttributeModifier;
class Entity;
class Level;
class Player;

class EnderMan : public Monster {
public:
    eINSTANCEOF GetType() { return eTYPE_ENDERMAN; }
    static Entity* create(Level* level) { return new EnderMan(level); }

public:
    static void staticCtor();

private:
    static AttributeModifier* SPEED_MODIFIER_ATTACKING;

    static bool MAY_TAKE[256];

    static const int DATA_CARRY_ITEM_ID = 16;
    static const int DATA_CARRY_ITEM_DATA = 17;
    static const int DATA_CREEPY = 18;

private:
    int teleportTime;
    int aggroTime;
    std::shared_ptr<Entity> lastAttackTarget;
    bool aggroedByPlayer;

public:
    EnderMan(Level* level);

protected:
    virtual void registerAttributes();
    virtual void defineSynchedData();

public:
    virtual void addAdditonalSaveData(CompoundTag* tag);
    virtual void readAdditionalSaveData(CompoundTag* tag);

protected:
    virtual std::shared_ptr<Entity> findAttackTarget();

private:
    bool isLookingAtMe(std::shared_ptr<Player> player);

public:
    virtual void aiStep();

protected:
    bool teleport();
    bool teleportTowards(std::shared_ptr<Entity> e);
    bool teleport(double xx, double yy, double zz);

    virtual int getAmbientSound();
    virtual int getHurtSound();
    virtual int getDeathSound();
    virtual int getDeathLoot();
    virtual void dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel);

public:
    void setCarryingTile(int carryingTile);
    int getCarryingTile();
    void setCarryingData(int carryingData);
    int getCarryingData();
    virtual bool hurt(DamageSource* source, float damage);
    bool isCreepy();
    void setCreepy(bool creepy);
};