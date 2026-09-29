#include "BeachBiome.h"

#include <stdint.h>

#include <vector>

#include "BiomeDecorator.h"
#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/tile/Tile.h"

BeachBiome::BeachBiome(int id) : Biome(id) {
    // remove default mob spawn settings
    friendlies.clear();
    friendlies_chicken.clear();  // 4J added
    topMaterial = (uint8_t)Tile::sand_Id;
    material = (uint8_t)Tile::sand_Id;

    decorator->treeCount = -999;
    decorator->deadBushCount = 0;
    decorator->reedsCount = 0;
    decorator->cactusCount = 0;
}