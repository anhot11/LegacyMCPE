#pragma once

#include "Item.h"

class Player;
class Level;

class BucketItem : public Item {
private:
    int content;

public:
    BucketItem(int id, int content);

    virtual bool TestUse(std::shared_ptr<ItemInstance> itemInstance,
                         Level* level, std::shared_ptr<Player> player);
    virtual std::shared_ptr<ItemInstance> use(
        std::shared_ptr<ItemInstance> itemInstance, Level* level,
        std::shared_ptr<Player> player);

    bool emptyBucket(Level* level, int xt, int yt, int zt);
};