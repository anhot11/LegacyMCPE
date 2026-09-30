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

    // Profile button (Row 5): Quick preset selector for performance vs high-end
    profileButton = new Button(300, width / 2 - 155, height / 6 + 24 * 5, 310, 20, "");
    buttons.push_back(profileButton);
    updateProfileButton();

    buttons.push_back(new Button(200, width / 2 - 100, height / 6 + 24 * 6,
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
            // Switch to Equilibrado
            options->fancyGraphics = true;
            options->ambientOcclusion = true;
            options->renderClouds = true;
            options->viewDistance = 2; // Short
            options->particles = 1;
        } else if (options->fancyGraphics && options->ambientOcclusion && options->viewDistance <= 1) {
            // Switch to Rendimiento (FPS+ for low-end phones)
            options->fancyGraphics = false;
            options->ambientOcclusion = false;
            options->renderClouds = false;
            options->viewDistance = 2; // Short (4 chunks)
            options->particles = 2; // Minimal
            options->advancedOpengl = false;
        } else {
            // Switch to Alta Calidad (High-end phones)
            options->fancyGraphics = true;
            options->ambientOcclusion = true;
            options->renderClouds = true;
            options->viewDistance = 1; // Normal (8 chunks)
            options->particles = 0; // All
        }

        if (minecraft->level) {
            minecraft->levelRenderer->allChanged();
        }

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