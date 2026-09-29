#include "ColoredTile.h"

#include <string>

#include "minecraft/world/IconRegister.h"
#include "minecraft/world/item/DyePowderItem.h"
#include "minecraft/world/level/tile/Tile.h"

class Material;

ColoredTile::ColoredTile(int id, Material* material) : Tile(id, material) {}

Icon* ColoredTile::getTexture(int face, int data) {
    return icons[data % ICON_COUNT];
}

int ColoredTile::getSpawnResourcesAuxValue(int data) { return data; }

int ColoredTile::getTileDataForItemAuxValue(int auxValue) {
    return (~auxValue & 0xf);
}

int ColoredTile::getItemAuxValueForTileData(int data) { return (~data & 0xf); }

void ColoredTile::registerIcons(IconRegister* iconRegister) {
    for (int i = 0; i < ICON_COUNT; i++) {
        icons[i] = iconRegister->registerIcon(
            getIconName() + "_" +
            DyePowderItem::COLOR_TEXTURES[getItemAuxValueForTileData(i)]);
    }
}