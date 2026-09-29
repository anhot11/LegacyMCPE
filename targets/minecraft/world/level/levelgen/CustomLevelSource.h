#pragma once

#include <stdint.h>

#include <format>
#include <string>
#include <vector>

#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/chunk/ChunkSource.h"

class Level;
class PerlinNoise;
class Random;

#ifndef _CONTENT_PACKAGE
#define _OVERRIDE_HEIGHTMAP
#endif

class ProgressListener;
class LargeFeature;
class StrongholdFeature;
class VillageFeature;
class MineShaftFeature;
class RandomScatteredLargeFeature;

class CustomLevelSource : public ChunkSource {
public:
    static const double SNOW_CUTOFF;
    static const double SNOW_SCALE;
    static const bool FLOATING_ISLANDS = false;
    static const int CHUNK_HEIGHT = 8;
    static const int CHUNK_WIDTH = 4;

private:
#ifdef _OVERRIDE_HEIGHTMAP
    Random* random;
    Random* pprandom;  // 4J - added
    PerlinNoise* perlinNoise3;
    LargeFeature* caveFeature;
    StrongholdFeature* strongholdFeature;
    VillageFeature* villageFeature;
    MineShaftFeature* mineShaftFeature;
    RandomScatteredLargeFeature* scatteredFeature;
    LargeFeature* canyonFeature;
    Level* level;
#endif

    std::vector<uint8_t> m_heightmapOverride;
    std::vector<uint8_t> m_waterheightOverride;

private:
    const bool generateStructures;

public:
    CustomLevelSource(Level* level, int64_t seed, bool generateStructures);
    ~CustomLevelSource();

public:
    void prepareHeights(int xOffs, int zOffs, std::vector<uint8_t>& blocks);

public:
    void buildSurfaces(int xOffs, int zOffs, std::vector<uint8_t>& blocks,
                       std::vector<Biome*>& biomes);

private:
    virtual LevelChunk* create(int x, int z);

public:
    virtual LevelChunk* getChunk(int xOffs, int zOffs);
    virtual void lightChunk(LevelChunk* lc);  // 4J added

public:
    virtual bool hasChunk(int x, int y);

private:
    void calcWaterDepths(ChunkSource* parent, int xt, int zt);

public:
    virtual void postProcess(ChunkSource* parent, int xt, int zt);
    virtual bool save(bool force, ProgressListener* progressListener);
    virtual bool tick();
    virtual bool shouldSave();
    virtual std::string gatherStats();

public:
    virtual std::vector<Biome::MobSpawnerData*>* getMobsAt(
        MobCategory* mobCategory, int x, int y, int z);
    virtual TilePos* findNearestMapFeature(Level* level,
                                           const std::string& featureName,
                                           int x, int y, int z);
    virtual void recreateLogicStructuresForChunk(int chunkX, int chunkZ);
};
