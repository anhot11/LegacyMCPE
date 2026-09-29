#pragma once
#include <memory>

#include "AbstractContainerScreen.h"
class DispenserTileEntity;
class Inventory;

class TrapScreen : public AbstractContainerScreen {
public:
    TrapScreen(std::shared_ptr<Inventory> inventory,
               std::shared_ptr<DispenserTileEntity> trap);

protected:
    virtual void renderLabels();
    virtual void renderBg(float a);

private:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<DispenserTileEntity> trap;
};