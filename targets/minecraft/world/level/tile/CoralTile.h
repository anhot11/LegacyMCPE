#pragma once
#include "HalfTransparentTile.h"
#include "minecraft/world/level/material/Material.h"

class CoralTile : public HalfTransparentTile {
public:
    CoralTile(int id);
    virtual int getColor(LevelSource* level, int x, int y, int z);
    virtual int getColor(LevelSource* level, int x, int y, int z,
                         int data);  // 4J added
};