#include "DisconnectedScreen.h"

#include <vector>

#include "minecraft/client/Minecraft.h"
#include "minecraft/client/gui/Button.h"
#include "minecraft/client/gui/Screen.h"
#include "minecraft/client/title/TitleScreen.h"
#include "minecraft/locale/Language.h"

DisconnectedScreen::DisconnectedScreen(const std::string& title,
                                       const std::string reason,
                                       void* reasonObjects, ...) {
    Language* language = Language::getInstance();

    this->title = language->getElement(title);
    if (reasonObjects != nullptr) {
        this->reason = language->getElement(reason, reasonObjects);
    } else {
        this->reason = language->getElement(reason);
    }
}

void DisconnectedScreen::tick() {}

void DisconnectedScreen::keyPressed(char eventCharacter, int eventKey) {}

void DisconnectedScreen::init() {
    Language* language = Language::getInstance();

    buttons.clear();
    buttons.push_back(new Button(0, width / 2 - 100, height / 4 + 24 * 5 + 12,
                                 language->getElement("gui.toMenu")));
}

void DisconnectedScreen::buttonClicked(Button* button) {
    if (button->id == 0) {
        minecraft->setScreen(new TitleScreen());
    }
}

void DisconnectedScreen::render(int xm, int ym, float a) {
    renderBackground();

    drawCenteredString(font, title, width / 2, height / 2 - 50, 0xffffff);
    drawCenteredString(font, reason, width / 2, height / 2 - 10, 0xffffff);

    Screen::render(xm, ym, a);
}
