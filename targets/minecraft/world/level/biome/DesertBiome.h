#pragma once

#include "Biome.h"
#include "minecraft/world/level/biome/Biome.h"

class DesertBiome : public Biome {
public:
    DesertBiome(int id);
    virtual void decorate(Level* level, Random* random, int xo, int zo);
};