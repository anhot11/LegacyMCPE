#include "FixedBiomeSource.h"

#include <algorithm>
#include <vector>

#include "java/Arrays.h"
#include "java/Random.h"
#include "minecraft/world/level/TilePos.h"
#include "minecraft/world/level/biome/Biome.h"

FixedBiomeSource::FixedBiomeSource(Biome* fixed, float temperature,
                                   float downfall) {
    biome = fixed;
    this->temperature = temperature;
    this->downfall = downfall;
}

Biome* FixedBiomeSource::getBiome(ChunkPos* cp) { return biome; }

Biome* FixedBiomeSource::getBiome(int x, int z) { return biome; }

float FixedBiomeSource::getTemperature(int x, int z) { return temperature; }

void FixedBiomeSource::getTemperatureBlock(std::vector<float>& temperatures,
                                           int x, int z, int w, int h) const {
    if (temperatures.empty() || temperatures.size() < (unsigned int)(w * h)) {
        temperatures = std::vector<float>(w * h);
    }

    Arrays::fill(temperatures, 0, w * h, temperature);
}

std::vector<float> FixedBiomeSource::getTemperatureBlock(int x, int z, int w,
                                                         int h) const {
    std::vector<float> temps(w * h);
    getTemperatureBlock(temps, x, z, w, h);
    return temps;
}

// 4J - note that caller is responsible for deleting returned array.
// temperatures array is for output only.
void FixedBiomeSource::getTemperatureBlock(std::vector<double>& temperatures,
                                           int x, int z, int w, int h) const {
    temperatures = std::vector<double>(w * h);

    Arrays::fill(temperatures, 0, w * h, (double)temperature);
}

void FixedBiomeSource::getDownfallBlock(std::vector<float>& downfalls, int x,
                                        int z, int w, int h) const {
    if (downfalls.empty() || downfalls.size() < (unsigned int)(w * h)) {
        downfalls = std::vector<float>(w * h);
    }
    Arrays::fill(downfalls, 0, w * h, downfall);
}

std::vector<float> FixedBiomeSource::getDownfallBlock(int x, int z, int w,
                                                      int h) const {
    std::vector<float> downfalls(w * h);
    getDownfallBlock(downfalls, x, z, w, h);
    return downfalls;
}

float FixedBiomeSource::getDownfall(int x, int z) const { return downfall; }

void FixedBiomeSource::getDownfallBlock(std::vector<double>& downfalls, int x,
                                        int z, int w, int h) {
    if (downfalls.empty() || downfalls.size() < (unsigned int)(w * h)) {
        downfalls = std::vector<double>(w * h);
    }
    Arrays::fill(downfalls, 0, w * h, (double)downfall);
}

// 4J - caller is responsible for deleting biomes array, plus any optional
// arrays output if pointers are passed in (_temperatures, _downfalls)
void FixedBiomeSource::getBiomeBlock(std::vector<Biome*>& biomes, int x, int z,
                                     int w, int h, bool useCache) const {
    biomes = std::vector<Biome*>(w * h);

    Arrays::fill(biomes, 0, w * h, biome);
}

// 4J - caller is responsible for deleting biomes array, plus any optional
// arrays output if pointers are passed in (_temperatures, _downfalls)
void FixedBiomeSource::getBiomeIndexBlock(std::vector<uint8_t>& biomeIndices,
                                          int x, int z, int w, int h,
                                          bool useCache) const {
    biomeIndices = std::vector<uint8_t>(w * h);
    int biomeIndex = biome->id;
    Arrays::fill(biomeIndices, 0, w * h, biomeIndex);
}

// 4J-PB added in from beyond 1.8.2
// 4J - caller is responsible for deleting biomes array, plus any optional
// arrays output if pointers are passed in (_temperatures, _downfalls)
void FixedBiomeSource::getRawBiomeBlock(std::vector<Biome*>& biomes, int x,
                                        int z, int w, int h) const {
    biomes = std::vector<Biome*>(w * h);

    Arrays::fill(biomes, 0, w * h, biome);
}

// 4J-PB added in from beyond 1.8.2
// 4J - caller is responsible for deleting biomes array, plus any optional
// arrays output if pointers are passed in (_temperatures, _downfalls)
std::vector<Biome*> FixedBiomeSource::getRawBiomeBlock(int x, int z, int w,
                                                       int h) const {
    std::vector<Biome*> biomes;
    getRawBiomeBlock(biomes, x, z, w, h);
    return biomes;
}

TilePos* FixedBiomeSource::findBiome(int x, int z, int r, Biome* toFind,
                                     Random* random) {
    if (toFind == biome) {
        return new TilePos(x - r + random->nextInt(r * 2 + 1), 0,
                           z - r + random->nextInt(r * 2 + 1));
    }
    return nullptr;
}

TilePos* FixedBiomeSource::findBiome(int x, int z, int r,
                                     const std::vector<Biome*>& allowed,
                                     Random* random) {
    if (find(allowed.begin(), allowed.end(), biome) != allowed.end()) {
        return new TilePos(x - r + random->nextInt(r * 2 + 1), 0,
                           z - r + random->nextInt(r * 2 + 1));
    }

    return nullptr;
}

bool FixedBiomeSource::containsOnly(int x, int z, int r, Biome* allowed) {
    return allowed == biome;
}

bool FixedBiomeSource::containsOnly(int x, int z, int r,
                                    const std::vector<Biome*>& allowed) {
    return find(allowed.begin(), allowed.end(), biome) != allowed.end();
}