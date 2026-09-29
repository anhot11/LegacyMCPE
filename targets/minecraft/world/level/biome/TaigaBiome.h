#pragma once
#include "Biome.h"
#include "minecraft/world/level/biome/Biome.h"

class TaigaBiome : public Biome {
public:
    TaigaBiome(int id);

    virtual Feature* getTreeFeature(Random* random);
};