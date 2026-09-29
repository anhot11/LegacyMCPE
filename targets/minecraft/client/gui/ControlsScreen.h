#pragma once
#include <string>

#include "Screen.h"

class Options;

class ControlsScreen : public Screen {
private:
    Screen* lastScreen;

protected:
    std::string title;

private:
    Options* options;

    int selectedKey;

    static const int BUTTON_WIDTH = 70;
    static const int ROW_WIDTH = 160;

public:
    ControlsScreen(Screen* lastScreen, Options* options);

private:
    int getLeftScreenPosition();

public:
    void init() override;

protected:
    void buttonClicked(Button* button) override;
    void keyPressed(char eventCharacter, int eventKey) override;

public:
    void render(int xm, int ym, float a) override;
};