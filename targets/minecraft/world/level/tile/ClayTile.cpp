#include "ClayTile.h"

#include "minecraft/world/item/Item.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"

ClayTile::ClayTile(int id) : Tile(id, Material::clay) {}

int ClayTile::getResource(int data, Random* random, int playerBonusLevel) {
    return Item::clay->id;
}

int ClayTile::getResourceCount(Random* random) { return 4; }
