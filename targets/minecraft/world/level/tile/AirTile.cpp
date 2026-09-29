#include "AirTile.h"

#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"

AirTile::AirTile(int id) : Tile(id, Material::air) {}
