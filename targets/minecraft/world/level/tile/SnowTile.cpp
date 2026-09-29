#include "SnowTile.h"

#include "minecraft/world/item/Item.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/LightLayer.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"

SnowTile::SnowTile(int id) : Tile(id, Material::snow) { setTicking(true); }

int SnowTile::getResource(int data, Random* random, int playerBonusLevel) {
    return Item::snowBall->id;
}

int SnowTile::getResourceCount(Random* random) { return 4; }

void SnowTile::tick(Level* level, int x, int y, int z, Random* random) {
    if (level->getBrightness(LightLayer::Block, x, y, z) > 11) {
        this->spawnResources(level, x, y, z, level->getData(x, y, z), 0);
        level->removeTile(x, y, z);
    }
}

bool SnowTile::shouldTileTick(Level* level, int x, int y, int z) {
    return level->getBrightness(LightLayer::Block, x, y, z) > 11;
}