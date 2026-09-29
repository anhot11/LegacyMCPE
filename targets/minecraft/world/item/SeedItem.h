#pragma once

#include "Item.h"

class SeedItem : public Item {
private:
    int resultId;
    int targetLand;

public:
    SeedItem(int id, int resultId, int targetLand);

    virtual bool useOn(std::shared_ptr<ItemInstance> instance,
                       std::shared_ptr<Player> player, Level* level, int x,
                       int y, int z, int face, float clickX, float clickY,
                       float clickZ, bool bTestUseOnOnly = false);
};