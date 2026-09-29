#pragma once
#include <stdint.h>

#include <format>
#include <vector>

#include "BiomeSource.h"

class Biome;

class FixedBiomeSource : public BiomeSource {
private:
    Biome* biome;
    float temperature, downfall;

public:
    using BiomeSource::getTemperature;

    FixedBiomeSource(Biome* fixed, float temperature, float downfall);

    virtual Biome* getBiome(ChunkPos* cp);
    virtual Biome* getBiome(int x, int z);
    virtual float getTemperature(int x, int z);
    virtual void getTemperatureBlock(std::vector<float>& temperatures, int x,
                                     int z, int w, int h) const;
    virtual std::vector<float> getTemperatureBlock(int x, int z, int w,
                                                   int h) const;
    virtual void getTemperatureBlock(std::vector<double>& temperatures, int x,
                                     int z, int w, int h) const;
    virtual void getDownfallBlock(std::vector<float>& downfalls, int x, int z,
                                  int w, int h) const;
    virtual std::vector<float> getDownfallBlock(int x, int z, int w,
                                                int h) const;
    virtual float getDownfall(int x, int z) const;
    virtual void getDownfallBlock(std::vector<double>& downfalls, int x, int z,
                                  int w, int h);
    virtual void getBiomeBlock(std::vector<Biome*>& biomes, int x, int z, int w,
                               int h, bool useCache) const;
    virtual void getBiomeIndexBlock(std::vector<uint8_t>& biomeIndices, int x,
                                    int z, int w, int h, bool useCache) const;

    // 4J-PB added in from beyond 1.8.2
    virtual std::vector<Biome*> getRawBiomeBlock(int x, int z, int w,
                                                 int h) const;
    virtual void getRawBiomeBlock(std::vector<Biome*>& biomes, int x, int z,
                                  int w, int h) const;

    ////////////////////////////////////
    virtual TilePos* findBiome(int x, int z, int r, Biome* toFind,
                               Random* random);
    virtual TilePos* findBiome(int x, int z, int r,
                               const std::vector<Biome*>& allowed,
                               Random* random);
    virtual bool containsOnly(int x, int z, int r, Biome* allowed);
    virtual bool containsOnly(int x, int z, int r,
                              const std::vector<Biome*>& allowed);
};
