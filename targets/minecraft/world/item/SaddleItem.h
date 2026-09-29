#pragma once

#include "Item.h"

class SaddleItem : public Item {
public:
    SaddleItem(int id);

    virtual bool interactEnemy(std::shared_ptr<ItemInstance> itemInstance,
                               std::shared_ptr<Player> player,
                               std::shared_ptr<LivingEntity> mob);
    virtual bool hurtEnemy(std::shared_ptr<ItemInstance> itemInstance,
                           std::shared_ptr<LivingEntity> mob,
                           std::shared_ptr<LivingEntity> attacker);
};