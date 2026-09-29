#include "VideoSettingsScreen.h"

#include <vector>

#include "SlideButton.h"
#include "SmallButton.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/Options.h"
#include "minecraft/client/gui/Button.h"
#include "minecraft/client/gui/Screen.h"
#include "minecraft/locale/Language.h"

// 4jcraft
#define ITEM_COUNT 10

VideoSettingsScreen::VideoSettingsScreen(Screen* lastScreen, Options* options) {
    this->title = "Video Settings";  // 4J - added
    this->lastScreen = lastScreen;
    this->options = options;
}

void VideoSettingsScreen::init() {
    Language* language = Language::getInstance();
    this->title = language->getElement("options.videoTitle");

    const Options::Option* items[ITEM_COUNT] = {
        Options::Option::GRAPHICS,
        Options::Option::RENDER_DISTANCE,
        Options::Option::AMBIENT_OCCLUSION,
        Options::Option::FRAMERATE_LIMIT,
        Options::Option::ANAGLYPH,
        Options::Option::VIEW_BOBBING,
        Options::Option::GUI_SCALE,
        Options::Option::ADVANCED_OPENGL,
        Options::Option::GAMMA,
        Options::Option::FOV};

    for (int i = 0; i < ITEM_COUNT; i++) {
        const Options::Option* item = items[i];
        int xPos = width / 2 - 155 + (i % 2 * 160);
        int yPos = height / 6 + 24 * (i / 2);

        if (!item->isProgress()) {
            buttons.push_back(new SmallButton(item->getId(), xPos, yPos, item,
                                              options->getMessage(item)));
        } else {
            buttons.push_back(new SlideButton(item->getId(), xPos, yPos, item,
                                              options->getMessage(item),
                                              options->getProgressValue(item)));
        }
    }

    //        buttons.add(new Button(VIDEO_BUTTON_ID, width / 2 - 100, height /
    //        6 + 24 * 4 + 12, language.getElement("options.video")));
    //        buttons.add(new Button(CONTROLS_BUTTON_ID, width / 2 - 100, height
    //        / 6 + 24 * 5 + 12, language.getElement("options.controls")));
    buttons.push_back(new Button(200, width / 2 - 100, height / 6 + 24 * 6,
                                 language->getElement("gui.done")));
}

void VideoSettingsScreen::buttonClicked(Button* button) {
    if (!button->active) return;
    if (button->id < 100 && (dynamic_cast<SmallButton*>(button) != nullptr)) {
        options->toggle(((SmallButton*)button)->getOption(), 1);
        button->msg = options->getMessage(Options::Option::getItem(button->id));
        return;
    }
    if (button->id == 200) {
        minecraft->options->save();
        minecraft->setScreen(lastScreen);
        return;
    }
}

void VideoSettingsScreen::render(int xm, int ym, float a) {
    renderBackground();
    drawCenteredString(font, title, width / 2, 20, 0xffffff);

    Screen::render(xm, ym, a);
}