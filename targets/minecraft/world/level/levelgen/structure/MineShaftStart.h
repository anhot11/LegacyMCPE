#pragma once

#include "StructureStart.h"
#include "minecraft/world/level/levelgen/structure/StructureFeatureIO.h"

class Level;
class Random;

class MineShaftStart : public StructureStart {
public:
    static StructureStart* Create() { return new MineShaftStart(); }
    virtual EStructureStart GetType() { return eStructureStart_MineShaftStart; }

public:
    MineShaftStart();
    MineShaftStart(Level* level, Random* random, int chunkX, int chunkZ);
};