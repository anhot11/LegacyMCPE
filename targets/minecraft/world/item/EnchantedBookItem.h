#pragma once

#include <format>
#include <memory>
#include <string>
#include <vector>

#include "Item.h"
#include "minecraft/world/item/ItemInstance.h"

class EnchantmentInstance;
class CompoundTag;
class Enchantment;
class Random;
class WeighedTreasure;
template <class T>
class ListTag;

class EnchantedBookItem : public Item {
public:
    static const std::string TAG_STORED_ENCHANTMENTS;

    EnchantedBookItem(int id);

    bool isFoil(std::shared_ptr<ItemInstance> itemInstance);
    bool isEnchantable(std::shared_ptr<ItemInstance> itemInstance);
    const Rarity* getRarity(std::shared_ptr<ItemInstance> itemInstance);
    ListTag<CompoundTag>* getEnchantments(std::shared_ptr<ItemInstance> item);
    void appendHoverText(std::shared_ptr<ItemInstance> itemInstance,
                         std::shared_ptr<Player> player,
                         std::vector<HtmlString>* lines, bool advanced);
    void addEnchantment(std::shared_ptr<ItemInstance> item,
                        EnchantmentInstance* enchantment);
    std::shared_ptr<ItemInstance> createForEnchantment(
        EnchantmentInstance* enchant);
    void createForEnchantment(
        Enchantment* enchant,
        std::vector<std::shared_ptr<ItemInstance> >* items);
    std::shared_ptr<ItemInstance> createForRandomLoot(Random* random);
    WeighedTreasure* createForRandomTreasure(Random* random);
    WeighedTreasure* createForRandomTreasure(Random* random, int minCount,
                                             int maxCount, int weight);
};