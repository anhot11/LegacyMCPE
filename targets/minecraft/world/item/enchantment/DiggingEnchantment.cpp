#include "DiggingEnchantment.h"

#include <memory>

#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/enchantment/Enchantment.h"
#include "minecraft/world/item/enchantment/EnchantmentCategory.h"
#include "strings.h"

DiggingEnchantment::DiggingEnchantment(int id, int frequency)
    : Enchantment(id, frequency, EnchantmentCategory::digger) {
    setDescriptionId(IDS_ENCHANTMENT_DIGGING);
}

int DiggingEnchantment::getMinCost(int level) { return 1 + 10 * (level - 1); }

int DiggingEnchantment::getMaxCost(int level) {
    return Enchantment::getMinCost(level) + 50;
}

int DiggingEnchantment::getMaxLevel() { return 5; }

bool DiggingEnchantment::canEnchant(std::shared_ptr<ItemInstance> item) {
    if (item->getItem()->id == Item::shears_Id) return true;
    return Enchantment::canEnchant(item);
}