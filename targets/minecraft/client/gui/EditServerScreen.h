#pragma once
#include <string>
#include "Screen.h"

class Button;
class EditBox;
class SelectWorldScreen;

class EditServerScreen : public Screen {
private:
    SelectWorldScreen* lastScreen;
    int editIndex; // -1 if adding new server
    std::string initialName;
    std::string initialIp;

    EditBox* nameEdit;
    EditBox* ipEdit;
    Button* saveButton;
    Button* cancelButton;

public:
    EditServerScreen(SelectWorldScreen* lastScreen, int editIndex, const std::string& name = "", const std::string& ip = "");
    virtual ~EditServerScreen();

    virtual void init() override;
    virtual void tick() override;
    virtual void removed() override;

protected:
    virtual void buttonClicked(Button* button) override;
    virtual void keyPressed(char ch, int eventKey) override;
    virtual void mouseClicked(int x, int y, int buttonNum) override;

public:
    virtual void render(int xm, int ym, float a) override;
};
