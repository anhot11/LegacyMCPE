#pragma once

#include <memory>

#include "minecraft/world/item/ItemInstance.h"

class CompoundTag;
class Item;
class Tile;

class MerchantRecipe {
private:
    std::shared_ptr<ItemInstance> buyA;
    std::shared_ptr<ItemInstance> buyB;
    std::shared_ptr<ItemInstance> sell;
    int uses;
    int maxUses;

    void _init(std::shared_ptr<ItemInstance> buyA,
               std::shared_ptr<ItemInstance> buyB,
               std::shared_ptr<ItemInstance> sell);

public:
    MerchantRecipe(CompoundTag* tag);
    MerchantRecipe(std::shared_ptr<ItemInstance> buyA,
                   std::shared_ptr<ItemInstance> buyB,
                   std::shared_ptr<ItemInstance> sell, int uses = 0,
                   int maxUses = 7);
    MerchantRecipe(std::shared_ptr<ItemInstance> buy,
                   std::shared_ptr<ItemInstance> sell);
    MerchantRecipe(std::shared_ptr<ItemInstance> buy, Item* sell);
    MerchantRecipe(std::shared_ptr<ItemInstance> buy, Tile* sell);

    std::shared_ptr<ItemInstance> getBuyAItem();
    std::shared_ptr<ItemInstance> getBuyBItem();
    bool hasSecondaryBuyItem();
    std::shared_ptr<ItemInstance> getSellItem();
    bool isSame(MerchantRecipe* other);
    bool isSameSameButBetter(MerchantRecipe* other);
    int getUses();
    int getMaxUses();
    void increaseUses();
    void increaseMaxUses(int amount);
    bool isDeprecated();
    void enforceDeprecated();
    void load(CompoundTag* tag);
    CompoundTag* createTag();
};