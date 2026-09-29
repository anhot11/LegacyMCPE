#include "LeafTileItem.h"

#include <memory>

#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/TileItem.h"
#include "minecraft/world/level/FoliageColor.h"
#include "minecraft/world/level/tile/LeafTile.h"
#include "minecraft/world/level/tile/Tile.h"

LeafTileItem::LeafTileItem(int id) : TileItem(id) {
    setMaxDamage(0);
    setStackedByData(true);
}

int LeafTileItem::getLevelDataForAuxValue(int auxValue) {
    return auxValue | LeafTile::PERSISTENT_LEAF_BIT;
}

Icon* LeafTileItem::getIcon(int itemAuxValue) {
    return Tile::leaves->getTexture(0, itemAuxValue);
}

int LeafTileItem::getColor(std::shared_ptr<ItemInstance> item,
                           int spriteLayer) {
    int data = item->getAuxValue();
    if ((data & LeafTile::EVERGREEN_LEAF) == LeafTile::EVERGREEN_LEAF) {
        return FoliageColor::getEvergreenColor();
    }
    if ((data & LeafTile::BIRCH_LEAF) == LeafTile::BIRCH_LEAF) {
        return FoliageColor::getBirchColor();
    }
    return FoliageColor::getDefaultColor();
}

unsigned int LeafTileItem::getDescriptionId(
    std::shared_ptr<ItemInstance> instance) {
    int auxValue = instance->getAuxValue();
    if (auxValue < 0 || auxValue >= LeafTile::LEAF_NAMES_LENGTH) {
        auxValue = 0;
    }
    return LeafTile::LEAF_NAMES[auxValue];
}