#include "EmptyLevelChunk.h"

#include "java/Arrays.h"
#include "java/Random.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/LightLayer.h"
#include "minecraft/world/level/chunk/LevelChunk.h"
#include "minecraft/world/phys/AABB.h"

class Entity;
class EntitySelector;

EmptyLevelChunk::EmptyLevelChunk(Level* level, int x, int z)
    : LevelChunk(level, x, z) {
    dontSave = true;
    // Set this as fully post-processed, so we don't try and run post-processing
    // on any edge chunks that will overlap into real chunks
    terrainPopulated = LevelChunk::sTerrainPopulatedAllNeighbours |
                       LevelChunk::sTerrainPostPostProcessed;
}

EmptyLevelChunk::EmptyLevelChunk(Level* level, std::vector<uint8_t>& blocks,
                                 int x, int z)
    : LevelChunk(level, blocks, x, z) {
    dontSave = true;
    // Set this as fully post-processed, so we don't try and run post-processing
    // on any edge chunks that will overlap into real chunks
    terrainPopulated = LevelChunk::sTerrainPopulatedAllNeighbours |
                       LevelChunk::sTerrainPostPostProcessed;
}

bool EmptyLevelChunk::isAt(int x, int z) {
    return x == this->x && z == this->z;
}

int EmptyLevelChunk::getHeightmap(int x, int z) { return 0; }

void EmptyLevelChunk::recalcBlockLights() {}

void EmptyLevelChunk::recalcHeightmapOnly() {}

void EmptyLevelChunk::recalcHeightmap() {}

void EmptyLevelChunk::lightLava() {}

int EmptyLevelChunk::getTile(int x, int y, int z) { return 0; }

bool EmptyLevelChunk::setTileAndData(int x, int y, int z, int _tile,
                                     int _data) {
    return true;
}

bool EmptyLevelChunk::setTile(int x, int y, int z, int _tile) { return true; }

int EmptyLevelChunk::getData(int x, int y, int z) { return 0; }

bool EmptyLevelChunk::setData(int x, int y, int z, int val, int mask,
                              bool* maskedBitsChanged) {
    *maskedBitsChanged = true;
    return false;
}

int EmptyLevelChunk::getBrightness(LightLayer::variety layer, int x, int y,
                                   int z) {
    return 0;
}

// 4J added
void EmptyLevelChunk::getNeighbourBrightnesses(int* brightnesses,
                                               LightLayer::variety layer, int x,
                                               int y, int z) {
    for (int i = 0; i < 6; i++) {
        brightnesses[i] = 0;
    }
}

void EmptyLevelChunk::setBrightness(LightLayer::variety layer, int x, int y,
                                    int z, int brightness) {}

int EmptyLevelChunk::getRawBrightness(int x, int y, int z, int skyDampen) {
    return 0;
}

void EmptyLevelChunk::addEntity(std::shared_ptr<Entity> e) {}

void EmptyLevelChunk::removeEntity(std::shared_ptr<Entity> e) {}

void EmptyLevelChunk::removeEntity(std::shared_ptr<Entity> e, int yc) {}

bool EmptyLevelChunk::isSkyLit(int x, int y, int z) { return false; }

void EmptyLevelChunk::skyBrightnessChanged() {}

std::shared_ptr<TileEntity> EmptyLevelChunk::getTileEntity(int x, int y,
                                                           int z) {
    return std::shared_ptr<TileEntity>();
}

void EmptyLevelChunk::addTileEntity(std::shared_ptr<TileEntity> te) {}

void EmptyLevelChunk::setTileEntity(int x, int y, int z,
                                    std::shared_ptr<TileEntity> tileEntity) {}

void EmptyLevelChunk::removeTileEntity(int x, int y, int z) {}

void EmptyLevelChunk::load() {}

void EmptyLevelChunk::unload(bool unloadTileEntities)  // 4J - added parameter
{}

bool EmptyLevelChunk::containsPlayer() { return false; }

void EmptyLevelChunk::markUnsaved() {}

void EmptyLevelChunk::getEntities(std::shared_ptr<Entity> except, AABB bb,
                                  std::vector<std::shared_ptr<Entity> >& es,
                                  EntitySelector* selector) {}

void EmptyLevelChunk::getEntitiesOfClass(
    const std::type_info& ec, AABB bb,
    std::vector<std::shared_ptr<Entity> >& es, EntitySelector* selector) {}

int EmptyLevelChunk::countEntities() { return 0; }

bool EmptyLevelChunk::shouldSave(bool force) { return false; }

void EmptyLevelChunk::setBlocks(std::vector<uint8_t>& newBlocks, int sub) {}

int EmptyLevelChunk::getBlocksAndData(std::vector<uint8_t>& data, int x0,
                                      int y0, int z0, int x1, int y1, int z1,
                                      int p, bool includeLighting /* = true*/) {
    int xs = x1 - x0;
    int ys = y1 - y0;
    int zs = z1 - z0;

    int s = xs * ys * zs;
    int len;
    if (includeLighting) {
        len = s + s / 2 * 3;
    } else {
        len = s + s / 2;
    }

    Arrays::fill(data, p, p + len, (uint8_t)0);
    return len;
}

int EmptyLevelChunk::setBlocksAndData(std::vector<uint8_t>& data, int x0,
                                      int y0, int z0, int x1, int y1, int z1,
                                      int p, bool includeLighting /* = true*/) {
    int xs = x1 - x0;
    int ys = y1 - y0;
    int zs = z1 - z0;

    int s = xs * ys * zs;
    if (includeLighting) {
        return s + s / 2 * 3;
    } else {
        return s + s / 2;
    }
}

bool EmptyLevelChunk::testSetBlocksAndData(std::vector<uint8_t>& data, int x0,
                                           int y0, int z0, int x1, int y1,
                                           int z1, int p) {
    return false;
}

Random* EmptyLevelChunk::getRandom(int64_t l) {
    return new Random((level->getSeed() + x * x * 4987142 + x * 5947611 +
                       z * z * 4392871l + z * 389711) ^
                      l);
}

bool EmptyLevelChunk::isEmpty() { return true; }
