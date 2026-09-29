#include "GlassTile.h"

#include <string>

#include "minecraft/world/level/tile/HalfTransparentTile.h"

class Material;

GlassTile::GlassTile(int id, Material* material, bool allowSame)
    : HalfTransparentTile(id, "glass", material, allowSame) {}

int GlassTile::getResourceCount(Random* random) { return 0; }

int GlassTile::getRenderLayer() { return 0; }

bool GlassTile::isSolidRender() { return false; }

bool GlassTile::isCubeShaped() { return false; }

bool GlassTile::isSilkTouchable() { return true; }