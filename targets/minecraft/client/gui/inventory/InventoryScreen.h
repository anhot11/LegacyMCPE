#pragma once
#include <memory>

#include "AbstractContainerScreen.h"

class Player;
class Button;

class InventoryScreen : public AbstractContainerScreen {
public:
    InventoryScreen(std::shared_ptr<Player> player);
    virtual void init() override;

protected:
    virtual void renderLabels() override;

private:
    float xMouse, yMouse;

public:
    virtual void render(int xm, int ym, float a) override;

protected:
    virtual void renderBg(float a) override;
    virtual void buttonClicked(Button* button) override;
};