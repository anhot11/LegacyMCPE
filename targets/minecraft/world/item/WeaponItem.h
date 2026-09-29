#pragma once

#include "Item.h"
#include "minecraft/world/entity/ai/attributes/Attribute.h"
#include "minecraft/world/item/UseAnim.h"

class WeaponItem : public Item {
private:
    float damage;
    const Tier* tier;

public:
    WeaponItem(int id, const Tier* tier);
    virtual float getTierDamage();
    virtual float getDestroySpeed(std::shared_ptr<ItemInstance> itemInstance,
                                  Tile* tile);
    virtual bool hurtEnemy(std::shared_ptr<ItemInstance> itemInstance,
                           std::shared_ptr<LivingEntity> mob,
                           std::shared_ptr<LivingEntity> attacker);
    virtual bool mineBlock(std::shared_ptr<ItemInstance> itemInstance,
                           Level* level, int tile, int x, int y, int z,
                           std::shared_ptr<LivingEntity> owner);
    virtual bool isHandEquipped();
    virtual UseAnim getUseAnimation(std::shared_ptr<ItemInstance> itemInstance);
    virtual int getUseDuration(std::shared_ptr<ItemInstance> itemInstance);
    virtual std::shared_ptr<ItemInstance> use(
        std::shared_ptr<ItemInstance> instance, Level* level,
        std::shared_ptr<Player> player);
    virtual bool canDestroySpecial(Tile* tile);
    virtual int getEnchantmentValue();

    const Tier* getTier();
    bool isValidRepairItem(std::shared_ptr<ItemInstance> source,
                           std::shared_ptr<ItemInstance> repairItem);
    attrAttrModMap* getDefaultAttributeModifiers();
};