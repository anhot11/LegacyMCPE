#pragma once

#include <stdint.h>

#include <memory>
#include <string>

#include "OwnableEntity.h"
#include "minecraft/world/entity/animal/Animal.h"

class SitGoal;
class Level;
class LivingEntity;

class TamableAnimal : public Animal, public OwnableEntity {
protected:
    static const int DATA_FLAGS_ID = 16;
    static const int DATA_OWNERUUID_ID = 17;

    SitGoal* sitGoal;

public:
    TamableAnimal(Level* level);
    virtual ~TamableAnimal();

protected:
    virtual void defineSynchedData();

public:
    virtual void addAdditonalSaveData(CompoundTag* tag);
    virtual void readAdditionalSaveData(CompoundTag* tag);

protected:
    virtual void spawnTamingParticles(bool success);

public:
    virtual void handleEntityEvent(uint8_t id);
    virtual bool isTame();
    virtual void setTame(bool value);
    virtual bool isSitting();
    virtual void setSitting(bool value);
    virtual std::string getOwnerUUID();
    virtual void setOwnerUUID(const std::string& name);
    virtual std::shared_ptr<Entity> getOwner();
    virtual SitGoal* getSitGoal();
    bool wantsToAttack(std::shared_ptr<LivingEntity> target,
                       std::shared_ptr<LivingEntity> owner);
    Team* getTeam();
    bool isAlliedTo(std::shared_ptr<LivingEntity> other);
};