#include "Feature.h"

#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/Tile.h"

class Random;

Feature::Feature() { this->doUpdate = false; }

Feature::Feature(bool doUpdate) { this->doUpdate = doUpdate; }

void Feature::applyFeature(Level* level, Random* random, int xChunk,
                           int zChunk) {}

void Feature::placeBlock(Level* level, int x, int y, int z, int tile) {
    placeBlock(level, x, y, z, tile, 0);
}

void Feature::placeBlock(Level* level, int x, int y, int z, int tile,
                         int data) {
    if (doUpdate) {
        level->setTileAndData(x, y, z, tile, data, Tile::UPDATE_ALL);
    } else {
        level->setTileAndData(x, y, z, tile, data, Tile::UPDATE_CLIENTS);
    }
}