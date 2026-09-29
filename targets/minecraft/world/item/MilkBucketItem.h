#pragma once

#include "Item.h"
#include "minecraft/world/item/UseAnim.h"

class MilkBucketItem : public Item {
private:
    static const int DRINK_DURATION = (int)(20 * 1.6);

public:
    MilkBucketItem(int id);

    virtual std::shared_ptr<ItemInstance> useTimeDepleted(
        std::shared_ptr<ItemInstance> instance, Level* level,
        std::shared_ptr<Player> player);
    virtual int getUseDuration(std::shared_ptr<ItemInstance> itemInstance);
    virtual UseAnim getUseAnimation(std::shared_ptr<ItemInstance> itemInstance);
    virtual std::shared_ptr<ItemInstance> use(
        std::shared_ptr<ItemInstance> instance, Level* level,
        std::shared_ptr<Player> player);
};