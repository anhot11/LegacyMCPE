#include "HayBlockTile.h"

#include <string>

#include "minecraft/world/IconRegister.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/RotatedPillarTile.h"

HayBlockTile::HayBlockTile(int id) : RotatedPillarTile(id, Material::grass) {}

int HayBlockTile::getRenderShape() { return SHAPE_TREE; }

Icon* HayBlockTile::getTypeTexture(int type) { return icon; }

void HayBlockTile::registerIcons(IconRegister* iconRegister) {
    iconTop = iconRegister->registerIcon(getIconName() + "_top");
    icon = iconRegister->registerIcon(getIconName() + "_side");
}