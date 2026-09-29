#pragma once
#include <memory>

#include "java/Class.h"
#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/entity/LivingEntity.h"

class Level;

class PrimedTnt : public Entity {
public:
    eINSTANCEOF GetType() { return eTYPE_PRIMEDTNT; };
    static Entity* create(Level* level) { return new PrimedTnt(level); }

public:
    static const int serialVersionUID = 0;

    int life;
    std::weak_ptr<LivingEntity> owner;

    void _init();

    PrimedTnt(Level* level);
    PrimedTnt(Level* level, double x, double y, double z,
              std::shared_ptr<LivingEntity> owner);

protected:
    virtual void defineSynchedData();
    virtual bool makeStepSound();

public:
    virtual bool isPickable();
    virtual void tick();

private:
    void explode();

protected:
    virtual void addAdditonalSaveData(CompoundTag* entityTag);
    virtual void readAdditionalSaveData(CompoundTag* tag);

public:
    virtual float getShadowHeightOffs();
    virtual std::shared_ptr<LivingEntity> getOwner();
};
