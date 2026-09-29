#pragma once

#include "Biome.h"
#include "minecraft/world/level/biome/Biome.h"

class PlainsBiome : public Biome {
    friend class Biome;

protected:
    PlainsBiome(int id);
};