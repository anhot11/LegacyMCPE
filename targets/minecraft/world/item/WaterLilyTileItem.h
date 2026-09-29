#pragma once

#include "ColoredTileItem.h"

class WaterLilyTileItem : public ColoredTileItem {
public:
    using ColoredTileItem::getColor;
    WaterLilyTileItem(int id);

    virtual std::shared_ptr<ItemInstance> use(
        std::shared_ptr<ItemInstance> itemInstance, Level* level,
        std::shared_ptr<Player> player);
    virtual bool TestUse(std::shared_ptr<ItemInstance> itemInstance,
                         Level* level, std::shared_ptr<Player> player);
    virtual int getColor(int data, int spriteLayer);
};
