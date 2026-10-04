#include "RenameWorldScreen.h"

#include <vector>

#include "Button.h"
#include "EditBox.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/gui/Screen.h"
#include "minecraft/locale/Language.h"
#include "minecraft/world/level/storage/LevelStorageSource.h"
#include "platform/stubs.h"
#include "util/StringHelpers.h"

RenameWorldScreen::RenameWorldScreen(Screen* lastScreen,
                                     const std::string& levelId) {
    nameEdit = nullptr;
    this->lastScreen = lastScreen;
    this->levelId = levelId;
}

RenameWorldScreen::~RenameWorldScreen() {
    if (nameEdit != nullptr) {
        delete nameEdit;
        nameEdit = nullptr;
    }
}

void RenameWorldScreen::tick() {
    if (nameEdit != nullptr) {
        nameEdit->tick();
    }
}

void RenameWorldScreen::init() {
    Language* language = Language::getInstance();
    Keyboard::enableRepeatEvents(true);
    buttons.clear();

    buttons.push_back(new Button(0, width / 2 - 100, height / 4 + 96 + 12,
                                 language ? language->getElement("selectWorld.renameButton") : "Renombrar"));
    buttons.push_back(new Button(1, width / 2 - 100, height / 4 + 120 + 12,
                                 language ? language->getElement("gui.cancel") : "Cancelar"));

    if (nameEdit != nullptr) {
        delete nameEdit;
        nameEdit = nullptr;
    }
    nameEdit = new EditBox(this, font, width / 2 - 100, 60, 200, 20, levelId);
    nameEdit->focus(true);
}

void RenameWorldScreen::removed() {
    Keyboard::enableRepeatEvents(false);
    if (nameEdit != nullptr) {
        nameEdit->focus(false);
    }
    PlatformInput.StopTextInput();
}

void RenameWorldScreen::buttonClicked(Button* button) {
    if (!button->active) return;
    if (button->id == 1) {
        minecraft->setScreen(lastScreen);
    } else if (button->id == 0) {
        if (nameEdit != nullptr) {
            LevelStorageSource* levelSource = minecraft ? minecraft->getLevelSource() : nullptr;
            if (levelSource != nullptr) {
                levelSource->renameLevel(levelId, trimString(nameEdit->getValue()));
            }
        }
        minecraft->setScreen(lastScreen);
    }
}

void RenameWorldScreen::keyPressed(char ch, int eventKey) {
    if (nameEdit != nullptr) {
        nameEdit->keyPressed(ch, eventKey);
        if (!buttons.empty() && buttons[0] != nullptr) {
            buttons[0]->active = trimString(nameEdit->getValue()).length() > 0;
        }
    }

    if (ch == 13 && !buttons.empty() && buttons[0] != nullptr && buttons[0]->active) {
        buttonClicked(buttons[0]);
    }
}

void RenameWorldScreen::mouseClicked(int x, int y, int buttonNum) {
    Screen::mouseClicked(x, y, buttonNum);
    if (nameEdit != nullptr) {
        nameEdit->mouseClicked(x, y, buttonNum);
    }
}

void RenameWorldScreen::render(int xm, int ym, float a) {
    Language* language = Language::getInstance();

    renderBackground();

    drawCenteredString(font, language ? language->getElement("selectWorld.renameTitle") : "Renombrar mundo",
                       width / 2, height / 4 - 60 + 20, 0xffffff);
    drawString(font, language ? language->getElement("selectWorld.enterName") : "Ingresa el nuevo nombre:",
               width / 2 - 100, 47, 0xa0a0a0);

    if (nameEdit != nullptr) {
        nameEdit->render();
    }

    Screen::render(xm, ym, a);
}