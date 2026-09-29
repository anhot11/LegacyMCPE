#pragma once
#include "Biome.h"
#include "minecraft/world/level/biome/Biome.h"

class ForestBiome : public Biome {
public:
    ForestBiome(int id);

    virtual Feature* getTreeFeature(Random* random);
};
