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

    std::string locTitle = language ? language->getElement(title) : "";
    this->title = (locTitle.empty() || locTitle == title) ? title : locTitle;

    if (reasonObjects != nullptr && language) {
        this->reason = language->getElement(reason, reasonObjects);
    } else {
        std::string locReason = language ? language->getElement(reason) : "";
        this->reason = (locReason.empty() || locReason == reason) ? reason : locReason;
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

    size_t start = 0;
    size_t end = 0;
    int lineY = height / 2 - 10;
    while ((end = reason.find('\n', start)) != std::string::npos) {
        std::string line = reason.substr(start, end - start);
        drawCenteredString(font, line, width / 2, lineY, 0xffffff);
        lineY += 12;
        start = end + 1;
    }
    if (start < reason.size()) {
        std::string line = reason.substr(start);
        drawCenteredString(font, line, width / 2, lineY, 0xffffff);
    }

    Screen::render(xm, ym, a);
}
