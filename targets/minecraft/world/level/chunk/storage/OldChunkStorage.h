#pragma once
#include <cstdint>
#include <vector>

#include "ChunkStorage.h"
#include "java/File.h"
#include "minecraft/world/level/chunk/LevelChunk.h"
#include "nbt/CompoundTag.h"
#include "nbt/NbtIo.h"

class Level;
class CompoundTag;
class DataInputStream;
class DataOutputStream;
class LevelChunk;

class OldChunkStorage : public ChunkStorage {
private:
    // 4J added so we can have separate storage arrays for different threads
    class ThreadStorage {
    public:
        std::vector<uint8_t> blockData;
        std::vector<uint8_t> dataData;
        std::vector<uint8_t> skyLightData;
        std::vector<uint8_t> blockLightData;

        ThreadStorage();
        ~ThreadStorage();
    };
    static thread_local ThreadStorage* m_tlsStorage;
    static ThreadStorage* m_defaultThreadStorage;

public:
    // Each new thread that needs to use Compression will need to call one of
    // the following 2 functions, to either create its own local storage, or
    // share the default storage already allocated by the main thread
    static void CreateNewThreadStorage();
    static void UseDefaultThreadStorage();
    static void ReleaseThreadStorage();

private:
    File dir;
    bool create;

public:
    OldChunkStorage(File dir, bool create);

private:
    File getFile(int x, int z);
    LevelChunk* load(Level* level, int x, int z);

public:
    virtual void save(Level* level, LevelChunk* levelChunk);

    static bool saveEntities(LevelChunk* lc, Level* level,
                             CompoundTag* tag);  // 4J Added
    static void save(LevelChunk* lc, Level* level,
                     DataOutputStream* dos);  // 4J Added
    static void save(LevelChunk* lc, Level* level, CompoundTag* tag);
    static void loadEntities(LevelChunk* lc, Level* level, CompoundTag* tag);
    static LevelChunk* load(Level* level, CompoundTag* tag);
    static LevelChunk* load(Level* level, DataInputStream* dis);  // 4J Added

    virtual void tick();
    virtual void flush();
    virtual void saveEntities(Level* level, LevelChunk* levelChunk);
};
