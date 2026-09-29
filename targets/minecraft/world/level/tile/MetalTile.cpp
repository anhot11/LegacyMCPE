#include "MetalTile.h"

#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"

MetalTile::MetalTile(int id) : Tile(id, Material::metal) {}