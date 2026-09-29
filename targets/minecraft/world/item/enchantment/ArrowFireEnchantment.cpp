#include "ArrowFireEnchantment.h"

#include "minecraft/world/item/enchantment/Enchantment.h"
#include "minecraft/world/item/enchantment/EnchantmentCategory.h"
#include "strings.h"

ArrowFireEnchantment::ArrowFireEnchantment(int id, int frequency)
    : Enchantment(id, frequency, EnchantmentCategory::bow) {
    setDescriptionId(IDS_ENCHANTMENT_ARROW_FIRE);
}

int ArrowFireEnchantment::getMinCost(int level) { return 20; }

int ArrowFireEnchantment::getMaxCost(int level) { return 50; }

int ArrowFireEnchantment::getMaxLevel() { return 1; }