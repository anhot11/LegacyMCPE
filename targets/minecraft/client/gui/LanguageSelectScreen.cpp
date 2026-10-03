#include "LanguageSelectScreen.h"

#include <cmath>
#include <algorithm>

#include "Button.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/Options.h"
#include "minecraft/client/gui/Font.h"
#include "minecraft/client/renderer/Tesselator.h"
#include "minecraft/client/renderer/Textures.h"
#include "app/common/Audio/ConsoleSoundEngine.h"
#include "app/common/Audio/SoundTypes.h"
#include "minecraft/locale/Language.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "platform/renderer/renderer.h"
#include "platform/input/input.h"
#include "platform/stubs.h"

LanguageSelectScreen::LanguageSelectScreen(Screen* lastScreen, Options* options) {
    this->lastScreen = lastScreen;
    this->options = options;
    this->selectedIndex = 0;
    this->scrollY = 0.0f;
    this->maxScroll = 0.0f;
    this->isDragging = false;
    this->isDraggingScrollbar = false;
    this->dragStartY = 0;
    this->dragStartScroll = 0.0f;
    this->hasMoved = false;
    this->btnDone = nullptr;
    this->btnScrollUp = nullptr;
    this->btnScrollDown = nullptr;
}

void LanguageSelectScreen::init() {
    Language* lang = Language::getInstance();
    title = lang->getElement("options.language");

    // All 15 supported languages with flag UVs in flags.png (24x16 each)
    languages = {
        {"es_ES", "Español", "España", 96, 0},
        {"es_MX", "Español", "México", 128, 0},
        {"en_US", "English", "US", 0, 0},
        {"en_GB", "English", "UK", 32, 0},
        {"pt_BR", "Português", "Brasil", 64, 24},
        {"pt_PT", "Português", "Portugal", 96, 24},
        {"fr_FR", "Français", "France", 160, 0},
        {"fr_CA", "Français", "Canada", 192, 0},
        {"de_DE", "Deutsch", "Deutschland", 64, 0},
        {"it_IT", "Italiano", "Italia", 224, 0},
        {"ru_RU", "Русский", "Россия", 128, 24},
        {"ja_JP", "日本語", "日本", 0, 24},
        {"ko_KR", "한국어", "대한민국", 32, 24},
        {"zh_CN", "简体中文", "中国", 160, 24},
        {"zh_TW", "繁體中文", "台灣", 192, 24},
    };

    selectedIndex = 0;
    for (size_t i = 0; i < languages.size(); i++) {
        if (languages[i].code == options->language) {
            selectedIndex = (int)i;
            break;
        }
    }

    int botW = (width >= 450) ? 220 : 180;
    btnDone = new Button(DONE_BUTTON_ID, width / 2 - botW / 2, height - 34, botW, 24,
                         lang->getElement("gui.done"));
    btnDone->setTextureIcon(64, 32, 16);
    btnDone->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::door_wood)));
    buttons.push_back(btnDone);

    int slotW = (width >= 450) ? 280 : 240;
    int barX = width / 2 + slotW / 2 + 6;
    btnScrollUp = new Button(SCROLL_UP_BUTTON_ID, barX, 32, 22, 20, "^");
    btnScrollDown = new Button(SCROLL_DOWN_BUTTON_ID, barX, height - 64, 22, 20, "v");
    buttons.push_back(btnScrollUp);
    buttons.push_back(btnScrollDown);
}

void LanguageSelectScreen::selectLanguage(int index) {
    if (index < 0 || index >= (int)languages.size()) return;
    selectedIndex = index;
    options->language = languages[index].code;
    Language::getInstance()->loadLanguage(options->language);
    options->save();

    title = Language::getInstance()->getElement("options.language");
    if (btnDone) {
        btnDone->msg = Language::getInstance()->getElement("gui.done");
    }
    minecraft->soundEngine->playUI(eSoundType_RANDOM_CLICK, 1.0f, 1.0f);
}

void LanguageSelectScreen::buttonClicked(Button* button) {
    if (!button->active) return;
    if (button->id == DONE_BUTTON_ID) {
        options->save();
        minecraft->setScreen(lastScreen);
    } else if (button->id == SCROLL_UP_BUTTON_ID) {
        scrollY = std::max(0.0f, scrollY - 64.0f);
    } else if (button->id == SCROLL_DOWN_BUTTON_ID) {
        scrollY = std::min(maxScroll, scrollY + 64.0f);
    }
}

static void renderClippedDirt(Minecraft* mc, int width, int yStart, int yEnd) {
    if (yEnd <= yStart) return;
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    Tesselator* t = Tesselator::getInstance();
    glBindTexture(GL_TEXTURE_2D, mc->textures->loadTexture(TN_GUI_BACKGROUND));
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    float s = 32.0f;
    t->begin();
    t->color(0x404040);
    t->vertexUV(0.0f, (float)yEnd, 0.0f, 0.0f, (float)yEnd / s);
    t->vertexUV((float)width, (float)yEnd, 0.0f, (float)width / s, (float)yEnd / s);
    t->vertexUV((float)width, (float)yStart, 0.0f, (float)width / s, (float)yStart / s);
    t->vertexUV(0.0f, (float)yStart, 0.0f, 0.0f, (float)yStart / s);
    t->end();
}

void LanguageSelectScreen::render(int xm, int ym, float a) {
    int listY0 = 30;
    int listY1 = height - 42;
    int slotHeight = 32;
    int totalHeight = (int)languages.size() * slotHeight;
    maxScroll = std::max(0.0f, (float)(totalHeight - (listY1 - listY0)));

    int slotW = (width >= 450) ? 280 : 240;
    int slotX = width / 2 - slotW / 2;
    int barX = slotX + slotW + 6;
    int barW = 10;
    int trackY0 = listY0 + 26;
    int trackY1 = listY1 - 26;
    int trackH = trackY1 - trackY0;
    int thumbH = (trackH > 0 && totalHeight > 0)
                     ? std::max(24, (int)((float)trackH * (float)trackH / (float)totalHeight))
                     : 24;

    // 1. Draw base dirt background
    renderDirtBackground(0);

    // 2. Touch / drag input handling
    bool isDown = PlatformInput.ButtonDown(0, MINECRAFT_ACTION_ACTION);
    if (isDown) {
        if (!isDragging && !isDraggingScrollbar) {
            // Check if touch started on or near the scrollbar track (generous hit box)
            if (maxScroll > 0.0f && xm >= barX - 10 && xm <= barX + barW + 16 &&
                ym >= trackY0 && ym <= trackY1) {
                isDraggingScrollbar = true;
            } else if (ym >= listY0 && ym <= listY1 && xm < barX - 8) {
                isDragging = true;
                dragStartY = ym;
                dragStartScroll = scrollY;
                hasMoved = false;
            }
        }

        if (isDraggingScrollbar && maxScroll > 0.0f) {
            float rel = (float)(ym - trackY0 - thumbH / 2) / (float)(trackH - thumbH);
            scrollY = rel * maxScroll;
            if (scrollY < 0.0f) scrollY = 0.0f;
            if (scrollY > maxScroll) scrollY = maxScroll;
        } else if (isDragging) {
            int dy = ym - dragStartY;
            if (std::abs(dy) > 4) {
                hasMoved = true;
            }
            scrollY = dragStartScroll - dy;
            if (scrollY < 0.0f) scrollY = 0.0f;
            if (scrollY > maxScroll) scrollY = maxScroll;
        }
    } else {
        if (isDragging) {
            if (!hasMoved) {
                int clickedY = dragStartY - listY0 + (int)scrollY;
                int slot = clickedY / slotHeight;
                if (slot >= 0 && slot < (int)languages.size()) {
                    selectLanguage(slot);
                }
            }
            isDragging = false;
        }
        isDraggingScrollbar = false;
    }

    // 3. Render items
    for (size_t i = 0; i < languages.size(); i++) {
        int itemY = listY0 + 4 - (int)scrollY + (int)i * slotHeight;
        if (itemY + slotHeight < listY0 || itemY > listY1) continue;

        // Selection background
        if ((int)i == selectedIndex) {
            fill(slotX - 2, itemY - 1, slotX + slotW + 2, itemY + slotHeight - 3, 0x80ffffff);
            fill(slotX - 1, itemY, slotX + slotW + 1, itemY + slotHeight - 4, 0x60000000);
        } else {
            fill(slotX, itemY, slotX + slotW, itemY + slotHeight - 4, 0x40000000);
        }

        // Draw country flag
        minecraft->textures->bindTexture("gui/flags.png");
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        int flagX = slotX + 8;
        int flagY = itemY + (slotHeight - 4 - 16) / 2;
        blit(flagX, flagY, languages[i].flagU, languages[i].flagV, 24, 16);

        // Flag border
        hLine(flagX - 1, flagX + 24, flagY - 1, 0xff303030);
        hLine(flagX - 1, flagX + 24, flagY + 16, 0xff303030);
        vLine(flagX - 1, flagY - 1, flagY + 16, 0xff303030);
        vLine(flagX + 24, flagY - 1, flagY + 16, 0xff303030);

        // Language text
        std::string nameStr = languages[i].name;
        std::string regionStr = "(" + languages[i].region + ") - " + languages[i].code;
        int titleColor = ((int)i == selectedIndex) ? 0xffff55 : 0xffffff;
        drawString(font, nameStr, flagX + 32, itemY + 3, titleColor);
        drawString(font, regionStr, flagX + 32, itemY + 15, 0xaaaaaa);
    }

    // 4. Clip header & footer masks (prevents items bleeding over title & buttons)
    renderClippedDirt(minecraft, width, 0, listY0);
    renderClippedDirt(minecraft, width, listY1, height);

    // 5. Draw top and bottom shadows
    fillGradient(0, listY0, width, listY0 + 4, 0xff000000, 0x00000000);
    fillGradient(0, listY1 - 4, width, listY1, 0x00000000, 0xff000000);

    // 6. Draw scrollbar
    if (maxScroll > 0.0f && trackH > thumbH) {
        int thumbY = trackY0 + (int)(scrollY * (float)(trackH - thumbH) / maxScroll);
        // Track background
        fill(barX, trackY0, barX + barW, trackY1, 0x80000000);
        // Thumb border
        fill(barX, thumbY, barX + barW, thumbY + thumbH, 0xff666666);
        // Thumb body
        fill(barX + 1, thumbY + 1, barX + barW - 1, thumbY + thumbH - 1, 0xffcccccc);
    }

    // 7. Title & Screen elements (Done & Scroll buttons)
    drawCenteredString(font, title, width / 2, 12, 0xffffff);
    Screen::render(xm, ym, a);
}
