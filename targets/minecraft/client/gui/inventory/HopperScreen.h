#pragma once
#include <memory>

#include "minecraft/client/gui/inventory/AbstractContainerScreen.h"

class HopperTileEntity;
class MinecartHopper;
class Inventory;
class Container;

class HopperScreen : public AbstractContainerScreen {
public:
    HopperScreen(std::shared_ptr<Inventory> inventory,
                 std::shared_ptr<Container> hopper);

protected:
    virtual void renderLabels() override;
    virtual void renderBg(float a) override;

private:
    std::shared_ptr<Inventory> inventory;
    std::shared_ptr<Container> hopper;
};