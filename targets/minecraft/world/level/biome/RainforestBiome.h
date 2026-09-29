#pragma once
#include "Biome.h"
#include "minecraft/world/level/biome/Biome.h"

class RainforestBiome : public Biome {
public:
    RainforestBiome(int id);
    virtual Feature* getTreeFeature(Random* random);
};