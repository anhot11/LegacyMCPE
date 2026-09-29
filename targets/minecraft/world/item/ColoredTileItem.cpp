#include "ColoredTileItem.h"

#include <memory>

#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/TileItem.h"
#include "minecraft/world/level/tile/Tile.h"

ColoredTileItem::ColoredTileItem(int id, bool stackedByData) : TileItem(id) {
    this->colorTile = Tile::tiles[getTileId()];

    if (stackedByData) {
        setMaxDamage(0);
        setStackedByData(true);
    }
}

ColoredTileItem::~ColoredTileItem() {}

int ColoredTileItem::getColor(std::shared_ptr<ItemInstance> item,
                              int spriteLayer) {
    return colorTile->getColor(item->getAuxValue());
}

Icon* ColoredTileItem::getIcon(int auxValue) {
    return colorTile->getTexture(0, auxValue);
}

int ColoredTileItem::getLevelDataForAuxValue(int auxValue) { return auxValue; }

ColoredTileItem* ColoredTileItem::setDescriptionPostfixes(
    std::vector<int>& descriptionPostfixes) {
    this->descriptionPostfixes.clear();
    this->descriptionPostfixes = std::vector<int>(descriptionPostfixes.size());
    for (unsigned int i = 0; i < descriptionPostfixes.size(); ++i) {
        this->descriptionPostfixes[i] = descriptionPostfixes[i];
    }

    return this;
}

unsigned int ColoredTileItem::getDescriptionId(
    std::shared_ptr<ItemInstance> instance) {
    if (descriptionPostfixes.empty()) {
        return TileItem::getDescriptionId(instance);
    }
    int id = instance->getAuxValue();
    if (id >= 0 && id < descriptionPostfixes.size()) {
        return descriptionPostfixes
            [id];  // TileItem::getDescriptionId(instance)
                   // + "." + descriptionPostfixes[id];
    }
    return TileItem::getDescriptionId(instance);
}