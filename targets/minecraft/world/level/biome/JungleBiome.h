#pragma once

#include "Biome.h"
#include "minecraft/world/level/biome/Biome.h"

class JungleBiome : public Biome {
public:
    JungleBiome(int id);

    Feature* getTreeFeature(Random* random);
    Feature* getGrassFeature(Random* random);
    void decorate(Level* level, Random* random, int xo, int zo);
};