#pragma once

#include <memory>

#include "Entity.h"
#include "java/Class.h"
#include "minecraft/world/entity/Entity.h"

class DamageSource;
class Level;

class DelayedRelease : public Entity {
public:
    virtual eINSTANCEOF GetType() { return eTYPE_DELAYEDRELEASE; }

private:
    std::shared_ptr<Entity> toRelease;
    int delay;

public:
    DelayedRelease(Level* level, std::shared_ptr<Entity> toRelease, int delay);

protected:
    virtual bool makeStepSound();

public:
    virtual void tick();
    virtual bool hurt(DamageSource* source, float damage);

protected:
    virtual void defineSynchedData();
    virtual void readAdditionalSaveData(CompoundTag* tag);
    virtual void addAdditonalSaveData(CompoundTag* tag);
};