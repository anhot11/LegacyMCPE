#include "MinecartFurnace.h"

#include <stdint.h>

#include <memory>
#include <string>

#include "java/Random.h"
#include "minecraft/SharedConstants.h"
#include "minecraft/core/particles/ParticleTypes.h"
#include "minecraft/network/packet/ContainerOpenPacket.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/damageSource/DamageSource.h"
#include "minecraft/world/entity/SyncedEntityData.h"
#include "minecraft/world/entity/item/Minecart.h"
#include "minecraft/world/entity/player/Abilities.h"
#include "minecraft/world/entity/player/Inventory.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/Tile.h"
#include "nbt/CompoundTag.h"

MinecartFurnace::MinecartFurnace(Level* level) : Minecart(level) {
    defineSynchedData();

    fuel = 0;
    xPush = zPush = 0.0f;
}

MinecartFurnace::MinecartFurnace(Level* level, double x, double y, double z)
    : Minecart(level, x, y, z) {
    defineSynchedData();

    fuel = 0;
    xPush = zPush = 0.0f;
}

// 4J Added
int MinecartFurnace::getContainerType() {
    return ContainerOpenPacket::MINECART_HOPPER;
}

int MinecartFurnace::getType() { return TYPE_FURNACE; }

void MinecartFurnace::defineSynchedData() {
    Minecart::defineSynchedData();
    entityData->define(DATA_ID_FUEL, (uint8_t)0);
}

void MinecartFurnace::tick() {
    Minecart::tick();

    if (fuel > 0) {
        fuel--;
    }
    if (fuel <= 0) {
        xPush = zPush = 0;
    }
    setHasFuel(fuel > 0);

    if (hasFuel() && random->nextInt(4) == 0) {
        level->addParticle(eParticleType_largesmoke, x, y + 0.8, z, 0, 0, 0);
    }
}

void MinecartFurnace::destroy(DamageSource* source) {
    Minecart::destroy(source);

    if (!source->isExplosion()) {
        spawnAtLocation(std::make_shared<ItemInstance>(Tile::furnace, 1), 0);
    }
}

void MinecartFurnace::moveAlongTrack(int xt, int yt, int zt, double maxSpeed,
                                     double slideSpeed, int tile, int data) {
    Minecart::moveAlongTrack(xt, yt, zt, maxSpeed, slideSpeed, tile, data);

    double sd = xPush * xPush + zPush * zPush;
    if (sd > 0.01 * 0.01 && xd * xd + zd * zd > 0.001) {
        sd = Mth::sqrt(sd);
        xPush /= sd;
        zPush /= sd;

        if (xPush * xd + zPush * zd < 0) {
            xPush = 0;
            zPush = 0;
        } else {
            xPush = xd;
            zPush = zd;
        }
    }
}

void MinecartFurnace::applyNaturalSlowdown() {
    double sd = xPush * xPush + zPush * zPush;

    if (sd > 0.01 * 0.01) {
        sd = Mth::sqrt(sd);
        xPush /= sd;
        zPush /= sd;
        double speed = 0.05;
        xd *= 0.8f;
        yd *= 0;
        zd *= 0.8f;
        xd += xPush * speed;
        zd += zPush * speed;
    } else {
        xd *= 0.98f;
        yd *= 0;
        zd *= 0.98f;
    }

    Minecart::applyNaturalSlowdown();
}

bool MinecartFurnace::interact(std::shared_ptr<Player> player) {
    std::shared_ptr<ItemInstance> selected = player->inventory->getSelected();
    if (selected != nullptr && selected->id == Item::coal_Id) {
        if (!player->abilities.instabuild && --selected->count == 0)
            player->inventory->setItem(player->inventory->selected, nullptr);
        fuel += SharedConstants::TICKS_PER_SECOND * 180;
    }
    xPush = x - player->x;
    zPush = z - player->z;

    return true;
}

void MinecartFurnace::addAdditonalSaveData(CompoundTag* base) {
    Minecart::addAdditonalSaveData(base);
    base->putDouble("PushX", xPush);
    base->putDouble("PushZ", zPush);
    base->putShort("Fuel", (short)fuel);
}

void MinecartFurnace::readAdditionalSaveData(CompoundTag* base) {
    Minecart::readAdditionalSaveData(base);
    xPush = base->getDouble("PushX");
    zPush = base->getDouble("PushZ");
    fuel = base->getShort("Fuel");
}

bool MinecartFurnace::hasFuel() {
    return (entityData->getByte(DATA_ID_FUEL) & 1) != 0;
}

void MinecartFurnace::setHasFuel(bool fuel) {
    if (fuel) {
        entityData->set(DATA_ID_FUEL,
                        (uint8_t)(entityData->getByte(DATA_ID_FUEL) | 1));
    } else {
        entityData->set(DATA_ID_FUEL,
                        (uint8_t)(entityData->getByte(DATA_ID_FUEL) & ~1));
    }
}

Tile* MinecartFurnace::getDefaultDisplayTile() { return Tile::furnace_lit; }

int MinecartFurnace::getDefaultDisplayData() { return 2; }