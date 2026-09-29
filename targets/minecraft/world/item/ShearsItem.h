#pragma once

#include "Item.h"

class ShearsItem : public Item {
public:
    ShearsItem(int itemId);
    virtual bool mineBlock(std::shared_ptr<ItemInstance> itemInstance,
                           Level* level, int tile, int x, int y, int z,
                           std::shared_ptr<LivingEntity> owner);
    virtual bool canDestroySpecial(Tile* tile);
    virtual float getDestroySpeed(std::shared_ptr<ItemInstance> itemInstance,
                                  Tile* tile);
};