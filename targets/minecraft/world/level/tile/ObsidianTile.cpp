#include "ObsidianTile.h"

#include "minecraft/world/level/tile/StoneTile.h"
#include "minecraft/world/level/tile/Tile.h"

ObsidianTile::ObsidianTile(int id) : StoneTile(id) {}

int ObsidianTile::getResourceCount(Random* random) { return 1; }

int ObsidianTile::getResource(int data, Random* random, int playerBonusLevel) {
    return Tile::obsidian_Id;
}