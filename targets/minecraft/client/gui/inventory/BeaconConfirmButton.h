#pragma once
#include "AbstractBeaconButton.h"

class BeaconScreen;

class BeaconConfirmButton : public AbstractBeaconButton {
public:
    BeaconConfirmButton(BeaconScreen* screen, int id, int x, int y);
    void renderTooltip(int xm, int ym) override;

private:
    BeaconScreen* screen;
};