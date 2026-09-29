#pragma once

#include <format>
#include <vector>

#include "Item.h"
#include "minecraft/world/entity/ai/attributes/Attribute.h"

class Mob;
class Tile;

class DiggerItem : public Item {
private:
    std::vector<Tile*>* tiles;

protected:
    float speed;

private:
    float attackDamage;

protected:
    const Tier* tier;

    DiggerItem(int id, float attackDamage, const Tier* tier,
               std::vector<Tile*>* tiles);

public:
    virtual float getDestroySpeed(std::shared_ptr<ItemInstance> itemInstance,
                                  Tile* tile);
    virtual bool hurtEnemy(std::shared_ptr<ItemInstance> itemInstance,
                           std::shared_ptr<LivingEntity> mob,
                           std::shared_ptr<LivingEntity> attacker);
    virtual bool mineBlock(std::shared_ptr<ItemInstance> itemInstance,
                           Level* level, int tile, int x, int y, int z,
                           std::shared_ptr<LivingEntity> owner);
    virtual bool isHandEquipped();
    virtual int getEnchantmentValue();

    const Tier* getTier();
    bool isValidRepairItem(std::shared_ptr<ItemInstance> source,
                           std::shared_ptr<ItemInstance> repairItem);
    virtual attrAttrModMap* getDefaultAttributeModifiers();
};