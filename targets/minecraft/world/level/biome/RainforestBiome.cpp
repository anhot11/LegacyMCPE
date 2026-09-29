#include "RainforestBiome.h"

#include "java/Random.h"
#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/levelgen/feature/BasicTreeFeature.h"
#include "minecraft/world/level/levelgen/feature/TreeFeature.h"

RainforestBiome::RainforestBiome(int id) : Biome(id) {}

Feature* RainforestBiome::getTreeFeature(Random* random) {
    if (random->nextInt(3) == 0) {
        return new BasicTree(false);
    }
    return new TreeFeature(false);
}