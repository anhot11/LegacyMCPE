#pragma once

#include <format>
#include <string>
#include <unordered_map>
#include <vector>

#include "StructureFeature.h"
#include "StructureStart.h"
#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/levelgen/structure/StructureFeature.h"
#include "minecraft/world/level/levelgen/structure/StructureFeatureIO.h"

class Level;
class Random;

class RandomScatteredLargeFeature : public StructureFeature {
public:
    static const std::string OPTION_SPACING;

    static void staticCtor();
    static std::vector<Biome*> allowedBiomes;

private:
    std::vector<Biome::MobSpawnerData*> swamphutEnemies;
    int spacing;
    int minSeparation;

    void _init();

public:
    RandomScatteredLargeFeature();
    RandomScatteredLargeFeature(
        std::unordered_map<std::string, std::string> options);

    std::string getFeatureName();

protected:
    virtual bool isFeatureChunk(int x, int z, bool bIsSuperflat = false);
    StructureStart* createStructureStart(int x, int z);

public:
    class ScatteredFeatureStart : public StructureStart {
    public:
        static StructureStart* Create() { return new ScatteredFeatureStart(); }
        virtual EStructureStart GetType() {
            return eStructureStart_ScatteredFeatureStart;
        }

    public:
        ScatteredFeatureStart();
        ScatteredFeatureStart(Level* level, Random* random, int chunkX,
                              int chunkZ);
    };

public:
    bool isSwamphut(int cellX, int cellY, int cellZ);
    std::vector<Biome::MobSpawnerData*>* getSwamphutEnemies();
};