#include "StainedGlassBlock.h"

#include <string>

#include "minecraft/world/IconRegister.h"
#include "minecraft/world/item/DyePowderItem.h"
#include "minecraft/world/level/tile/HalfTransparentTile.h"

class Icon;
class Material;

Icon* StainedGlassBlock::ICONS[StainedGlassBlock::ICONS_LENGTH];

StainedGlassBlock::StainedGlassBlock(int id, Material* material)
    : HalfTransparentTile(id, "glass", material, false) {}

Icon* StainedGlassBlock::getTexture(int face, int data) {
    return ICONS[data % ICONS_LENGTH];
}

int StainedGlassBlock::getSpawnResourcesAuxValue(int data) { return data; }

int StainedGlassBlock::getItemAuxValueForBlockData(int data) {
    return (~data & 0xf);
}

int StainedGlassBlock::getRenderLayer() { return 1; }

void StainedGlassBlock::registerIcons(IconRegister* iconRegister) {
    for (int i = 0; i < ICONS_LENGTH; i++) {
        ICONS[i] = iconRegister->registerIcon(
            getIconName() + "_" +
            DyePowderItem::COLOR_TEXTURES[getItemAuxValueForBlockData(i)]);
    }
}

int StainedGlassBlock::getResourceCount(Random* random) { return 0; }

bool StainedGlassBlock::isSilkTouchable() { return true; }

bool StainedGlassBlock::isCubeShaped() { return false; }
