#include "SlideButton.h"

#include "minecraft/client/Minecraft.h"
#include "minecraft/client/Options.h"
#include "minecraft/client/gui/Button.h"
#include "platform/renderer/renderer.h"
#include "platform/stubs.h"

SlideButton::SlideButton(int id, int x, int y, const Options::Option* option,
                         const std::string& msg, float value)
    : Button(id, x, y, 150, 20, msg) {
    this->sliding = false;  // 4J added
    this->option = option;
    this->value = value;
}

SlideButton::SlideButton(int id, int x, int y, int width, int height,
                         const Options::Option* option, const std::string& msg,
                         float value)
    : Button(id, x, y, width, height, msg) {
    this->sliding = false;
    this->option = option;
    this->value = value;
}

int SlideButton::getYImage(bool hovered) { return 0; }

void SlideButton::renderBg(Minecraft* minecraft, int xm, int ym) {
    if (!visible) return;
    if (sliding) {
        value = (xm - (x + 4)) / (float)(w - 8);
        if (value < 0) value = 0;
        if (value > 1) value = 1;
        minecraft->options->set(option, value);
        msg = minecraft->options->getMessage(option);
    }
    glColor4f(1, 1, 1, 1);
    int v0 = 46 + 1 * 20;
    int thumbX = x + (int)(value * (w - 8));
    if (h == 20) {
        blit(thumbX, y, 0, v0, 4, h);
        blit(thumbX + 4, y, 196, v0, 4, h);
    } else {
        int hTop = h / 2;
        int hBot = h - hTop;
        blit(thumbX, y, 0, v0, 4, hTop, 4, 10);
        blit(thumbX + 4, y, 196, v0, 4, hTop, 4, 10);
        blit(thumbX, y + hTop, 0, v0 + 10, 4, hBot, 4, 10);
        blit(thumbX + 4, y + hTop, 196, v0 + 10, 4, hBot, 4, 10);
    }
}

bool SlideButton::clicked(Minecraft* minecraft, int mx, int my) {
    if (Button::clicked(minecraft, mx, my)) {
        value = (mx - (x + 4)) / (float)(w - 8);
        if (value < 0) value = 0;
        if (value > 1) value = 1;
        minecraft->options->set(option, value);
        msg = minecraft->options->getMessage(option);
        sliding = true;
        return true;
    }

    return false;
}

void SlideButton::released(int mx, int my) { sliding = false; }