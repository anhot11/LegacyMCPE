#include "HalfTransparentTile.h"

#include "minecraft/world/IconRegister.h"
#include "minecraft/world/level/LevelSource.h"
#include "minecraft/world/level/tile/Tile.h"

class Material;

HalfTransparentTile::HalfTransparentTile(int id, const std::string& tex,
                                         Material* material, bool allowSame)
    : Tile(id, material, false) {
    this->allowSame = allowSame;
    this->texture = tex;
}

bool HalfTransparentTile::isSolidRender(bool isServerLevel) { return false; }

bool HalfTransparentTile::shouldRenderFace(LevelSource* level, int x, int y,
                                           int z, int face) {
    int id = level->getTile(x, y, z);
    if (!allowSame && id == this->id) return false;
    return Tile::shouldRenderFace(level, x, y, z, face);
}

bool HalfTransparentTile::blocksLight() { return false; }

void HalfTransparentTile::registerIcons(IconRegister* iconRegister) {
    icon = iconRegister->registerIcon(texture);
}