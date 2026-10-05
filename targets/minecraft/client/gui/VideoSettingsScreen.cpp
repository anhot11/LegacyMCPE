#include "VideoSettingsScreen.h"

extern bool g_mcplXboxPreset;
#include <vector>

#include "SlideButton.h"
#include "SmallButton.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/Options.h"
#include "minecraft/client/gui/Button.h"
#include "minecraft/client/gui/Screen.h"
#include "minecraft/client/renderer/LevelRenderer.h"
#include "minecraft/locale/Language.h"
#include "minecraft/world/item/ArmorItem.h"
#include "minecraft/world/item/BowItem.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/tile/Tile.h"

// 4jcraft
#define ITEM_COUNT 10

static Button* s_xboxButton = nullptr;

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
        Options::Option::RENDER_CLOUDS,
        Options::Option::VIEW_BOBBING,
        Options::Option::GUI_SCALE,
        Options::Option::ADVANCED_OPENGL,
        Options::Option::GAMMA,
        Options::Option::FOV};

    int btnW = (width >= 450) ? 190 : 155;
    int btnH = 24;
    int rowSpacing = 27;
    int startY = height / 6 - 10;

    for (int i = 0; i < ITEM_COUNT; i++) {
        const Options::Option* item = items[i];
        int xPos = width / 2 - (btnW + 5) + (i % 2 * (btnW + 10));
        int yPos = startY + rowSpacing * (i / 2);

        Button* btn = nullptr;
        if (!item->isProgress()) {
            btn = new SmallButton(item->getId(), xPos, yPos, btnW, btnH, item,
                                  options->getMessage(item));
        } else {
            btn = new SlideButton(item->getId(), xPos, yPos, btnW, btnH, item,
                                  options->getMessage(item),
                                  options->getProgressValue(item));
        }

        // Distinctive Minecraft icons for each video setting
        if (item == Options::Option::GRAPHICS) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(
                new ItemInstance((Tile*)Tile::leaves)));
        } else if (item == Options::Option::RENDER_DISTANCE) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(
                new ItemInstance(Item::eyeOfEnder)));
        } else if (item == Options::Option::AMBIENT_OCCLUSION) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(
                new ItemInstance((Tile*)Tile::glowstone)));
        } else if (item == Options::Option::RENDER_CLOUDS) {
            btn->setIconItem(
                std::shared_ptr<ItemInstance>(new ItemInstance(Item::feather)));
        } else if (item == Options::Option::VIEW_BOBBING) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(
                new ItemInstance((Item*)Item::boots_iron)));
        } else if (item == Options::Option::GUI_SCALE) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(
                new ItemInstance(Item::painting)));
        } else if (item == Options::Option::ADVANCED_OPENGL) {
            btn->setIconItem(
                std::shared_ptr<ItemInstance>(new ItemInstance(Item::diamond)));
        }

        buttons.push_back(btn);
    }

    // Profile button (Row 5): Quick preset selector for performance vs high-end
    profileButton = new Button(300, width / 2 - (btnW + 5),
                               startY + rowSpacing * 5, btnW, btnH, "");
    profileButton->setIconItem(
        std::shared_ptr<ItemInstance>(new ItemInstance(Item::apple_gold)));
    buttons.push_back(profileButton);

    // Original Xbox preset: Xbox graphics + unthrottled chunk loading
    s_xboxButton =
        new Button(301, width / 2 + 5, startY + rowSpacing * 5, btnW, btnH, "");
    s_xboxButton->setIconItem(
        std::shared_ptr<ItemInstance>(new ItemInstance(Item::diamond)));
    buttons.push_back(s_xboxButton);
    updateProfileButton();

    int doneW = (width >= 450) ? 230 : 190;
    Button* btnDone =
        new Button(200, width / 2 - doneW / 2, startY + rowSpacing * 6 + 3,
                   doneW, btnH, language->getElement("gui.done"));
    btnDone->setTextureIcon(64, 32, 16);
    btnDone->setIconItem(
        std::shared_ptr<ItemInstance>(new ItemInstance(Item::door_wood)));
    buttons.push_back(btnDone);
}

void VideoSettingsScreen::updateProfileButton() {
    if (profileButton == nullptr) return;
    if (s_xboxButton != nullptr) {
        s_xboxButton->msg =
            options->xboxPreset ? "Preset Xbox: SI" : "Preset Xbox: NO";
    }
    if (!options->fancyGraphics && !options->ambientOcclusion &&
        options->viewDistance >= 2) {
        profileButton->msg = "Perfil: Rendimiento";
    } else if (options->fancyGraphics && options->ambientOcclusion &&
               options->viewDistance <= 1) {
        profileButton->msg = "Perfil: Alta Calidad";
    } else {
        profileButton->msg = "Perfil: Equilibrado";
    }
}

void VideoSettingsScreen::buttonClicked(Button* button) {
    if (!button->active) return;
    if (button->id == 301) {
        options->xboxPreset = !options->xboxPreset;
        g_mcplXboxPreset = options->xboxPreset;
        if (options->xboxPreset) {
            // Original Xbox 360 look: fancy graphics, AO, clouds, far view, all
            // particles
            options->fancyGraphics = true;
            options->ambientOcclusion = true;
            options->renderClouds = true;
            options->viewDistance = 2;
            options->particles = 0;
        } else {
            // Back to the mobile-optimised look
            options->fancyGraphics = false;
            options->ambientOcclusion = false;
            options->renderClouds = false;
            options->viewDistance = 2;
            options->particles = 1;
        }
        if (minecraft->level) {
            minecraft->levelRenderer->allChanged();
        }
        minecraft->options->save();
        for (auto b : buttons) {
            if (b->id < 100) {
                SmallButton* sb = dynamic_cast<SmallButton*>(b);
                if (sb != nullptr)
                    sb->msg = options->getMessage(sb->getOption());
            }
        }
        updateProfileButton();
        return;
    }
    if (button->id == 300) {
        options->xboxPreset = false;
        g_mcplXboxPreset = false;
        // Preset cycle: Rendimiento -> Equilibrado -> Alta Calidad ->
        // Rendimiento
        if (!options->fancyGraphics && !options->ambientOcclusion &&
            options->viewDistance >= 2) {
            // Currently Rendimiento -> Switch to Equilibrado
            options->fancyGraphics = true;
            options->ambientOcclusion = true;
            options->renderClouds = true;
            options->viewDistance = 2;  // Short
            options->particles = 1;
        } else if (options->fancyGraphics && options->ambientOcclusion &&
                   options->viewDistance <= 1) {
            // Currently Alta Calidad -> Switch to Rendimiento (FPS+ for low-end
            // phones)
            options->fancyGraphics = false;
            options->ambientOcclusion = false;
            options->renderClouds = false;
            options->viewDistance = 2;  // Short (4 chunks)
            options->particles = 2;     // Minimal
            options->advancedOpengl = false;
        } else {
            // Currently Equilibrado (or custom) -> Switch to Alta Calidad
            // (High-end phones)
            options->fancyGraphics = true;
            options->ambientOcclusion = true;
            options->renderClouds = true;
            options->viewDistance = 1;  // Normal (8 chunks)
            options->particles = 0;     // All
        }

        if (minecraft->level) {
            minecraft->levelRenderer->allChanged();
        }
        minecraft->options->save();

        // Refresh existing button labels
        for (auto b : buttons) {
            if (b->id < 100) {
                SmallButton* sb = dynamic_cast<SmallButton*>(b);
                if (sb != nullptr) {
                    sb->msg = options->getMessage(sb->getOption());
                }
            }
        }
        updateProfileButton();
        return;
    }
    if (button->id < 100 && (dynamic_cast<SmallButton*>(button) != nullptr)) {
        options->toggle(((SmallButton*)button)->getOption(), 1);
        button->msg = options->getMessage(Options::Option::getItem(button->id));
        if (minecraft->level) {
            minecraft->levelRenderer->allChanged();
        }
        minecraft->options->save();
        updateProfileButton();
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