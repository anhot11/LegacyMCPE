#include "FallingTile.h"

#include <stdint.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "java/Random.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/damageSource/DamageSource.h"
#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/HeavyTile.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/level/tile/entity/TileEntity.h"
#include "nbt/CompoundTag.h"
#include "nbt/Tag.h"

// 4J - added for common ctor code
void FallingTile::_init() {
    // 4J Stu - This function call had to be moved here from the Entity ctor to
    // ensure that the derived version of the function is called
    this->defineSynchedData();

    tile = 0;
    data = 0;
    time = 0;
    dropItem = true;

    cancelDrop = false;
    hurtEntities = false;
    fallDamageMax = 40;
    fallDamageAmount = 2;
    tileData = nullptr;

    // 4J Added so that client-side falling tiles can fall through blocks
    // This fixes a bug on the host where the tile update from the server comes
    // in before the client-side falling tile has reached that level, causing it
    // to stop at one block higher.
    m_ignoreVerticalCollisions = level->isClientSide;
}

FallingTile::FallingTile(Level* level) : Entity(level) { _init(); }

FallingTile::FallingTile(Level* level, double x, double y, double z, int tile,
                         int data)
    : Entity(level) {
    _init();

    this->tile = tile;
    this->data = data;
    blocksBuilding = true;
    setSize(0.98f, 0.98f);
    heightOffset = bbHeight / 2.0f;
    setPos(x, y, z);

    xd = 0;
    yd = 0;
    zd = 0;

    xo = x;
    yo = y;
    zo = z;

    // 4J added - without this newly created falling tiles weren't interpolating
    // their render positions correctly
    xOld = x;
    yOld = y;
    zOld = z;
}

FallingTile::~FallingTile() { delete tileData; }

bool FallingTile::makeStepSound() { return false; }

void FallingTile::defineSynchedData() {}

bool FallingTile::isPickable() { return !removed; }

void FallingTile::tick() {
    if (tile == 0) {
        remove();
        return;
    }

    xo = x;
    yo = y;
    zo = z;
    time++;

    yd -= 0.04f;
    move(xd, yd, zd);
    xd *= 0.98f;
    yd *= 0.98f;
    zd *= 0.98f;

    if (!level->isClientSide) {
        int xt = Mth::floor(x);
        int yt = Mth::floor(y);
        int zt = Mth::floor(z);
        if (time == 1) {
            if (level->getTile(xt, yt, zt) == tile) {
                level->removeTile(xt, yt, zt);
            } else {
                remove();
                return;
            }
        }

        if (onGround) {
            xd *= 0.7f;
            zd *= 0.7f;
            yd *= -0.5f;

            if (level->getTile(xt, yt, zt) != Tile::pistonMovingPiece_Id) {
                remove();
                if (!cancelDrop &&
                    level->mayPlace(tile, xt, yt, zt, true, 1, nullptr,
                                    nullptr) &&
                    !HeavyTile::isFree(level, xt, yt - 1, zt) &&
                    level->setTileAndData(xt, yt, zt, tile, data,
                                          Tile::UPDATE_ALL)) {
                    HeavyTile* hv = dynamic_cast<HeavyTile*>(Tile::tiles[tile]);
                    if (hv) {
                        hv->onLand(level, xt, yt, zt, data);
                    }
                    if (tileData != nullptr &&
                        Tile::tiles[tile]->isEntityTile()) {
                        std::shared_ptr<TileEntity> tileEntity =
                            level->getTileEntity(xt, yt, zt);

                        if (tileEntity != nullptr) {
                            CompoundTag* swap = new CompoundTag();
                            tileEntity->save(swap);
                            std::vector<Tag*> allTags = tileData->getAllTags();
                            for (auto it = allTags.begin(); it != allTags.end();
                                 ++it) {
                                Tag* tag = *it;
                                if (tag->getName().compare("x") == 0 ||
                                    tag->getName().compare("y") == 0 ||
                                    tag->getName().compare("z") == 0)
                                    continue;
                                swap->put(tag->getName(), tag->copy());
                            }
                            tileEntity->load(swap);
                            tileEntity->setChanged();
                        }
                    }
                } else {
                    if (dropItem && !cancelDrop)
                        spawnAtLocation(
                            std::make_shared<ItemInstance>(
                                tile, 1,
                                Tile::tiles[tile]->getSpawnResourcesAuxValue(
                                    data)),
                            0);
                }
            }
        } else if ((time > 20 * 5 && !level->isClientSide &&
                    (yt < 1 || yt > Level::maxBuildHeight)) ||
                   (time > 20 * 30)) {
            if (dropItem)
                spawnAtLocation(
                    std::make_shared<ItemInstance>(
                        tile, 1,
                        Tile::tiles[tile]->getSpawnResourcesAuxValue(data)),
                    0);
            remove();
        }
    }
}

void FallingTile::causeFallDamage(float distance) {
    if (hurtEntities) {
        int dmg = Mth::ceil(distance - 1);
        if (dmg > 0) {
            // 4J: Copy vector since it might be modified when we hurt the
            // entities (invalidating our iterator)
            std::vector<std::shared_ptr<Entity> >* entities =
                new std::vector<std::shared_ptr<Entity> >(
                    *level->getEntities(shared_from_this(), &bb));
            DamageSource* source = tile == Tile::anvil_Id
                                       ? DamageSource::anvil
                                       : DamageSource::fallingBlock;
            // for (Entity entity : entities)
            for (auto it = entities->begin(); it != entities->end(); ++it) {
                (*it)->hurt(source, std::min(Mth::floor(dmg * fallDamageAmount),
                                             fallDamageMax));
            }
            delete entities;

            if (tile == Tile::anvil_Id &&
                random->nextFloat() < 0.05f + (dmg * 0.05)) {
                int damage = data >> 2;
                int dir = data & 3;

                if (++damage > 2) {
                    cancelDrop = true;
                } else {
                    data = dir | (damage << 2);
                }
            }
        }
    }
}

void FallingTile::addAdditonalSaveData(CompoundTag* tag) {
    tag->putByte("Tile", (uint8_t)tile);
    tag->putInt("TileID", tile);
    tag->putByte("Data", (uint8_t)data);
    tag->putByte("Time", (uint8_t)time);
    tag->putBoolean("DropItem", dropItem);
    tag->putBoolean("HurtEntities", hurtEntities);
    tag->putFloat("FallHurtAmount", fallDamageAmount);
    tag->putInt("FallHurtMax", fallDamageMax);
    if (tileData != nullptr) tag->putCompound("TileEntityData", tileData);
}

void FallingTile::readAdditionalSaveData(CompoundTag* tag) {
    if (tag->contains("TileID")) {
        tile = tag->getInt("TileID");
    } else {
        tile = tag->getByte("Tile") & 0xff;
    }
    data = tag->getByte("Data") & 0xff;
    time = tag->getByte("Time") & 0xff;

    if (tag->contains("HurtEntities")) {
        hurtEntities = tag->getBoolean("HurtEntities");
        fallDamageAmount = tag->getFloat("FallHurtAmount");
        fallDamageMax = tag->getInt("FallHurtMax");
    } else if (tile == Tile::anvil_Id) {
        hurtEntities = true;
    }

    if (tag->contains("DropItem")) {
        dropItem = tag->getBoolean("DropItem");
    }

    if (tag->contains("TileEntityData")) {
        tileData = tag->getCompound("TileEntityData");
    }

    if (tile == 0) {
        tile = Tile::sand_Id;
    }
}

float FallingTile::getShadowHeightOffs() { return 0; }

Level* FallingTile::getLevel() { return level; }

void FallingTile::setHurtsEntities(bool value) { this->hurtEntities = value; }

bool FallingTile::displayFireAnimation() { return false; }
