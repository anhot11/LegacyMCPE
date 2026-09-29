#pragma once
#include "AbstractBeaconButton.h"

class BeaconScreen;

class BeaconCancelButton : public AbstractBeaconButton {
public:
    BeaconCancelButton(BeaconScreen* screen, int id, int x, int y);
    void renderTooltip(int xm, int ym) override;

private:
    BeaconScreen* screen;
};