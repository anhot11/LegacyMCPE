#pragma once

#include "Item.h"

class Icon;

class BottleItem : public Item {
public:
    BottleItem(int id);

    //@Override
    Icon* getIcon(int auxValue);

    virtual std::shared_ptr<ItemInstance> use(
        std::shared_ptr<ItemInstance> itemInstance, Level* level,
        std::shared_ptr<Player> player);
    virtual bool TestUse(std::shared_ptr<ItemInstance> itemInstance,
                         Level* level, std::shared_ptr<Player> player);

    //@Override
    void registerIcons(IconRegister* iconRegister);
};