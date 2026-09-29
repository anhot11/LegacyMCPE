#include "StoneTile.h"

#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"

StoneTile::StoneTile(int id) : Tile(id, Material::stone) {}

int StoneTile::getResource(int data, Random* random, int playerBonusLevel) {
    return Tile::cobblestone_Id;
}