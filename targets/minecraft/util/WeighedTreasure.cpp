#include "WeighedTreasure.h"

#include "java/Random.h"
#include "minecraft/util/WeighedRandom.h"
#include "minecraft/world/Container.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/tile/entity/DispenserTileEntity.h"

WeighedTreasure::WeighedTreasure(int itemId, int auxValue, int minCount,
                                 int maxCount, int weight)
    : WeighedRandomItem(weight) {
    this->item = std::make_shared<ItemInstance>(itemId, 1, auxValue);
    this->minCount = minCount;
    this->maxCount = maxCount;
}

WeighedTreasure::WeighedTreasure(std::shared_ptr<ItemInstance> item,
                                 int minCount, int maxCount, int weight)
    : WeighedRandomItem(weight) {
    this->item = item;
    this->minCount = minCount;
    this->maxCount = maxCount;
}

void WeighedTreasure::addChestItems(Random* random,
                                    const std::vector<WeighedTreasure*>& items,
                                    std::shared_ptr<Container> dest,
                                    int numRolls) {
    for (int r = 0; r < numRolls; r++) {
        WeighedTreasure* treasure =
            (WeighedTreasure*)WeighedRandom::getRandomItem(
                random, *((std::vector<WeighedRandomItem*>*)&items));

        int count =
            treasure->minCount +
            random->nextInt(treasure->maxCount - treasure->minCount + 1);
        if (treasure->item->getMaxStackSize() >= count) {
            std::shared_ptr<ItemInstance> copy = treasure->item->copy();
            copy->count = count;
            dest->setItem(random->nextInt(dest->getContainerSize()), copy);
        } else {
            // use multiple slots
            for (int c = 0; c < count; c++) {
                std::shared_ptr<ItemInstance> copy = treasure->item->copy();
                copy->count = 1;
                dest->setItem(random->nextInt(dest->getContainerSize()), copy);
            }
        }
    }
}

void WeighedTreasure::addDispenserItems(
    Random* random, const std::vector<WeighedTreasure*>& items,
    std::shared_ptr<DispenserTileEntity> dest, int numRolls) {
    for (int r = 0; r < numRolls; r++) {
        WeighedTreasure* treasure =
            (WeighedTreasure*)WeighedRandom::getRandomItem(
                random, *((std::vector<WeighedRandomItem*>*)&items));

        int count =
            treasure->minCount +
            random->nextInt(treasure->maxCount - treasure->minCount + 1);
        if (treasure->item->getMaxStackSize() >= count) {
            std::shared_ptr<ItemInstance> copy = treasure->item->copy();
            copy->count = count;
            dest->setItem(random->nextInt(dest->getContainerSize()), copy);
        } else {
            // use multiple slots
            for (int c = 0; c < count; c++) {
                std::shared_ptr<ItemInstance> copy = treasure->item->copy();
                copy->count = 1;
                dest->setItem(random->nextInt(dest->getContainerSize()), copy);
            }
        }
    }
}

std::vector<WeighedTreasure*> WeighedTreasure::addToTreasure(
    std::vector<WeighedTreasure*>& items, WeighedTreasure* extra) {
    std::vector<WeighedTreasure*> result(items.size() + 1);
    int i = 0;

    for (int j = 0; j < items.size(); j++) {
        result[i++] = items[j];
    }

    result[i++] = extra;

    return result;
}