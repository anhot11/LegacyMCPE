#include "SnowItem.h"

#include <memory>
#include <optional>

#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/AuxDataTileItem.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/level/tile/TopSnowTile.h"

SnowItem::SnowItem(int id, Tile* parentTile)
    : AuxDataTileItem(id, parentTile) {}

bool SnowItem::useOn(std::shared_ptr<ItemInstance> instance,
                     std::shared_ptr<Player> player, Level* level, int x, int y,
                     int z, int face, float clickX, float clickY, float clickZ,
                     bool bTestUseOnOnly) {
    if (instance->count == 0) return false;
    if (!player->mayUseItemAt(x, y, z, face, instance)) return false;

    int currentTile = level->getTile(x, y, z);

    // Are we adding extra snow to an existing tile?
    if (currentTile == Tile::topSnow_Id) {
        Tile* snowTile = Tile::tiles[getTileId()];
        int currentData = level->getData(x, y, z);
        int currentHeight = currentData & TopSnowTile::HEIGHT_MASK;

        auto snow_bb = snowTile->getAABB(level, x, y, z);
        if (currentHeight <= TopSnowTile::MAX_HEIGHT &&
            level->isUnobstructed(snow_bb.has_value() ? &*snow_bb : nullptr)) {
            if (!bTestUseOnOnly) {
                // Increase snow tile height
                if (level->setData(
                        x, y, z,
                        (currentHeight + 1) |
                            (currentData & ~TopSnowTile::HEIGHT_MASK),
                        Tile::UPDATE_CLIENTS)) {
                    level->playSound(x + 0.5f, y + 0.5f, z + 0.5f,
                                     snowTile->soundType->getPlaceSound(),
                                     (snowTile->soundType->getVolume() + 1) / 2,
                                     snowTile->soundType->getPitch() * 0.8f);
                    instance->count--;
                    return true;
                }
            } else {
                return true;
            }
        }
    }

    return AuxDataTileItem::useOn(instance, player, level, x, y, z, face,
                                  clickX, clickY, clickZ, bTestUseOnOnly);
}
