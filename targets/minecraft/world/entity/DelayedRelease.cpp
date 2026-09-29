#include "DelayedRelease.h"

#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/level/Level.h"

DelayedRelease::DelayedRelease(Level* level, std::shared_ptr<Entity> toRelease,
                               int delay)
    : Entity(level) {
    moveTo(toRelease->x, toRelease->y, toRelease->z, 0, 0);
    this->toRelease = toRelease;
    this->delay = delay;
}

bool DelayedRelease::makeStepSound() { return false; }

void DelayedRelease::tick() {
    if (delay-- <= 0) {
        level->addEntity(toRelease);
        remove();
    }
}

bool DelayedRelease::hurt(DamageSource* source, float damage) { return false; }

void DelayedRelease::defineSynchedData() {}

void DelayedRelease::readAdditionalSaveData(CompoundTag* tag) {}

void DelayedRelease::addAdditonalSaveData(CompoundTag* tag) {}