#pragma once

#include <format>
#include <memory>
#include <vector>

#include "WeighedRandom.h"
#include "minecraft/util/WeighedRandom.h"

class Container;
class DispenserTileEntity;
class ItemInstance;
class Random;

class WeighedTreasure : public WeighedRandomItem {
private:
    std::shared_ptr<ItemInstance> item;
    int minCount;
    int maxCount;

public:
    WeighedTreasure(int itemId, int auxValue, int minCount, int maxCount,
                    int weight);
    WeighedTreasure(std::shared_ptr<ItemInstance> item, int minCount,
                    int maxCount, int weight);

    static void addChestItems(Random* random,
                              const std::vector<WeighedTreasure*>& items,
                              std::shared_ptr<Container> dest, int numRolls);
    static void addDispenserItems(Random* random,
                                  const std::vector<WeighedTreasure*>& items,
                                  std::shared_ptr<DispenserTileEntity> dest,
                                  int numRolls);
    static std::vector<WeighedTreasure*> addToTreasure(
        std::vector<WeighedTreasure*>& items, WeighedTreasure* extra);
};