#pragma once

#include "Item.h"

class NameTagItem : public Item {
public:
    NameTagItem(int id);

    bool interactEnemy(std::shared_ptr<ItemInstance> itemInstance,
                       std::shared_ptr<Player> player,
                       std::shared_ptr<LivingEntity> target);
};