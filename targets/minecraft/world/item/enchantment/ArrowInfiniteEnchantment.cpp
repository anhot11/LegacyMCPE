#include "ArrowInfiniteEnchantment.h"

#include "minecraft/world/item/enchantment/Enchantment.h"
#include "minecraft/world/item/enchantment/EnchantmentCategory.h"
#include "strings.h"

ArrowInfiniteEnchantment::ArrowInfiniteEnchantment(int id, int frequency)
    : Enchantment(id, frequency, EnchantmentCategory::bow) {
    setDescriptionId(IDS_ENCHANTMENT_ARROW_INFINITE);
}

int ArrowInfiniteEnchantment::getMinCost(int level) { return 20; }

int ArrowInfiniteEnchantment::getMaxCost(int level) { return 50; }

int ArrowInfiniteEnchantment::getMaxLevel() { return 1; }