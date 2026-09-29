#pragma once
#include <string>

#include "Screen.h"

class EditBox;
class Button;

class JoinMultiplayerScreen : public Screen {
private:
    Screen* lastScreen;
    EditBox* ipEdit;

public:
    JoinMultiplayerScreen(Screen* lastScreen);
    virtual void tick() override;
    virtual void init() override;
    virtual void removed() override;

protected:
    virtual void buttonClicked(Button* button) override;

private:
    virtual int parseInt(const std::string& str, int def);

protected:
    virtual void keyPressed(char ch, int eventKey) override;
    virtual void mouseClicked(int x, int y, int buttonNum) override;

public:
    virtual void render(int xm, int ym, float a) override;
};