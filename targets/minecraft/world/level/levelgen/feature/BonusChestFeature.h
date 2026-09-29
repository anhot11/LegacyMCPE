#pragma once
#include <format>
#include <vector>

#include "Feature.h"

class Random;
class Level;
class WeighedTreasure;

class BonusChestFeature : public Feature {
private:
    const std::vector<WeighedTreasure*> treasureList;
    const int numRolls;

public:
    BonusChestFeature(std::vector<WeighedTreasure*>& treasureList,
                      int numRolls);

    virtual bool place(Level* level, Random* random, int x, int y, int z);
    bool place(Level* level, Random* random, int x, int y, int z,
               bool force);  // 4J added this method with extra force parameter
};
