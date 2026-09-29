#include "PrimedTnt.h"

#include <stdint.h>

#include <cmath>
#include <numbers>
#include <string>

#include "java/JavaMath.h"
#include "minecraft/core/particles/ParticleTypes.h"
#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/level/Level.h"
#include "nbt/CompoundTag.h"

class LivingEntity;

void PrimedTnt::_init() {
    life = 0;

    // Original Java Ctor
    blocksBuilding = true;
    setSize(0.98f, 0.98f);
    heightOffset = bbHeight / 2.0f;

    owner = std::weak_ptr<LivingEntity>();
}

PrimedTnt::PrimedTnt(Level* level) : Entity(level) {
    // 4J Stu - This function call had to be moved here from the Entity ctor to
    // ensure that the derived version of the function is called
    this->defineSynchedData();

    _init();
}

PrimedTnt::PrimedTnt(Level* level, double x, double y, double z,
                     std::shared_ptr<LivingEntity> owner)
    : Entity(level) {
    _init();

    setPos(x, y, z);

    float rot = (float)(Math::random() * std::numbers::pi * 2);
    xd = -sin(rot) * 0.02f;
    yd = +0.2f;
    zd = -cos(rot) * 0.02f;

    life = 80;

    xo = x;
    yo = y;
    zo = z;

    this->owner = std::weak_ptr<LivingEntity>(owner);
}

void PrimedTnt::defineSynchedData() {}

bool PrimedTnt::makeStepSound() { return false; }

bool PrimedTnt::isPickable() { return !removed; }

void PrimedTnt::tick() {
    xo = x;
    yo = y;
    zo = z;

    yd -= 0.04f;
    move(xd, yd, zd);
    xd *= 0.98f;
    yd *= 0.98f;
    zd *= 0.98f;

    if (onGround) {
        xd *= 0.7f;
        zd *= 0.7f;
        yd *= -0.5f;
    }

    if (life-- <= 0) {
        remove();
        if (!level->isClientSide) {
            explode();
        }
    } else {
        level->addParticle(eParticleType_smoke, x, y + 0.5f, z, 0, 0, 0);
    }
}

void PrimedTnt::explode() {
    float r = 4.0f;
    level->explode(shared_from_this(), x, y, z, r, true);
}

void PrimedTnt::addAdditonalSaveData(CompoundTag* entityTag) {
    entityTag->putByte("Fuse", (uint8_t)life);
}

void PrimedTnt::readAdditionalSaveData(CompoundTag* tag) {
    life = tag->getByte("Fuse");
}

float PrimedTnt::getShadowHeightOffs() { return 0; }

std::shared_ptr<LivingEntity> PrimedTnt::getOwner() { return owner.lock(); }