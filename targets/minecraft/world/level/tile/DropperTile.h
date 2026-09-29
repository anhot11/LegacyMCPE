#pragma once

#include "DispenserTile.h"

class DispenseItemBehavior;

class DropperTile : public DispenserTile {
private:
    DispenseItemBehavior* DISPENSE_BEHAVIOUR;

public:
    DropperTile(int id);

    virtual void registerIcons(IconRegister* iconRegister);

protected:
    virtual DispenseItemBehavior* getDispenseMethod(
        std::shared_ptr<ItemInstance> item);

public:
    virtual std::shared_ptr<TileEntity> newTileEntity(Level* level);

protected:
    virtual void dispenseFrom(Level* level, int x, int y, int z);
};