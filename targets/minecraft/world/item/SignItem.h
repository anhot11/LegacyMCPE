#pragma once

#include "Item.h"

class SignItem : public Item {
public:
    SignItem(int id);

    virtual bool useOn(std::shared_ptr<ItemInstance> instance,
                       std::shared_ptr<Player> player, Level* level, int x,
                       int y, int z, int face, float clickX, float clickY,
                       float clickZ, bool bTestUseOnOnly = false);
};