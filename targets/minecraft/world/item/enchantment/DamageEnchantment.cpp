#include "DamageEnchantment.h"

#include <memory>

#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/MobType.h"
#include "minecraft/world/item/HatchetItem.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/enchantment/Enchantment.h"
#include "minecraft/world/item/enchantment/EnchantmentCategory.h"
#include "strings.h"

const int DamageEnchantment::names[] = {IDS_ENCHANTMENT_DAMAGE_ALL,
                                        IDS_ENCHANTMENT_DAMAGE_UNDEAD,
                                        IDS_ENCHANTMENT_DAMAGE_ARTHROPODS};
const int DamageEnchantment::minCost[] = {1, 5, 5};
const int DamageEnchantment::levelCost[] = {11, 8, 8};
const int DamageEnchantment::levelCostSpan[] = {20, 20, 20};

DamageEnchantment::DamageEnchantment(int id, int frequency, int type)
    : Enchantment(id, frequency, EnchantmentCategory::weapon), type(type) {}

int DamageEnchantment::getMinCost(int level) {
    return minCost[type] + (level - 1) * levelCost[type];
}

int DamageEnchantment::getMaxCost(int level) {
    return getMinCost(level) + levelCostSpan[type];
}

int DamageEnchantment::getMaxLevel() { return 5; }

float DamageEnchantment::getDamageBonus(int level,
                                        std::shared_ptr<LivingEntity> target) {
    if (type == ALL) {
        return level * 1.25f;
    }
    if (type == UNDEAD && target->getMobType() == UNDEAD) {
        return level * 2.5f;
    }
    if (type == ARTHROPODS && target->getMobType() == ARTHROPOD) {
        return level * 2.5f;
    }
    return 0;
}

int DamageEnchantment::getDescriptionId() { return names[type]; }

bool DamageEnchantment::isCompatibleWith(Enchantment* other) const {
    return dynamic_cast<DamageEnchantment*>(other) == nullptr;
}

bool DamageEnchantment::canEnchant(std::shared_ptr<ItemInstance> item) {
    HatchetItem* hatchet = dynamic_cast<HatchetItem*>(item->getItem());
    if (hatchet) return true;
    return Enchantment::canEnchant(item);
}