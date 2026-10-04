#include "DeathScreen.h"

#include <memory>
#include <string>
#include <vector>

#include "Button.h"
#include "PauseScreen.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/gui/Screen.h"
#include "minecraft/client/multiplayer/MultiPlayerLocalPlayer.h"
#include "platform/renderer/renderer.h"
#include "platform/stubs.h"
#include "util/StringHelpers.h"

void DeathScreen::init() {
    buttons.clear();
    int btnW = 200;
    int btnH = 24;
    int btnX = width / 2 - btnW / 2;
    int startY = height / 4 + 68;
    buttons.push_back(new Button(1, btnX, startY, btnW, btnH, "Respawn"));
    buttons.push_back(
        new Button(2, btnX, startY + 30, btnW, btnH, "Title menu"));
}

void DeathScreen::keyPressed(char eventCharacter, int eventKey) {}

void DeathScreen::buttonClicked(Button* button) {
    if (!button || !button->active) return;
    if (button->id == 1) {
        button->active = false;
        if (minecraft->player != nullptr) {
            minecraft->player->respawn();
            if (minecraft->player->getHealth() > 0) {
                minecraft->setScreen(nullptr);
            }
        }
    }
    if (button->id == 2) {
        button->active = false;
        PauseScreen::exitWorld(minecraft, false);
    }
}

void DeathScreen::render(int xm, int ym, float a) {
    // If the player has already respawned with health > 0, close the death
    // screen immediately!
    if (minecraft->player != nullptr && minecraft->player->getHealth() > 0) {
        minecraft->setScreen(nullptr);
        return;
    }

    fillGradient(0, 0, width, height, 0x60500000, 0xa0803030);

    glPushMatrix();
    glScalef(2, 2, 2);
    drawCenteredString(font, "Game over!", width / 2 / 2, 60 / 2, 0xffffff);
    glPopMatrix();
    if (minecraft->player != nullptr) {
        drawCenteredString(
            font, "Score: &e" + toWString(minecraft->player->getScore()),
            width / 2, 100, 0xffffff);
    }

    Screen::render(xm, ym, a);
}

bool DeathScreen::isPauseScreen() { return false; }