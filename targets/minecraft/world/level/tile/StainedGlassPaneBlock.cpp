#include "StainedGlassPaneBlock.h"

#include <string>

#include "minecraft/world/IconRegister.h"
#include "minecraft/world/item/DyePowderItem.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/ThinFenceTile.h"

class Icon;

Icon* StainedGlassPaneBlock::ICONS[StainedGlassPaneBlock::ICONS_COUNT];
Icon* StainedGlassPaneBlock::EDGE_ICONS[StainedGlassPaneBlock::ICONS_COUNT];

StainedGlassPaneBlock::StainedGlassPaneBlock(int id)
    : ThinFenceTile(id, "glass", "glass_pane_top", Material::glass, false) {}

Icon* StainedGlassPaneBlock::getIconTexture(int face, int data) {
    return ICONS[data % ICONS_COUNT];
}

Icon* StainedGlassPaneBlock::getEdgeTexture(int data) {
    return EDGE_ICONS[~data & 0xF];
}

Icon* StainedGlassPaneBlock::getTexture(int face, int data) {
    return getIconTexture(face, ~data & 0xf);
}

int StainedGlassPaneBlock::getSpawnResourcesAuxValue(int data) { return data; }

int StainedGlassPaneBlock::getItemAuxValueForBlockData(int data) {
    return (data & 0xf);
}

int StainedGlassPaneBlock::getRenderLayer() { return 1; }

void StainedGlassPaneBlock::registerIcons(IconRegister* iconRegister) {
    ThinFenceTile::registerIcons(iconRegister);
    for (int i = 0; i < ICONS_COUNT; i++) {
        ICONS[i] = iconRegister->registerIcon(
            getIconName() + "_" +
            DyePowderItem::COLOR_TEXTURES[getItemAuxValueForBlockData(i)]);
        EDGE_ICONS[i] = iconRegister->registerIcon(
            getIconName() + "_pane_top_" +
            DyePowderItem::COLOR_TEXTURES[getItemAuxValueForBlockData(i)]);
    }
}