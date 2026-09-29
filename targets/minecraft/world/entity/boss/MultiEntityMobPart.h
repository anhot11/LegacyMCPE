#pragma once
#include <memory>
#include <string>

#include "java/Class.h"
#include "minecraft/world/entity/Entity.h"

class Level;
class MultiEntityMob;

class MultiEntityMobPart : public Entity {
public:
    eINSTANCEOF GetType() { return eTYPE_MULTIENTITY_MOB_PART; };

public:
    std::weak_ptr<MultiEntityMob> parentMob;
    const std::string id;

    MultiEntityMobPart(std::shared_ptr<MultiEntityMob> parentMob,
                       const std::string& id, float w, float h);

protected:
    virtual void defineSynchedData();
    virtual void readAdditionalSaveData(CompoundTag* tag);
    virtual void addAdditonalSaveData(CompoundTag* tag);

public:
    virtual bool isPickable();
    virtual bool hurt(DamageSource* source, float damage);
    virtual bool is(std::shared_ptr<Entity> other);
};