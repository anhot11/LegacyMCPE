#include "DigDurabilityEnchantment.h"

#include "java/Random.h"
#include "minecraft/world/item/ArmorItem.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/enchantment/Enchantment.h"
#include "minecraft/world/item/enchantment/EnchantmentCategory.h"
#include "strings.h"

DigDurabilityEnchantment::DigDurabilityEnchantment(int id, int frequency)
    : Enchantment(id, frequency, EnchantmentCategory::digger) {
    setDescriptionId(IDS_ENCHANTMENT_DURABILITY);
}

int DigDurabilityEnchantment::getMinCost(int level) {
    return 5 + (level - 1) * 8;
}

int DigDurabilityEnchantment::getMaxCost(int level) {
    return Enchantment::getMinCost(level) + 50;
}

int DigDurabilityEnchantment::getMaxLevel() { return 3; }

bool DigDurabilityEnchantment::canEnchant(std::shared_ptr<ItemInstance> item) {
    if (item->isDamageableItem()) return true;
    return Enchantment::canEnchant(item);
}

bool DigDurabilityEnchantment::shouldIgnoreDurabilityDrop(
    std::shared_ptr<ItemInstance> item, int level, Random* random) {
    ArmorItem* armor = dynamic_cast<ArmorItem*>(item->getItem());
    if (armor && random->nextFloat() < 0.6f) return false;
    return random->nextInt(level + 1) > 0;
}