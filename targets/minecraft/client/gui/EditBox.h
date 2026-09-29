#pragma once
#include <string>

#include "GuiComponent.h"

class Font;
class Screen;

class EditBox : public GuiComponent {
private:
    Font* font;
    int x;
    int y;
    int width;
    int height;
    std::string value;
    unsigned int maxLength;
    int frame;

public:
    bool inFocus;
    bool active;
    bool enableBackgroundDrawing;  // 4jcraft: for toggling the background
                                   // rendering (mainly for RepairScreen)

private:
    Screen* screen;

public:
    EditBox(Screen* screen, Font* font, int x, int y, int width, int height,
            const std::string& value);
    void setValue(const std::string& value);
    std::string getValue();
    void tick();
    void keyPressed(char ch, int eventKey);
    void mouseClicked(int mouseX, int mouseY, int buttonNum);
    void focus(bool newFocus);
    void render();
    void setMaxLength(int maxLength);
    int getMaxLength();

    // 4jcraft: for toggling the background rendering (mainly for RepairScreen)
    void setEnableBackgroundDrawing(bool enable);
};