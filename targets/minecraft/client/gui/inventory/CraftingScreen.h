#pragma once
#include <memory>

#include "AbstractContainerScreen.h"

class Inventory;
class Level;

class CraftingScreen : public AbstractContainerScreen {
private:
    std::shared_ptr<Inventory> inventory;

public:
    CraftingScreen(std::shared_ptr<Inventory> inventory, Level* level, int x,
                   int y, int z);
    virtual void removed() override;

protected:
    virtual void renderLabels() override;
    virtual void renderBg(float a) override;
};