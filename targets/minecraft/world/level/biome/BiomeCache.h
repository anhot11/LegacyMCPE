#pragma once
#include <stdint.h>

#include <format>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "java/JavaIntHash.h"
#include "minecraft/world/level/biome/Biome.h"

class BiomeSource;

class BiomeCache {
private:
    static const int DECAY_TIME = 1000 * 30;
    static const int ZONE_SIZE_BITS = 4;
    static const int ZONE_SIZE = 1 << ZONE_SIZE_BITS;
    static const int ZONE_SIZE_MASK = ZONE_SIZE - 1;

    const BiomeSource* source;
    int64_t lastUpdateTime;

public:
    class Block {
    public:
        // MGH - changed this to just cache biome indices, as we have direct
        // access to the data if we know the index.
        // 		std::vector<float> temps;
        // 		std::vector<float> downfall;
        // 		std::vector<Biome*> biomes;
        std::vector<uint8_t> biomeIndices;
        int x, z;
        int64_t lastUse;

        Block(int x, int z, BiomeCache* parent);
        ~Block();
        Biome* getBiome(int x, int z);
        float getTemperature(int x, int z);
        float getDownfall(int x, int z);
    };

private:
    std::unordered_map<int64_t, Block*, LongKeyHash, LongKeyEq>
        cached;               // 4J - was LongHashMap
    std::vector<Block*> all;  // was ArrayList<Block>

public:
    BiomeCache(BiomeSource* source);
    ~BiomeCache();

    Block* getBlockAt(int x, int z);
    Biome* getBiome(int x, int z);
    float getTemperature(int x, int z);
    float getDownfall(int x, int z);
    void update();
    std::vector<Biome*> getBiomeBlockAt(int x, int z);
    std::vector<uint8_t> getBiomeIndexBlockAt(int x, int z);

private:
    std::mutex m_CS;
};