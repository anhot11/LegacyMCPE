#pragma once

#include "Biome.h"
#include "minecraft/world/level/biome/Biome.h"

class Feature;

class ExtremeHillsBiome : public Biome {
    friend class Biome;

private:
    static const bool GENERATE_EMERALD_ORE = true;
    Feature* silverfishFeature;

protected:
    ExtremeHillsBiome(int id);
    ~ExtremeHillsBiome();

public:
    void decorate(Level* level, Random* random, int xo, int zo);
};