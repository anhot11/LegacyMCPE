#pragma once
#include <string>

#include "Screen.h"

class Button;
class EditBox;

class RenameWorldScreen : public Screen {
private:
    Screen* lastScreen;
    EditBox* nameEdit;
    std::string levelId;

public:
    RenameWorldScreen(Screen* lastScreen, const std::string& levelId);
    virtual void tick() override;
    virtual void init() override;
    virtual void removed() override;

protected:
    virtual void buttonClicked(Button* button) override;
    virtual void keyPressed(char ch, int eventKey) override;
    virtual void mouseClicked(int x, int y, int buttonNum) override;

public:
    virtual void render(int xm, int ym, float a) override;
};