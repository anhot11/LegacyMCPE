#include "DiggerItem.h"

#include <memory>
#include <vector>

#include "minecraft/world/entity/ai/attributes/Attribute.h"
#include "minecraft/world/entity/ai/attributes/AttributeModifier.h"
#include "minecraft/world/entity/monster/SharedMonsterAttributes.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/tile/Tile.h"

DiggerItem::DiggerItem(int id, float attackDamage, const Tier* tier,
                       std::vector<Tile*>* tiles)
    : Item(id), tier(tier) {
    // this->tier = tier;
    this->tiles = tiles;
    maxStackSize = 1;
    setMaxDamage(tier->getUses());
    this->speed = tier->getSpeed();
    this->attackDamage = attackDamage + tier->getAttackDamageBonus();
}

float DiggerItem::getDestroySpeed(std::shared_ptr<ItemInstance> itemInstance,
                                  Tile* tile) {
    for (unsigned int i = 0; i < tiles->size(); i++)
        if ((*tiles)[i] == tile) return speed;
    return 1;
}

bool DiggerItem::hurtEnemy(std::shared_ptr<ItemInstance> itemInstance,
                           std::shared_ptr<LivingEntity> mob,
                           std::shared_ptr<LivingEntity> attacker) {
    itemInstance->hurtAndBreak(2, attacker);
    return true;
}

bool DiggerItem::mineBlock(std::shared_ptr<ItemInstance> itemInstance,
                           Level* level, int tile, int x, int y, int z,
                           std::shared_ptr<LivingEntity> owner) {
    // Don't damage tools if the tile can be destroyed in one hit.
    if (Tile::tiles[tile]->getDestroySpeed(level, x, y, z) != 0.0)
        itemInstance->hurtAndBreak(1, owner);
    return true;
}

bool DiggerItem::isHandEquipped() { return true; }

int DiggerItem::getEnchantmentValue() { return tier->getEnchantmentValue(); }

const Item::Tier* DiggerItem::getTier() { return tier; }

bool DiggerItem::isValidRepairItem(std::shared_ptr<ItemInstance> source,
                                   std::shared_ptr<ItemInstance> repairItem) {
    if (tier->getTierItemId() == repairItem->id) {
        return true;
    }
    return Item::isValidRepairItem(source, repairItem);
}

attrAttrModMap* DiggerItem::getDefaultAttributeModifiers() {
    attrAttrModMap* result = Item::getDefaultAttributeModifiers();

    (*result)[SharedMonsterAttributes::ATTACK_DAMAGE->getId()] =
        new AttributeModifier(eModifierId_ITEM_BASEDAMAGE, attackDamage,
                              AttributeModifier::OPERATION_ADDITION);

    return result;
}