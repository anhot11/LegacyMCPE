#pragma once
#include <format>
#include <string>
#include <unordered_map>
#include <vector>

#include "StructureFeature.h"
#include "StructureStart.h"
#include "minecraft/world/level/levelgen/structure/StructureFeature.h"
#include "minecraft/world/level/levelgen/structure/StructureFeatureIO.h"

class Biome;
class Level;
class Random;

class VillageFeature : public StructureFeature {
public:
    static const std::string OPTION_SIZE_MODIFIER;
    static const std::string OPTION_SPACING;

private:
    int villageSizeModifier;
    int townSpacing;
    int minTownSeparation;

    void _init(int iXZSize);

public:
    static void staticCtor();
    static std::vector<Biome*> allowedBiomes;
    VillageFeature(int iXZSize);
    VillageFeature(std::unordered_map<std::string, std::string> options,
                   int iXZSize);
    std::string getFeatureName();

protected:
    virtual bool isFeatureChunk(int x, int z, bool bIsSuperflat = false);
    virtual StructureStart* createStructureStart(int x, int z);

public:
    class VillageStart : public StructureStart {
    public:
        static StructureStart* Create() { return new VillageStart(); }
        virtual EStructureStart GetType() {
            return eStructureStart_VillageStart;
        }

    private:
        bool valid;
        int m_iXZSize;

    public:
        VillageStart();
        VillageStart(Level* level, Random* random, int chunkX, int chunkZ,
                     int villageSizeModifier, int iXZSize);
        bool isValid();
        void addAdditonalSaveData(CompoundTag* tag);
        void readAdditonalSaveData(CompoundTag* tag);
    };

    int m_iXZSize;
};
