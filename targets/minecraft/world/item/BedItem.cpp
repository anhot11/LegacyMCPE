#include "BedItem.h"

#include <memory>

#include "minecraft/Direction.h"
#include "minecraft/Facing.h"
#include "minecraft/stats/GenericStats.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/BedTile.h"
#include "minecraft/world/level/tile/Tile.h"

BedItem::BedItem(int id) : Item(id) {}

bool BedItem::useOn(std::shared_ptr<ItemInstance> itemInstance,
                    std::shared_ptr<Player> player, Level* level, int x, int y,
                    int z, int face, float clickX, float clickY, float clickZ,
                    bool bTestUseOnOnly) {
    if (level->isClientSide) return true;

    if (face != Facing::UP) {
        return false;
    }

    // place on top of tile
    y = y + 1;

    BedTile* tile = (BedTile*)Tile::bed;

    int dir = (Mth::floor(player->yRot * 4 / (360) + 0.5f)) & 3;
    int xra = 0;
    int zra = 0;

    if (dir == Direction::SOUTH) zra = 1;
    if (dir == Direction::WEST) xra = -1;
    if (dir == Direction::NORTH) zra = -1;
    if (dir == Direction::EAST) xra = 1;

    if (!player->mayUseItemAt(x, y, z, face, itemInstance) ||
        !player->mayUseItemAt(x + xra, y, z + zra, face, itemInstance))
        return false;

    if (level->isEmptyTile(x, y, z) &&
        level->isEmptyTile(x + xra, y, z + zra) &&
        level->isTopSolidBlocking(x, y - 1, z) &&
        level->isTopSolidBlocking(x + xra, y - 1, z + zra)) {
        // 4J-PB - Adding a test only version to allow tooltips to be displayed
        if (!bTestUseOnOnly) {
            level->setTileAndData(x, y, z, tile->id, dir, Tile::UPDATE_ALL);
            // double-check that the bed was successfully placed
            if (level->getTile(x, y, z) == tile->id) {
                // 4J-JEV: Hook for durango 'BlockPlaced' event.
                player->awardStat(
                    GenericStats::blocksPlaced(tile->id),
                    GenericStats::param_blocksPlaced(
                        tile->id, itemInstance->getAuxValue(), 1));

                level->setTileAndData(x + xra, y, z + zra, tile->id,
                                      dir + BedTile::HEAD_PIECE_DATA,
                                      Tile::UPDATE_ALL);
            }

            itemInstance->count--;
        }
        return true;
    }

    return false;
}
