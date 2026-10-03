#include "EditBox.h"

#include "minecraft/SharedConstants.h"
#include "minecraft/client/gui/Screen.h"
#include "platform/input/input.h"
#include "platform/stubs.h"

EditBox::EditBox(Screen* screen, Font* font, int x, int y, int width,
                 int height, const std::string& value) {
    // 4J - added initialisers
    maxLength = 0;
    frame = 0;
    inFocus = false;
    active = true;
    enableBackgroundDrawing =
        true;  // 4jcraft: for toggling the background rendering (from 1.6.4,
               // mainly for RepairScreen)

    this->screen = screen;
    this->font = font;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->setValue(value);
}

EditBox::~EditBox() {
    if (inFocus) {
        PlatformInput.StopTextInput();
    }
}

void EditBox::setValue(const std::string& value) { this->value = value; }

std::string EditBox::getValue() { return value; }

void EditBox::tick() {
    frame++;
    if (inFocus && active) {
        std::string typed = PlatformInput.PollTextInput();
        if (!typed.empty()) {
            for (char ch : typed) {
                if (ch == '\b' || ch == 8) {
                    if (value.length() > 0) value.pop_back();
                } else if (((unsigned char)ch >= 32 || (unsigned char)ch >= 160 || SharedConstants::isAllowedChatCharacter(ch)) &&
                           (value.length() < (size_t)maxLength || maxLength == 0)) {
                    value += ch;
                }
            }
        }
        if (PlatformInput.PollBackspacePressed() && value.length() > 0) {
            value.pop_back();
        }
    }
}

void EditBox::keyPressed(char ch, int eventKey) {
    if (!active || !inFocus) {
        return;
    }

    if (ch == 9) {
        screen->tabPressed();
        return;
    }

    if ((eventKey == Keyboard::KEY_BACK || ch == '\b' || ch == 8) && value.length() > 0) {
        value.pop_back();
        return;
    }
    if (((unsigned char)ch >= 32 || (unsigned char)ch >= 160 || SharedConstants::isAllowedChatCharacter(ch)) &&
        (value.length() < maxLength || maxLength == 0)) {
        value += ch;
    }
}

void EditBox::mouseClicked(int mouseX, int mouseY, int buttonNum) {
    bool newFocus = active && (mouseX >= x - 4 && mouseX < (x + width + 4) &&
                               mouseY >= y - 4 && mouseY < (y + height + 4));
    focus(newFocus);
}

void EditBox::focus(bool newFocus) {
    if (newFocus) {
        // reset the underscore counter to give quicker selection feedback
        frame = 0;
        PlatformInput.StartTextInput();
    } else if (inFocus) {
        PlatformInput.StopTextInput();
    }
    inFocus = newFocus;
}

void EditBox::render() {
    // 4jcraft: render the background conditionally
    if (enableBackgroundDrawing) {
        fill(x - 1, y - 1, x + width + 1, y + height + 1, 0xffa0a0a0);
        fill(x, y, x + width, y + height, 0xff000000);
    }

    // 4jcraft: offset conditionally
    int textX = x;
    int textY = y;
    if (enableBackgroundDrawing) {
        textX += 4;
        textY += (height - 8) / 2;
    }

    if (active) {
        bool renderUnderscore = inFocus && (frame / 6 % 2 == 0);
        drawString(font, value + (renderUnderscore ? "_" : ""), textX, textY,
                   (enableBackgroundDrawing ? 0xe0e0e0 : 0xffffff));
    } else {
        drawString(font, value, textX, textY,
                   (enableBackgroundDrawing ? 0xe0e0e0 : 0xffffff));
    }
}

void EditBox::setMaxLength(int maxLength) { this->maxLength = maxLength; }

int EditBox::getMaxLength() { return maxLength; }

// 4jcraft: for toggling the background rendering (from 1.6.4, mainly for
// RepairScreen)
void EditBox::setEnableBackgroundDrawing(bool enable) {
    enableBackgroundDrawing = enable;
}