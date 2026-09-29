#pragma once
#include <stdint.h>

#include <string>
#include <vector>

#include "ChunkSource.h"
#include "EmptyLevelChunk.h"
#include "LevelChunk.h"
#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/chunk/ChunkSource.h"
#include "minecraft/world/level/chunk/storage/ChunkStorage.h"

class ProgressListener;
class ChunkStorage;
class Level;
class LevelChunk;

class ReadOnlyChunkCache : public ChunkSource {
private:
    static const int LEN = 16;
    static const int LEN_MASK = LEN - 1;
    std::vector<LevelChunk*> chunks;
    Level* level;
    ChunkStorage* storage;

public:
    ReadOnlyChunkCache(Level* level, ChunkStorage* storage);
    virtual ~ReadOnlyChunkCache();

    virtual bool hasChunk(int x, int z);
    std::vector<uint8_t> emptyPixels;
    virtual LevelChunk* create(int x, int z);
    virtual LevelChunk* getChunk(int x, int z);

private:
    LevelChunk* load(int x, int z);  // 4J - TODO - was synchronized

public:
    virtual void postProcess(ChunkSource* parent, int x, int z);
    virtual bool save(bool force, ProgressListener* progressListener);
    virtual bool tick();
    virtual bool shouldSave();
    virtual std::string gatherStats();

    virtual std::vector<Biome::MobSpawnerData*>* getMobsAt(
        MobCategory* mobCategory, int x, int y, int z);
    virtual TilePos* findNearestMapFeature(Level* level,
                                           const std::string& featureName,
                                           int x, int y, int z);
};
