#include "LeashFenceKnotEntity.h"

#include <vector>

#include "minecraft/world/entity/HangingEntity.h"
#include "minecraft/world/entity/Mob.h"
#include "minecraft/world/entity/player/Abilities.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/phys/AABB.h"

class CompoundTag;
class Entity;

void LeashFenceKnotEntity::_init() { defineSynchedData(); }

LeashFenceKnotEntity::LeashFenceKnotEntity(Level* level)
    : HangingEntity(level) {
    _init();
}

LeashFenceKnotEntity::LeashFenceKnotEntity(Level* level, int xTile, int yTile,
                                           int zTile)
    : HangingEntity(level, xTile, yTile, zTile, 0) {
    _init();
    setPos(xTile + .5, yTile + .5, zTile + .5);
}

void LeashFenceKnotEntity::defineSynchedData() {
    HangingEntity::defineSynchedData();
}

void LeashFenceKnotEntity::setDir(int dir) {
    // override to do nothing, knots don't have directions
}

int LeashFenceKnotEntity::getWidth() { return 9; }

int LeashFenceKnotEntity::getHeight() { return 9; }

bool LeashFenceKnotEntity::shouldRenderAtSqrDistance(double distance) {
    return distance < 32 * 32;
}

void LeashFenceKnotEntity::dropItem(std::shared_ptr<Entity> causedBy) {}

bool LeashFenceKnotEntity::save(CompoundTag* entityTag) {
    // knots are not saved, they are recreated by the entities that are tied
    return false;
}

void LeashFenceKnotEntity::addAdditonalSaveData(CompoundTag* tag) {}

void LeashFenceKnotEntity::readAdditionalSaveData(CompoundTag* tag) {}

bool LeashFenceKnotEntity::interact(std::shared_ptr<Player> player) {
    std::shared_ptr<ItemInstance> item = player->getCarriedItem();

    bool attachedMob = false;
    if (item != nullptr && item->id == Item::lead_Id) {
        if (!level->isClientSide) {
            // look for entities that can be attached to the fence
            double range = 7;
            AABB mob_aabb{x - range, y - range, z - range,
                          x + range, y + range, z + range};
            std::vector<std::shared_ptr<Entity> >* mobs =
                level->getEntitiesOfClass(typeid(Mob), &mob_aabb);
            if (mobs != nullptr) {
                for (auto it = mobs->begin(); it != mobs->end(); ++it) {
                    std::shared_ptr<Mob> mob =
                        std::dynamic_pointer_cast<Mob>(*it);
                    if (mob->isLeashed() && mob->getLeashHolder() == player) {
                        mob->setLeashedTo(shared_from_this(), true);
                        attachedMob = true;
                    }
                }
                delete mobs;
            }
        }
    }
    if (!level->isClientSide && !attachedMob) {
        remove();

        if (player->abilities.instabuild) {
            // if the player is in creative mode, attempt to remove all leashed
            // mobs without dropping additional items
            double range = 7;
            AABB mob_aabb{x - range, y - range, z - range,
                          x + range, y + range, z + range};
            std::vector<std::shared_ptr<Entity> >* mobs =
                level->getEntitiesOfClass(typeid(Mob), &mob_aabb);
            if (mobs != nullptr) {
                for (auto it = mobs->begin(); it != mobs->end(); ++it) {
                    std::shared_ptr<Mob> mob =
                        std::dynamic_pointer_cast<Mob>(*it);
                    if (mob->isLeashed() &&
                        mob->getLeashHolder() == shared_from_this()) {
                        mob->dropLeash(true, false);
                    }
                }
                delete mobs;
            }
        }
    }
    return true;
}

bool LeashFenceKnotEntity::survives() {
    // knots are placed on top of fence tiles
    int tile = level->getTile(xTile, yTile, zTile);
    if (Tile::tiles[tile] != nullptr &&
        Tile::tiles[tile]->getRenderShape() == Tile::SHAPE_FENCE) {
        return true;
    }
    return false;
}

std::shared_ptr<LeashFenceKnotEntity> LeashFenceKnotEntity::createAndAddKnot(
    Level* level, int x, int y, int z) {
    std::shared_ptr<LeashFenceKnotEntity> knot =
        std::shared_ptr<LeashFenceKnotEntity>(
            new LeashFenceKnotEntity(level, x, y, z));
    knot->forcedLoading = true;
    level->addEntity(knot);
    return knot;
}

std::shared_ptr<LeashFenceKnotEntity> LeashFenceKnotEntity::findKnotAt(
    Level* level, int x, int y, int z) {
    AABB leash_fence_knot_entity_aabb{x - 1.0, y - 1.0, z - 1.0,
                                      x + 1.0, y + 1.0, z + 1.0};
    std::vector<std::shared_ptr<Entity> >* knots = level->getEntitiesOfClass(
        typeid(LeashFenceKnotEntity), &leash_fence_knot_entity_aabb);
    if (knots != nullptr) {
        for (auto it = knots->begin(); it != knots->end(); ++it) {
            std::shared_ptr<LeashFenceKnotEntity> knot =
                std::dynamic_pointer_cast<LeashFenceKnotEntity>(*it);
            if (knot->xTile == x && knot->yTile == y && knot->zTile == z) {
                delete knots;
                return knot;
            }
        }
        delete knots;
    }
    return nullptr;
}
