#include "SandStoneTile.h"

#include "Facing.h"
#include "minecraft/world/IconRegister.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"
#include "strings.h"

class Icon;

const std::string SandStoneTile::TEXTURE_TOP = "sandstone_top";
const std::string SandStoneTile::TEXTURE_BOTTOM = "sandstone_bottom";
const std::string SandStoneTile::TEXTURE_NAMES[] = {
    "sandstone_side", "sandstone_carved", "sandstone_smooth"};

int SandStoneTile::SANDSTONE_NAMES[SANDSTONE_BLOCK_NAMES] = {
    IDS_TILE_SANDSTONE, IDS_TILE_SANDSTONE_CHISELED, IDS_TILE_SANDSTONE_SMOOTH};

SandStoneTile::SandStoneTile(int id) : Tile(id, Material::stone) {
    icons = nullptr;
    iconTop = nullptr;
    iconBottom = nullptr;
}

Icon* SandStoneTile::getTexture(int face, int data) {
    if (face == Facing::UP ||
        (face == Facing::DOWN &&
         (data == TYPE_HEIROGLYPHS || data == TYPE_SMOOTHSIDE))) {
        return iconTop;
    }
    if (face == Facing::DOWN) {
        return iconBottom;
    }
    if (data < 0 || data >= SANDSTONE_TILE_TEXTURE_COUNT) data = 0;
    return icons[data];
}

int SandStoneTile::getSpawnResourcesAuxValue(int data) { return data; }

void SandStoneTile::registerIcons(IconRegister* iconRegister) {
    icons = new Icon*[SANDSTONE_TILE_TEXTURE_COUNT];

    for (int i = 0; i < SANDSTONE_TILE_TEXTURE_COUNT; i++) {
        icons[i] = iconRegister->registerIcon(TEXTURE_NAMES[i]);
    }

    iconTop = iconRegister->registerIcon(TEXTURE_TOP);
    iconBottom = iconRegister->registerIcon(TEXTURE_BOTTOM);
}