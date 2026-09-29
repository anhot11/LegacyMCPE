#pragma once
#include <format>
#include <string>
#include <vector>

#include "StructureFeature.h"
#include "StructureStart.h"
#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/levelgen/structure/StructureFeature.h"
#include "minecraft/world/level/levelgen/structure/StructureFeatureIO.h"

class Random;
class ChunkPos;
class Level;

class NetherBridgeFeature : public StructureFeature {
private:
    std::vector<Biome::MobSpawnerData*> bridgeEnemies;
    bool isSpotSelected;
    ChunkPos* netherFortressPos;

public:
    NetherBridgeFeature();
    ~NetherBridgeFeature();
    std::string getFeatureName();
    std::vector<Biome::MobSpawnerData*>* getBridgeEnemies();

protected:
    virtual bool isFeatureChunk(int x, int z, bool bIsSuperflat);
    virtual StructureStart* createStructureStart(int x, int z);

public:
    void clearCachedBuildings();

    class NetherBridgeStart : public StructureStart {
    public:
        static StructureStart* Create() { return new NetherBridgeStart(); }
        virtual EStructureStart GetType() {
            return eStructureStart_NetherBridgeStart;
        }

    public:
        NetherBridgeStart();
        NetherBridgeStart(Level* level, Random* random, int chunkX, int chunkZ);
    };
};
