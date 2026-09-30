#include "OptionsScreen.h"

#include <vector>

#include "ControlsScreen.h"
#include "SlideButton.h"
#include "SmallButton.h"
#include "VideoSettingsScreen.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/Options.h"
#include "minecraft/client/gui/Button.h"
#include "minecraft/client/gui/Screen.h"
#include "minecraft/locale/Language.h"

OptionsScreen::OptionsScreen(Screen* lastScreen, Options* options) {
    title = "Options";  // 4J added

    this->lastScreen = lastScreen;
    this->options = options;
}

void OptionsScreen::init() {
    Language* language = Language::getInstance();
    this->title = language->getElement("options.title");

    int position = 0;

    // 4J - this was as static array but moving it into the function to remove
    // any issues with static initialisation order
    const Options::Option* items[5] = {
        Options::Option::MUSIC, Options::Option::SOUND,
        Options::Option::INVERT_MOUSE, Options::Option::SENSITIVITY,
        Options::Option::DIFFICULTY};
    int btnW = 150;
    int btnH = 20;
    int rowSpacing = 24;
    int startY = height / 6 - 8;

    for (int i = 0; i < 5; i++) {
        const Options::Option* item = items[i];
        int xPos = width / 2 - 155 + (position % 2 * 160);
        int yPos = startY + rowSpacing * (position >> 1);
        if (!item->isProgress()) {
            buttons.push_back(new SmallButton(
                item->getId(), xPos, yPos, btnW, btnH, item,
                options->getMessage(item)));
        } else {
            buttons.push_back(new SlideButton(
                item->getId(), xPos, yPos, btnW, btnH, item,
                options->getMessage(item), options->getProgressValue(item)));
        }
        position++;
    }

    buttons.push_back(new Button(VIDEO_BUTTON_ID, width / 2 - 100,
                                 startY + rowSpacing * 3 + 4, 200, btnH,
                                 language->getElement("options.video")));
    buttons.push_back(new Button(CONTROLS_BUTTON_ID, width / 2 - 100,
                                 startY + rowSpacing * 4 + 4, 200, btnH,
                                 language->getElement("options.controls")));
    buttons.push_back(new Button(200, width / 2 - 100, startY + rowSpacing * 5 + 10,
                                 200, btnH, language->getElement("gui.done")));
}

void OptionsScreen::buttonClicked(Button* button) {
    if (!button->active) return;
    if (button->id < 100 && (dynamic_cast<SmallButton*>(button) != nullptr)) {
        options->toggle(((SmallButton*)button)->getOption(), 1);
        button->msg = options->getMessage(Options::Option::getItem(button->id));
    }
    if (button->id == VIDEO_BUTTON_ID) {
        minecraft->options->save();
        minecraft->setScreen(new VideoSettingsScreen(this, options));
    }
    if (button->id == CONTROLS_BUTTON_ID) {
        minecraft->options->save();
        minecraft->setScreen(new ControlsScreen(this, options));
    }
    if (button->id == 200) {
        minecraft->options->save();
        minecraft->setScreen(lastScreen);
    }
}

void OptionsScreen::render(int xm, int ym, float a) {
    renderBackground();
    drawCenteredString(font, title, width / 2, 20, 0xffffff);
    Screen::render(xm, ym, a);
}