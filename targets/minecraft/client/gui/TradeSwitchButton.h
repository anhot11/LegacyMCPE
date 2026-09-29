#pragma once
#include "Button.h"

class TradeSwitchButton : public Button {
private:
    bool mirrored;

public:
    TradeSwitchButton(int id, int x, int y, bool mirrored);

protected:
    int getYImage(bool hovered) override;
    void renderBg(Minecraft* minecraft, int xm, int ym) override;
};