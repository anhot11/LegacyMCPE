#include "SpongeTile.h"

#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"

const int RANGE = 2;

Sponge::Sponge(int id) : Tile(id, Material::sponge) {}