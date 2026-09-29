#pragma once
#include "Screen.h"

class Minecraft;

class PauseScreen : public Screen {
private:
    int saveStep;
    int visibleTime;

public:
    PauseScreen();  // 4J added
    virtual void init() override;
    static void exitWorld(Minecraft* minecraft, bool save);

protected:
    using Screen::buttonClicked;

    virtual void buttonClicked(Button* button) override;

public:
    virtual void tick() override;
    virtual void render(int xm, int ym, float a) override;
};
