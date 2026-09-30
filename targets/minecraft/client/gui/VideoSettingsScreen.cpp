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

    int btnW = 160;
    int btnH = 24;
    int rowSpacing = 27;
    int startY = height / 6 - 6;

    for (int i = 0; i < ITEM_COUNT; i++) {
        const Options::Option* item = items[i];
        int xPos = width / 2 - 165 + (i % 2 * 170);
        int yPos = startY + rowSpacing * (i / 2);

        if (!item->isProgress()) {
            buttons.push_back(new SmallButton(item->getId(), xPos, yPos, btnW, btnH, item,
                                              options->getMessage(item)));
        } else {
            buttons.push_back(new SlideButton(item->getId(), xPos, yPos, btnW, btnH, item,
                                              options->getMessage(item),
                                              options->getProgressValue(item)));
        }
    }

    // Profile button (Row 5): Quick preset selector for performance vs high-end
    profileButton = new Button(300, width / 2 - 165, startY + rowSpacing * 5, 330, btnH, "");
    buttons.push_back(profileButton);
    updateProfileButton();

    buttons.push_back(new Button(200, width / 2 - 110, startY + rowSpacing * 6, 220, btnH,
                                 language->getElement("gui.done")));
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