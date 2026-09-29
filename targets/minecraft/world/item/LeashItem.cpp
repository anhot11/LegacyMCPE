#include "LeashItem.h"

#include <format>
#include <vector>

#include "minecraft/world/entity/LeashFenceKnotEntity.h"
#include "minecraft/world/entity/Mob.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/phys/AABB.h"

class Entity;

LeashItem::LeashItem(int id) : Item(id) {}

bool LeashItem::useOn(std::shared_ptr<ItemInstance> itemInstance,
                      std::shared_ptr<Player> player, Level* level, int x,
                      int y, int z, int face, float clickX, float clickY,
                      float clickZ, bool bTestUseOnOnly) {
    int tile = level->getTile(x, y, z);
    if (Tile::tiles[tile] != nullptr &&
        Tile::tiles[tile]->getRenderShape() == Tile::SHAPE_FENCE) {
        if (bTestUseOnOnly) return bindPlayerMobsTest(player, level, x, y, z);

        if (level->isClientSide) {
            return true;
        }

        bindPlayerMobs(player, level, x, y, z);
        return true;
    }
    return false;
}

bool LeashItem::bindPlayerMobs(std::shared_ptr<Player> player, Level* level,
                               int x, int y, int z) {
    // check if there is a knot at the given coordinate
    std::shared_ptr<LeashFenceKnotEntity> activeKnot =
        LeashFenceKnotEntity::findKnotAt(level, x, y, z);

    // look for entities that can be attached to the fence
    bool foundMobs = false;
    double range = 7;
    AABB mob_bb = AABB(x, y, z, x, y, z).grow(range, range, range);
    std::vector<std::shared_ptr<Entity> >* mobs =
        level->getEntitiesOfClass(typeid(Mob), &mob_bb);
    if (mobs != nullptr) {
        for (auto it = mobs->begin(); it != mobs->end(); ++it) {
            std::shared_ptr<Mob> mob = std::dynamic_pointer_cast<Mob>(*it);
            if (mob->isLeashed() && mob->getLeashHolder() == player) {
                if (activeKnot == nullptr) {
                    activeKnot =
                        LeashFenceKnotEntity::createAndAddKnot(level, x, y, z);
                }
                mob->setLeashedTo(activeKnot, true);
                foundMobs = true;
            }
        }
    }
    return foundMobs;
}

// 4J-JEV: Similar to bindPlayerMobs, but doesn't actually bind mobs,
bool LeashItem::bindPlayerMobsTest(std::shared_ptr<Player> player, Level* level,
                                   int x, int y, int z) {
    // look for entities that can be attached to the fence
    double range = 7;
    AABB mob_bb = AABB(x, y, z, x, y, z).grow(range, range, range);
    std::vector<std::shared_ptr<Entity> >* mobs =
        level->getEntitiesOfClass(typeid(Mob), &mob_bb);

    if (mobs != nullptr) {
        for (auto it = mobs->begin(); it != mobs->end(); ++it) {
            std::shared_ptr<Mob> mob = std::dynamic_pointer_cast<Mob>(*it);
            if (mob->isLeashed() && mob->getLeashHolder() == player)
                return true;
        }
    }
    return false;
}
