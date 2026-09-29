#pragma once
#include <string>

#include "Screen.h"

class Button;

class NameEntryScreen : public Screen {
private:
    Screen* lastScreen;

protected:
    std::string title;

private:
    int slot;
    std::string name;
    int frame;

public:
    NameEntryScreen(Screen* lastScreen, const std::string& oldName, int slot);
    virtual void init() override;
    virtual void removed() override;
    virtual void tick() override;

protected:
    virtual void buttonClicked(Button button);

private:
    static const std::string allowedChars;

protected:
    virtual void keyPressed(char ch, int eventKey) override;

public:
    virtual void render(int xm, int ym, float a) override;
};