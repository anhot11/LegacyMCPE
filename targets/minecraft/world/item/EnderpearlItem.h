#pragma once

#include "Item.h"

class EnderpearlItem : public Item {
public:
    EnderpearlItem(int id);

    virtual std::shared_ptr<ItemInstance> use(
        std::shared_ptr<ItemInstance> instance, Level* level,
        std::shared_ptr<Player> player);
    // 4J added
    virtual bool TestUse(std::shared_ptr<ItemInstance> instance, Level* level,
                         std::shared_ptr<Player> player);
};