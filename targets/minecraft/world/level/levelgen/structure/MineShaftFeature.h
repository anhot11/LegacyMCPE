#pragma once

#include <string>
#include <unordered_map>

#include "StructureFeature.h"
#include "minecraft/world/level/levelgen/structure/StructureFeature.h"

class MineShaftFeature : public StructureFeature {
public:
    static const std::string OPTION_CHANCE;

private:
    double chance;

public:
    MineShaftFeature();

    std::string getFeatureName();

    MineShaftFeature(std::unordered_map<std::string, std::string> options);

protected:
    virtual bool isFeatureChunk(int x, int z, bool bIsSuperflat = false);
    virtual StructureStart* createStructureStart(int x, int z);
};