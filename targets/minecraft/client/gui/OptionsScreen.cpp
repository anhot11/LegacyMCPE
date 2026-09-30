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
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/tile/Tile.h"

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
    int btnW = (width >= 450) ? 190 : 155;
    int btnH = 26;
    int rowSpacing = 30;
    int startY = height / 6 - 6;

    for (int i = 0; i < 5; i++) {
        const Options::Option* item = items[i];
        int xPos = width / 2 - (btnW + 5) + (position % 2 * (btnW + 10));
        int yPos = startY + rowSpacing * (position >> 1);
        Button* btn = nullptr;
        if (!item->isProgress()) {
            btn = new SmallButton(
                item->getId(), xPos, yPos, btnW, btnH, item,
                options->getMessage(item));
        } else {
            btn = new SlideButton(
                item->getId(), xPos, yPos, btnW, btnH, item,
                options->getMessage(item), options->getProgressValue(item));
        }

        // Set matching icons for options
        if (item == Options::Option::MUSIC) {
            btn->setTextureIcon(60, 48, 20);
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::jukebox)));
        } else if (item == Options::Option::SOUND) {
            btn->setTextureIcon(60, 48, 20);
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::noteblock)));
        } else if (item == Options::Option::INVERT_MOUSE) {
            btn->setTextureIcon(100, 48, 20);
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::bow)));
        } else if (item == Options::Option::SENSITIVITY) {
            btn->setTextureIcon(20, 48, 20);
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::compass)));
        } else if (item == Options::Option::DIFFICULTY) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::sword_iron)));
        }

        buttons.push_back(btn);
        position++;
    }

    int botW = (width >= 450) ? 230 : 190;
    Button* btnVideo = new Button(VIDEO_BUTTON_ID, width / 2 - botW / 2,
                                 startY + rowSpacing * 3 + 2, botW, btnH,
                                 language->getElement("options.video"));
    btnVideo->setTextureIcon(0, 48, 20);
    btnVideo->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::eyeOfEnder)));
    buttons.push_back(btnVideo);

    Button* btnControls = new Button(CONTROLS_BUTTON_ID, width / 2 - botW / 2,
                                 startY + rowSpacing * 4 + 2, botW, btnH,
                                 language->getElement("options.controls"));
    btnControls->setTextureIcon(100, 48, 20);
    btnControls->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::paper)));
    buttons.push_back(btnControls);

    Button* btnDone = new Button(200, width / 2 - botW / 2, startY + rowSpacing * 5 + 6,
                                 botW, btnH, language->getElement("gui.done"));
    btnDone->setTextureIcon(64, 32, 16);
    btnDone->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::door_wood)));
    buttons.push_back(btnDone);
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