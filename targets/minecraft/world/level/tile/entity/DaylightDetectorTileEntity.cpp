#include "DaylightDetectorTileEntity.h"

#include <memory>

#include "minecraft/SharedConstants.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/DaylightDetectorTile.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/level/tile/entity/TileEntity.h"

DaylightDetectorTileEntity::DaylightDetectorTileEntity() {}

void DaylightDetectorTileEntity::tick() {
    if (level != nullptr && !level->isClientSide &&
        (level->getGameTime() % SharedConstants::TICKS_PER_SECOND) == 0) {
        tile = getTile();
        if (tile != nullptr &&
            dynamic_cast<DaylightDetectorTile*>(tile) != nullptr) {
            ((DaylightDetectorTile*)tile)->updateSignalStrength(level, x, y, z);
        }
    }
}

// 4J Added
std::shared_ptr<TileEntity> DaylightDetectorTileEntity::clone() {
    std::shared_ptr<DaylightDetectorTileEntity> result =
        std::shared_ptr<DaylightDetectorTileEntity>(
            new DaylightDetectorTileEntity());
    TileEntity::clone(result);

    return result;
}