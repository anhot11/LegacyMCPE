#include "UntouchingEnchantment.h"

#include <memory>

#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/enchantment/Enchantment.h"
#include "minecraft/world/item/enchantment/EnchantmentCategory.h"
#include "strings.h"

UntouchingEnchantment::UntouchingEnchantment(int id, int frequency)
    : Enchantment(id, frequency, EnchantmentCategory::digger) {
    setDescriptionId(IDS_ENCHANTMENT_UNTOUCHING);
}

int UntouchingEnchantment::getMinCost(int level) { return 15; }

int UntouchingEnchantment::getMaxCost(int level) {
    return Enchantment::getMinCost(level) + 50;
}

int UntouchingEnchantment::getMaxLevel() { return 1; }

bool UntouchingEnchantment::isCompatibleWith(Enchantment* other) const {
    return Enchantment::isCompatibleWith(other) &&
           other->id != resourceBonus->id;
}

bool UntouchingEnchantment::canEnchant(std::shared_ptr<ItemInstance> item) {
    if (item->getItem()->id == Item::shears_Id) return true;
    return Enchantment::canEnchant(item);
}