#include "DirtTile.h"

#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"

DirtTile::DirtTile(int id) : Tile(id, Material::dirt) {}