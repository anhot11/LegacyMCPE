
#include "minecraft/world/level/biome/PlainsBiome.h"

#include <vector>

#include "java/Class.h"
#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/biome/BiomeDecorator.h"

PlainsBiome::PlainsBiome(int id) : Biome(id) {
    friendlies.push_back(new MobSpawnerData(eTYPE_HORSE, 5, 2, 6));

    decorator->treeCount = -999;
    decorator->flowerCount = 4;
    decorator->grassCount = 10;
}