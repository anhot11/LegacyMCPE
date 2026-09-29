#pragma once
#include <stdint.h>

#include <format>
#include <string>
#include <vector>

#include "LargeFeature.h"
#include "LargeHellCaveFeature.h"
#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/chunk/ChunkSource.h"
#include "minecraft/world/level/chunk/LevelChunk.h"
#include "minecraft/world/level/levelgen/feature/FlowerFeature.h"
#include "minecraft/world/level/levelgen/feature/HellFireFeature.h"
#include "minecraft/world/level/levelgen/feature/HellPortalFeature.h"
#include "minecraft/world/level/levelgen/feature/HellSpringFeature.h"
#include "minecraft/world/level/levelgen/feature/LightGemFeature.h"
#include "minecraft/world/level/levelgen/synth/PerlinNoise.h"

class ProgressListener;
class Level;
class Random;

class HellFlatLevelSource : public ChunkSource {
public:
    static const int CHUNK_HEIGHT = 8;
    static const int CHUNK_WIDTH = 4;

private:
    Random* random;
    Random* pprandom;

private:
    Level* level;

public:
    HellFlatLevelSource(Level* level, int64_t seed);
    ~HellFlatLevelSource();

private:
    void prepareHeights(int xOffs, int zOffs, std::vector<uint8_t>& blocks);

public:
    void buildSurfaces(int xOffs, int zOffs, std::vector<uint8_t>& blocks);

    LevelChunk* create(int x, int z);
    LevelChunk* getChunk(int xOffs, int zOffs);
    virtual void lightChunk(LevelChunk* lc);  // 4J added

public:
    virtual bool hasChunk(int x, int y);
    void postProcess(ChunkSource* parent, int xt, int zt);
    bool save(bool force, ProgressListener* progressListener);
    bool tick();
    bool shouldSave();
    std::string gatherStats();
    virtual std::vector<Biome::MobSpawnerData*>* getMobsAt(
        MobCategory* mobCategory, int x, int y, int z);
    virtual TilePos* findNearestMapFeature(Level* level,
                                           const std::string& featureName,
                                           int x, int y, int z);
    virtual void recreateLogicStructuresForChunk(int chunkX, int chunkZ);
};
