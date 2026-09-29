#include "EnchantmentInstance.h"

#include <vector>

#include "minecraft/util/WeighedRandom.h"
#include "minecraft/world/item/enchantment/Enchantment.h"

EnchantmentInstance::EnchantmentInstance(Enchantment* enchantment, int level)
    : WeighedRandomItem(enchantment->getFrequency()),
      enchantment(enchantment),
      level(level) {}

EnchantmentInstance::EnchantmentInstance(int id, int level)
    : WeighedRandomItem(Enchantment::enchantments[id]->getFrequency()),
      enchantment(Enchantment::enchantments[id]),
      level(level) {}

// 4J Added
EnchantmentInstance* EnchantmentInstance::copy() {
    return new EnchantmentInstance((Enchantment*)enchantment, (int)level);
}