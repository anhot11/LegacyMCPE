#include "ReadOnlyChunkCache.h"

#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/chunk/EmptyLevelChunk.h"
#include "minecraft/world/level/chunk/LevelChunk.h"
#include "minecraft/world/level/chunk/storage/ChunkStorage.h"

ReadOnlyChunkCache::ReadOnlyChunkCache(Level* level, ChunkStorage* storage) {
    chunks = std::vector<LevelChunk*>(LEN * LEN);
    emptyPixels = std::vector<uint8_t>(Level::CHUNK_TILE_COUNT);

    this->level = level;
    this->storage = storage;
}

ReadOnlyChunkCache::~ReadOnlyChunkCache() {
    for (unsigned int i = 0; i < chunks.size(); ++i) delete chunks[i];
}

bool ReadOnlyChunkCache::hasChunk(int x, int z) {
    int slot = (x & LEN_MASK) | ((z & LEN_MASK) * LEN);
    return chunks[slot] != nullptr && (chunks[slot]->isAt(x, z));
}

LevelChunk* ReadOnlyChunkCache::create(int x, int z) { return getChunk(x, z); }

LevelChunk* ReadOnlyChunkCache::getChunk(int x, int z) {
    int slot = (x & LEN_MASK) | ((z & LEN_MASK) * LEN);
    // 4J - removed try/catch
    //    try {
    if (!hasChunk(x, z)) {
        LevelChunk* newChunk = load(x, z);
        if (newChunk == nullptr) {
            newChunk = new EmptyLevelChunk(level, emptyPixels, x, z);
        }
        chunks[slot] = newChunk;
    }
    return chunks[slot];
    //    } catch (Exception e) {
    //        e.printStackTrace();
    //        return null;
    //    }
}

LevelChunk* ReadOnlyChunkCache::load(int x, int z) {
    // 4J - remove try/catch
    //    try {
    return storage->load(level, x, z);
    //    } catch (IOException e) {
    //        e.printStackTrace();
    //        return null;
    //    }
}
// 4J - TODO - was synchronized
void ReadOnlyChunkCache::postProcess(ChunkSource* parent, int x, int z) {}

bool ReadOnlyChunkCache::save(bool force, ProgressListener* progressListener) {
    return true;
}

bool ReadOnlyChunkCache::tick() { return false; }

bool ReadOnlyChunkCache::shouldSave() { return false; }

std::string ReadOnlyChunkCache::gatherStats() { return "ReadOnlyChunkCache"; }

std::vector<Biome::MobSpawnerData*>* ReadOnlyChunkCache::getMobsAt(
    MobCategory* mobCategory, int x, int y, int z) {
    return nullptr;
}

TilePos* ReadOnlyChunkCache::findNearestMapFeature(
    Level* level, const std::string& featureName, int x, int y, int z) {
    return nullptr;
}
