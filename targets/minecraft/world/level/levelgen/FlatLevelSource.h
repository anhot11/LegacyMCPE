#pragma once

#include <stdint.h>

#include <format>
#include <string>
#include <vector>

#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/chunk/ChunkSource.h"

class ProgressListener;
class LargeFeature;
class StrongholdFeature;
class VillageFeature;
class MineShaftFeature;
class PerlinNoise;
class Level;
class Random;

class FlatLevelSource : public ChunkSource {
public:
    static const int CHUNK_HEIGHT = 8;
    static const int CHUNK_WIDTH = 4;

private:
    Level* level;
    Random* random;
    Random* pprandom;

    bool generateStructures;
    VillageFeature* villageFeature;  // = new VillageFeature(1);

public:
    FlatLevelSource(Level* level, int64_t seed, bool generateStructures);
    ~FlatLevelSource();

private:
    void prepareHeights(std::vector<uint8_t>& blocks);

public:
    virtual LevelChunk* create(int x, int z);
    virtual LevelChunk* getChunk(int xOffs, int zOffs);
    virtual bool hasChunk(int x, int y);
    virtual void postProcess(ChunkSource* parent, int xt, int zt);
    virtual bool save(bool force, ProgressListener* progressListener);
    virtual bool tick();
    virtual bool shouldSave();
    virtual std::string gatherStats();
    virtual std::vector<Biome::MobSpawnerData*>* getMobsAt(
        MobCategory* mobCategory, int x, int y, int z);
    virtual TilePos* findNearestMapFeature(Level* level,
                                           const std::string& featureName,
                                           int x, int y, int z);
    virtual void recreateLogicStructuresForChunk(int chunkX, int chunkZ);
};
