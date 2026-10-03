#include "VideoSettingsScreen.h"

#include <vector>

#include "SlideButton.h"
#include "SmallButton.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/Options.h"
#include "minecraft/client/gui/Button.h"
#include "minecraft/client/gui/Screen.h"
#include "minecraft/client/renderer/LevelRenderer.h"
#include "minecraft/locale/Language.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/tile/Tile.h"

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
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::leaves)));
        } else if (item == Options::Option::RENDER_DISTANCE) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::eyeOfEnder)));
        } else if (item == Options::Option::AMBIENT_OCCLUSION) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::lightGem)));
        } else if (item == Options::Option::FRAMERATE_LIMIT) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::clock)));
        } else if (item == Options::Option::RENDER_CLOUDS) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::feather)));
        } else if (item == Options::Option::VIEW_BOBBING) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::boots_iron)));
        } else if (item == Options::Option::GUI_SCALE) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::painting)));
        } else if (item == Options::Option::ADVANCED_OPENGL) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::diamond)));
        } else if (item == Options::Option::GAMMA) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::torch)));
        } else if (item == Options::Option::FOV) {
            btn->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::bow)));
        }

        buttons.push_back(btn);
    }

    // Profile button (Row 5): Quick preset selector for performance vs high-end
    int profW = (btnW * 2) + 10;
    profileButton = new Button(300, width / 2 - profW / 2, startY + rowSpacing * 5, profW, btnH, "");
    profileButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::apple_gold)));
    buttons.push_back(profileButton);
    updateProfileButton();

    int doneW = (width >= 450) ? 230 : 190;
    Button* btnDone = new Button(200, width / 2 - doneW / 2, startY + rowSpacing * 6 + 3, doneW, btnH,
                                 language->getElement("gui.done"));
    btnDone->setTextureIcon(64, 32, 16);
    btnDone->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::door_wood)));
    buttons.push_back(btnDone);
}

void VideoSettingsScreen::updateProfileButton() {
    if (profileButton == nullptr) return;
    if (!options->fancyGraphics && !options->ambientOcclusion && options->viewDistance >= 2) {
        profileButton->msg = "Perfil Grafico: RENDIMIENTO (Bajo / FPS+)";
    } else if (options->fancyGraphics && options->ambientOcclusion && options->viewDistance <= 1) {
        profileButton->msg = "Perfil Grafico: ALTA CALIDAD (Gama Alta)";
    } else {
        profileButton->msg = "Perfil Grafico: EQUILIBRADO (Medio)";
    }
}

void VideoSettingsScreen::buttonClicked(Button* button) {
    if (!button->active) return;
    if (button->id == 300) {
        // Preset cycle: Rendimiento -> Equilibrado -> Alta Calidad -> Rendimiento
        if (!options->fancyGraphics && !options->ambientOcclusion && options->viewDistance >= 2) {
            // Currently Rendimiento -> Switch to Equilibrado
            options->fancyGraphics = true;
            options->ambientOcclusion = true;
            options->renderClouds = true;
            options->viewDistance = 2; // Short
            options->particles = 1;
        } else if (options->fancyGraphics && options->ambientOcclusion && options->viewDistance <= 1) {
            // Currently Alta Calidad -> Switch to Rendimiento (FPS+ for low-end phones)
            options->fancyGraphics = false;
            options->ambientOcclusion = false;
            options->renderClouds = false;
            options->viewDistance = 2; // Short (4 chunks)
            options->particles = 2; // Minimal
            options->advancedOpengl = false;
        } else {
            // Currently Equilibrado (or custom) -> Switch to Alta Calidad (High-end phones)
            options->fancyGraphics = true;
            options->ambientOcclusion = true;
            options->renderClouds = true;
            options->viewDistance = 1; // Normal (8 chunks)
            options->particles = 0; // All
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