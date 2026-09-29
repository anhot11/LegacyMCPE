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
#include "minecraft/world/level/levelgen/structure/NetherBridgeFeature.h"
#include "minecraft/world/level/levelgen/synth/PerlinNoise.h"

class ProgressListener;
class LargeFeature;
class Level;
class NetherBridgeFeature;
class PerlinNoise;
class Random;

class HellRandomLevelSource : public ChunkSource {
public:
    static const int CHUNK_HEIGHT = 8;
    static const int CHUNK_WIDTH = 4;

private:
    Random* random;
    Random* pprandom;  // 4J added

    PerlinNoise* lperlinNoise1;
    PerlinNoise* lperlinNoise2;
    PerlinNoise* perlinNoise1;
    PerlinNoise* perlinNoise2;
    PerlinNoise* perlinNoise3;

public:
    PerlinNoise* scaleNoise;
    PerlinNoise* depthNoise;

private:
    Level* level;

public:
    HellRandomLevelSource(Level* level, int64_t seed);
    ~HellRandomLevelSource();

    NetherBridgeFeature* netherBridgeFeature;

private:
    void prepareHeights(int xOffs, int zOffs, std::vector<uint8_t>& blocks);

public:
    void buildSurfaces(int xOffs, int zOffs, std::vector<uint8_t>& blocks);

private:
    LargeFeature* caveFeature;

public:
    LevelChunk* create(int x, int z);
    LevelChunk* getChunk(int xOffs, int zOffs);
    virtual void lightChunk(LevelChunk* lc);  // 4J added

private:
    std::vector<double> getHeights(std::vector<double>& buffer, int x, int y,
                                   int z, int xSize, int ySize, int zSize);

public:
    bool hasChunk(int x, int y);
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
