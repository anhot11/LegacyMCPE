#pragma once
#include "Feature.h"
#include "minecraft/world/level/material/Material.h"

class Level;

class CaveFeature : public Feature {
    virtual bool place(Level* level, Random* random, int x, int y, int z);
};