#include "AuxDataTileItem.h"

#include "minecraft/world/item/TileItem.h"
#include "minecraft/world/level/tile/Tile.h"

AuxDataTileItem::AuxDataTileItem(int id, Tile* parentTile) : TileItem(id) {
    this->parentTile = parentTile;

    setMaxDamage(0);
    setStackedByData(true);
}

Icon* AuxDataTileItem::getIcon(int itemAuxValue) {
    return parentTile->getTexture(2, itemAuxValue);
}

int AuxDataTileItem::getLevelDataForAuxValue(int auxValue) { return auxValue; }