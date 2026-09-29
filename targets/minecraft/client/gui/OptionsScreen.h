#pragma once
#include <string>

#include "Screen.h"

class Options;

class OptionsScreen : public Screen {
private:
    static const int CONTROLS_BUTTON_ID = 100;
    static const int VIDEO_BUTTON_ID = 101;
    Screen* lastScreen;

protected:
    std::string title;

private:
    Options* options;

public:
    OptionsScreen(Screen* lastScreen, Options* options);
    virtual void init() override;

protected:
    virtual void buttonClicked(Button* button) override;

public:
    virtual void render(int xm, int ym, float a) override;
};