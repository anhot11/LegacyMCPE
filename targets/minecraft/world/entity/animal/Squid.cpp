#include "Squid.h"

#include <math.h>

#include <memory>
#include <numbers>

#include "java/Random.h"
#include "minecraft/SharedConstants.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/entity/ai/attributes/AttributeInstance.h"
#include "minecraft/world/entity/animal/WaterAnimal.h"
#include "minecraft/world/entity/monster/SharedMonsterAttributes.h"
#include "minecraft/world/item/DyePowderItem.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/phys/AABB.h"

void Squid::_init() {
    xBodyRot = xBodyRotO = 0.0f;
    zBodyRot = zBodyRotO = 0.0f;

    tentacleMovement = oldTentacleMovement = 0.0f;
    tentacleAngle = oldTentacleAngle = 0.0f;

    speed = 0.0f;
    tentacleSpeed = 0.0f;
    rotateSpeed = 0.0f;

    tx = ty = tz = 0.0f;
}

Squid::Squid(Level* level) : WaterAnimal(level) {
    // 4J Stu - This function call had to be moved here from the Entity ctor to
    // ensure that the derived version of the function is called
    this->defineSynchedData();
    registerAttributes();
    setHealth(getMaxHealth());

    _init();
    this->setSize(0.95f, 0.95f);
    tentacleSpeed = 1 / (random->nextFloat() + 1) * 0.2f;
}

void Squid::registerAttributes() {
    WaterAnimal::registerAttributes();

    getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(10);
}

int Squid::getAmbientSound() { return -1; }

int Squid::getHurtSound() { return -1; }

int Squid::getDeathSound() { return -1; }

float Squid::getSoundVolume() { return 0.4f; }

int Squid::getDeathLoot() { return 0; }

bool Squid::makeStepSound() { return false; }

void Squid::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel) {
    int count = random->nextInt(3 + playerBonusLevel) + 1;
    for (int i = 0; i < count; i++) {
        spawnAtLocation(std::make_shared<ItemInstance>(Item::dye_powder, 1,
                                                       DyePowderItem::BLACK),
                        0.0f);
    }
}

bool Squid::isInWater() {
    AABB grown = bb.grow(0, -0.6, 0);
    return level->checkAndHandleWater(&grown, Material::water,
                                      shared_from_this());
}

void Squid::aiStep() {
    WaterAnimal::aiStep();

    xBodyRotO = xBodyRot;
    zBodyRotO = zBodyRot;

    oldTentacleMovement = tentacleMovement;
    oldTentacleAngle = tentacleAngle;

    tentacleMovement += tentacleSpeed;
    if (tentacleMovement > (float)std::numbers::pi * 2.0f) {
        tentacleMovement -= (float)std::numbers::pi * 2.0f;
        if (random->nextInt(10) == 0)
            tentacleSpeed = 1 / (random->nextFloat() + 1) * 0.2f;
    }

    if (isInWater()) {
        if (tentacleMovement < std::numbers::pi) {
            float tentacleScale = tentacleMovement / std::numbers::pi;
            tentacleAngle =
                sinf(tentacleScale * tentacleScale * std::numbers::pi) *
                std::numbers::pi * 0.25f;

            if (tentacleScale > .75) {
                speed = 1.0f;
                rotateSpeed = 1.0f;
            } else {
                rotateSpeed = rotateSpeed * 0.8f;
            }
        } else {
            tentacleAngle = 0.0f;
            speed = speed * 0.9f;
            rotateSpeed = rotateSpeed * 0.99f;
        }

        if (!level->isClientSide) {
            xd = tx * speed;
            yd = ty * speed;
            zd = tz * speed;
        }

        double horizontalMovement = sqrt(xd * xd + zd * zd);

        yBodyRot +=
            ((-(float)atan2(xd, zd) * 180 / std::numbers::pi) - yBodyRot) *
            0.1f;
        yRot = yBodyRot;
        zBodyRot = zBodyRot + (float)std::numbers::pi * rotateSpeed * 1.5f;
        xBodyRot +=
            ((-(float)atan2(horizontalMovement, yd) * 180 / std::numbers::pi) -
             xBodyRot) *
            0.1f;
    } else {
        tentacleAngle =
            Mth::abs(sinf(tentacleMovement)) * std::numbers::pi * 0.25f;

        if (!level->isClientSide) {
            // unable to move, apply gravity
            xd = 0.0f;
            yd -= 0.08;
            yd *= 0.98f;
            zd = 0.0f;
        }

        // fall over
        xBodyRot += (-90 - xBodyRot) * 0.02f;
    }
}

void Squid::travel(float xa, float ya) { move(xd, yd, zd); }

void Squid::serverAiStep() {
    noActionTime++;

    // ridiculous simple movement ai
    if (noActionTime > SharedConstants::TICKS_PER_SECOND * 5) {
        tx = ty = tz = 0;
    } else if (random->nextInt(50) == 0 || !wasInWater ||
               (tx == 0 && ty == 0 && tz == 0)) {
        float angle = random->nextFloat() * std::numbers::pi * 2.0f;
        tx = cosf(angle) * 0.2f;
        ty = -0.1f + random->nextFloat() * 0.2f;
        tz = sinf(angle) * 0.2f;
    }
    checkDespawn();  // 4J - 1.7.0 fix
}

bool Squid::canSpawn() {
    return y > 45 && y < level->seaLevel && WaterAnimal::canSpawn();
}
