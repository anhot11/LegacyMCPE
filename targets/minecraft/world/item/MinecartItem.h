#pragma once

#include "Item.h"
#include "minecraft/core/DefaultDispenseItemBehavior.h"

class BlockSource;

class MinecartItem : public Item {
private:
    class MinecartDispenseBehavior : public DefaultDispenseItemBehavior {
    private:
        DefaultDispenseItemBehavior defaultDispenseItemBehavior;

    public:
        virtual std::shared_ptr<ItemInstance> execute(
            BlockSource* source, std::shared_ptr<ItemInstance> dispensed,
            eOUTCOME& outcome);

    protected:
        virtual void playSound(BlockSource* source);
    };

public:
    int type;

    MinecartItem(int id, int type);

    virtual bool useOn(std::shared_ptr<ItemInstance> instance,
                       std::shared_ptr<Player> player, Level* level, int x,
                       int y, int z, int face, float clickX, float clickY,
                       float clickZ, bool bTestUseOnOnly = false);
};