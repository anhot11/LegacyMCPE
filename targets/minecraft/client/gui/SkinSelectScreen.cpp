#include "SkinSelectScreen.h"

#include <cmath>
#include <fstream>
#include <string>
#include <vector>

#include "minecraft/client/Lighting.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/Options.h"
#include "minecraft/client/gui/Button.h"
#include "minecraft/client/gui/Font.h"
#include "minecraft/client/model/HumanoidModel.h"
#include "minecraft/client/renderer/Tesselator.h"
#include "minecraft/client/renderer/Textures.h"
#include "minecraft/locale/Language.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/entity/player/SkinTypes.h"
#include "app/common/Audio/SoundTypes.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "platform/input/input.h"
#include "platform/renderer/renderer.h"
#include "platform/stubs.h"

#define DONE_BUTTON_ID 200
#define SCROLL_UP_BUTTON_ID 201
#define SCROLL_DOWN_BUTTON_ID 202

static bool fileExists(const std::string& path) {
    std::ifstream f(path.c_str());
    return f.good();
}

SkinSelectScreen::SkinSelectScreen(Screen* lastScreen)
    : lastScreen(lastScreen),
      previewModel(nullptr),
      selectedIndex(0),
      btnDone(nullptr),
      btnScrollUp(nullptr),
      btnScrollDown(nullptr),
      scrollY(0.0f),
      maxScroll(0.0f),
      isDragging(false),
      isDraggingScrollbar(false),
      dragStartY(0),
      dragStartScroll(0.0f),
      hasMoved(false),
      previewYaw(0.0f),
      previewPitch(0.0f),
      isDraggingPreview(false),
      previewDragStartX(0),
      previewDragStartY(0),
      previewDragStartYaw(0.0f),
      previewDragStartPitch(0.0f),
      vo(0.0f) {
    previewModel = new HumanoidModel(0.0f);

    // Register built-in skins
    skins.push_back({"Default", "Steve", "Aspecto Clásico", TN_MOB_CHAR, ""});
    skins.push_back({"Alex", "Alex", "Aspecto Clásico", TN_COUNT, "mob/alex.png"});
    skins.push_back({"Tennis", "Tennis Steve", "Deportes", TN_MOB_CHAR1, ""});
    skins.push_back({"Tuxedo", "Tuxedo Steve", "Formal", TN_MOB_CHAR2, ""});
    skins.push_back({"Athlete", "Athlete Steve", "Atleta", TN_MOB_CHAR3, ""});
    skins.push_back({"Scottish", "Scottish Steve", "Tradicional", TN_MOB_CHAR4, ""});
    skins.push_back({"Prisoner", "Prisoner Steve", "Rayas", TN_MOB_CHAR5, ""});
    skins.push_back({"Cyclist", "Cyclist Steve", "Ciclista", TN_MOB_CHAR6, ""});
    skins.push_back({"Boxer", "Boxer Steve", "Boxeo", TN_MOB_CHAR7, ""});

    // Check for custom skin files on external storage
    std::vector<std::string> customPaths = {
        "/sdcard/custom_skin.png",
        "/sdcard/Download/skin.png",
        "/sdcard/Download/custom_skin.png",
        "/sdcard/LegacyMCPE/skin.png"
    };

    for (const auto& cp : customPaths) {
        if (fileExists(cp)) {
            skins.push_back({"Custom", "Skin Personalizada", cp, TN_COUNT, cp});
            break;
        }
    }
}

SkinSelectScreen::~SkinSelectScreen() {
    if (previewModel != nullptr) {
        delete previewModel;
        previewModel = nullptr;
    }
}

void SkinSelectScreen::init() {
    buttons.clear();

    Options* options = minecraft->options;
    selectedIndex = 0;
    for (size_t i = 0; i < skins.size(); i++) {
        if (skins[i].id == options->skin) {
            selectedIndex = (int)i;
            break;
        }
    }

    int botW = (width >= 450) ? 220 : 180;
    btnDone = new Button(DONE_BUTTON_ID, width / 2 - botW / 2, height - 32, botW, 24, "Aceptar");
    btnDone->setTextureIcon(64, 32, 16);
    btnDone->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::door_wood)));
    buttons.push_back(btnDone);

    int listW = (width >= 500) ? 230 : 180;
    int barX = 24 + listW + 4;
    btnScrollUp = new Button(SCROLL_UP_BUTTON_ID, barX, 32, 20, 20, "^");
    btnScrollDown = new Button(SCROLL_DOWN_BUTTON_ID, barX, height - 58, 20, 20, "v");
    buttons.push_back(btnScrollUp);
    buttons.push_back(btnScrollDown);
}

void SkinSelectScreen::tick() {
    vo += 1.0f;
}

int SkinSelectScreen::bindSkinTexture(const SkinEntry& entry) {
    if (entry.textureEnum != TN_COUNT) {
        return minecraft->textures->loadTexture(entry.textureEnum);
    }
    if (!entry.texturePath.empty()) {
        return minecraft->textures->loadTexture(TN_COUNT, entry.texturePath);
    }
    return minecraft->textures->loadTexture(TN_MOB_CHAR);
}

int SkinSelectScreen::getActiveSkinTexture(Minecraft* mc) {
    if (!mc || !mc->options) {
        return mc ? mc->textures->loadTexture(TN_MOB_CHAR) : 0;
    }
    const std::string& skin = mc->options->skin;
    if (skin == "Alex" || skin == "alex" || skin == "mob/alex.png") {
        return mc->textures->loadTexture(TN_COUNT, "mob/alex.png");
    }
    if (skin == "Tennis" || skin == "Tennis Steve" || skin == "Skin1") {
        return mc->textures->loadTexture(TN_MOB_CHAR1);
    }
    if (skin == "Tuxedo" || skin == "Tuxedo Steve" || skin == "Skin2") {
        return mc->textures->loadTexture(TN_MOB_CHAR2);
    }
    if (skin == "Athlete" || skin == "Athlete Steve" || skin == "Skin3") {
        return mc->textures->loadTexture(TN_MOB_CHAR3);
    }
    if (skin == "Scottish" || skin == "Scottish Steve" || skin == "Skin4") {
        return mc->textures->loadTexture(TN_MOB_CHAR4);
    }
    if (skin == "Prisoner" || skin == "Prisoner Steve" || skin == "Skin5") {
        return mc->textures->loadTexture(TN_MOB_CHAR5);
    }
    if (skin == "Cyclist" || skin == "Cyclist Steve" || skin == "Skin6") {
        return mc->textures->loadTexture(TN_MOB_CHAR6);
    }
    if (skin == "Boxer" || skin == "Boxer Steve" || skin == "Skin7") {
        return mc->textures->loadTexture(TN_MOB_CHAR7);
    }
    if (skin == "Custom" || skin.find(".png") != std::string::npos) {
        int tid = mc->textures->loadTexture(TN_COUNT, skin);
        if (tid > 0) return tid;
    }
    return mc->textures->loadTexture(TN_MOB_CHAR);
}

void SkinSelectScreen::selectSkin(int index) {
    if (index < 0 || index >= (int)skins.size()) return;
    selectedIndex = index;
    minecraft->options->skin = skins[index].id;
    minecraft->options->save();

    // Sync in-game player skin if player is active
    if (minecraft->player != nullptr) {
        if (skins[index].id == "Default") {
            minecraft->player->setPlayerDefaultSkin(EDefaultSkins::Skin0);
        } else if (skins[index].id == "Tennis") {
            minecraft->player->setPlayerDefaultSkin(EDefaultSkins::Skin1);
        } else if (skins[index].id == "Tuxedo") {
            minecraft->player->setPlayerDefaultSkin(EDefaultSkins::Skin2);
        } else if (skins[index].id == "Athlete") {
            minecraft->player->setPlayerDefaultSkin(EDefaultSkins::Skin3);
        } else if (skins[index].id == "Scottish") {
            minecraft->player->setPlayerDefaultSkin(EDefaultSkins::Skin4);
        } else if (skins[index].id == "Prisoner") {
            minecraft->player->setPlayerDefaultSkin(EDefaultSkins::Skin5);
        } else if (skins[index].id == "Cyclist") {
            minecraft->player->setPlayerDefaultSkin(EDefaultSkins::Skin6);
        } else if (skins[index].id == "Boxer") {
            minecraft->player->setPlayerDefaultSkin(EDefaultSkins::Skin7);
        }
    }

    minecraft->soundEngine->playUI(eSoundType_RANDOM_CLICK, 1.0f, 1.0f);
}

void SkinSelectScreen::buttonClicked(Button* button) {
    if (!button->active) return;
    if (button->id == DONE_BUTTON_ID) {
        minecraft->options->save();
        minecraft->setScreen(lastScreen);
    } else if (button->id == SCROLL_UP_BUTTON_ID) {
        scrollY = std::max(0.0f, scrollY - 60.0f);
    } else if (button->id == SCROLL_DOWN_BUTTON_ID) {
        scrollY = std::min(maxScroll, scrollY + 60.0f);
    }
}

void SkinSelectScreen::mouseClicked(int xm, int ym, int buttonNum) {
    Screen::mouseClicked(xm, ym, buttonNum);
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

void SkinSelectScreen::render(int xm, int ym, float a) {
    int listY0 = 30;
    int listY1 = height - 40;
    int slotHeight = 36;
    int totalHeight = (int)skins.size() * slotHeight;
    maxScroll = std::max(0.0f, (float)(totalHeight - (listY1 - listY0)));

    int listX = 20;
    int listW = (width >= 500) ? 230 : 180;
    int barX = listX + listW + 4;
    int barW = 8;
    int trackY0 = listY0 + 26;
    int trackY1 = listY1 - 26;
    int trackH = trackY1 - trackY0;
    int thumbH = (trackH > 0 && totalHeight > 0)
                     ? std::max(20, (int)((float)trackH * (float)trackH / (float)totalHeight))
                     : 20;

    // Right Preview Area
    int previewLeft = barX + barW + 16;
    int previewRight = width - 16;
    int previewCenterX = (previewLeft + previewRight) / 2;
    int previewCenterY = height / 2 + 10;

    // 1. Draw base dirt background
    renderDirtBackground(0);

    // 2. Touch / input handling
    bool isDown = PlatformInput.ButtonDown(0, MINECRAFT_ACTION_ACTION);
    if (isDown) {
        if (!isDragging && !isDraggingScrollbar && !isDraggingPreview) {
            // Check if touch started on 3D preview area
            if (xm >= previewLeft && xm <= previewRight && ym >= listY0 && ym <= listY1) {
                isDraggingPreview = true;
                previewDragStartX = xm;
                previewDragStartY = ym;
                previewDragStartYaw = previewYaw;
                previewDragStartPitch = previewPitch;
            } else if (maxScroll > 0.0f && xm >= barX - 10 && xm <= barX + barW + 16 &&
                       ym >= trackY0 && ym <= trackY1) {
                isDraggingScrollbar = true;
            } else if (ym >= listY0 && ym <= listY1 && xm >= listX && xm <= listX + listW) {
                isDragging = true;
                dragStartY = ym;
                dragStartScroll = scrollY;
                hasMoved = false;
            }
        }

        if (isDraggingPreview) {
            int dx = xm - previewDragStartX;
            int dy = ym - previewDragStartY;
            previewYaw = previewDragStartYaw - (float)dx * 0.9f;
            previewPitch = previewDragStartPitch + (float)dy * 0.4f;
            if (previewPitch > 30.0f) previewPitch = 30.0f;
            if (previewPitch < -30.0f) previewPitch = -30.0f;
        } else if (isDraggingScrollbar && maxScroll > 0.0f) {
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
                if (slot >= 0 && slot < (int)skins.size()) {
                    selectSkin(slot);
                }
            }
            isDragging = false;
        }
        isDraggingScrollbar = false;
        isDraggingPreview = false;
    }

    // 3. Render skin list items
    for (size_t i = 0; i < skins.size(); i++) {
        int itemY = listY0 + 4 - (int)scrollY + (int)i * slotHeight;
        if (itemY + slotHeight < listY0 || itemY > listY1) continue;

        bool isSelected = ((int)i == selectedIndex);

        // Selection background
        if (isSelected) {
            fill(listX - 2, itemY - 1, listX + listW + 2, itemY + slotHeight - 3, 0x80ffffff);
            fill(listX - 1, itemY, listX + listW + 1, itemY + slotHeight - 4, 0x70005500);
        } else {
            fill(listX, itemY, listX + listW, itemY + slotHeight - 4, 0x40000000);
        }

        // Skin label
        int titleColor = isSelected ? 0x55ff55 : 0xffffff;
        drawString(font, skins[i].displayName, listX + 8, itemY + 4, titleColor);
        drawString(font, skins[i].subtitle, listX + 8, itemY + 18, 0x888888);

        // Equipped badge on selected item
        if (isSelected) {
            std::string equippedStr = "[ACTIVO]";
            drawString(font, equippedStr, listX + listW - font->width(equippedStr) - 6, itemY + 11, 0x55ff55);
        }
    }

    // 4. Clip header & footer masks
    renderClippedDirt(minecraft, width, 0, listY0);
    renderClippedDirt(minecraft, width, listY1, height);

    // 5. Draw shadows
    fillGradient(0, listY0, width, listY0 + 4, 0xff000000, 0x00000000);
    fillGradient(0, listY1 - 4, width, listY1, 0x00000000, 0xff000000);

    // 6. Draw scrollbar
    if (maxScroll > 0.0f && trackH > thumbH) {
        int thumbY = trackY0 + (int)(scrollY * (float)(trackH - thumbH) / maxScroll);
        fill(barX, trackY0, barX + barW, trackY1, 0x80000000);
        fill(barX, thumbY, barX + barW, thumbY + thumbH, 0xff666666);
        fill(barX + 1, thumbY + 1, barX + barW - 1, thumbY + thumbH - 1, 0xffcccccc);
    }

    // 7. Render 3D Preview on the right
    if (previewModel != nullptr && selectedIndex >= 0 && selectedIndex < (int)skins.size()) {
        float ss = (height >= 300) ? 100.0f : 80.0f;
        int pX = previewCenterX;
        int pY = (int)((float)height - 24.0f - 1.5f * ss);

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glDisable(GL_COLOR_MATERIAL);
        glEnable(GL_RESCALE_NORMAL);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(true);
        glClear(GL_DEPTH_BUFFER_BIT);

        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.1f);
        glDisable(GL_CULL_FACE);

        Lighting::turnOnGui();

        glPushMatrix();
        glTranslatef((float)pX, (float)pY, 50.0f);
        glScalef(-ss, ss, ss);

        // Rotation: 180 to face camera + user drag rotation
        float currentYaw = 180.0f + previewYaw;
        glRotatef(currentYaw, 0.0f, 1.0f, 0.0f);

        int texId = bindSkinTexture(skins[selectedIndex]);
        glBindTexture(GL_TEXTURE_2D, texId);

        previewModel->attackTime = 0.0f;
        previewModel->holdingRightHand = 0;
        previewModel->holdingLeftHand = 0;
        previewModel->sneaking = false;
        previewModel->idle = false;
        previewModel->eating = false;
        previewModel->eating_swing = 0.0f;
        previewModel->eating_t = 0.0f;
        previewModel->young = false;
        previewModel->riding = false;

        float subtleHeadYaw = sinf(vo * 0.03f) * 2.0f;
        float headPitch = previewPitch + cosf(vo * 0.03f) * 1.5f;

        previewModel->render(nullptr, 0.0f, 0.0f, 0.0f, subtleHeadYaw, headPitch, 1.0f / 16.0f, true);

        glPopMatrix();
        Lighting::turnOff();
        glDisable(GL_RESCALE_NORMAL);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(false);
        glEnable(GL_CULL_FACE);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        // Hint below 3D character
        std::string hint = "Arrastra para rotar 360°";
        drawCenteredString(font, hint, previewCenterX, height - 52, 0x888888);
    }

    // 8. Screen Title & Buttons
    std::string screenTitle = "Selector de Aspectos (Skins)";
    drawCenteredString(font, screenTitle, width / 2, 10, 0xffffff);

    Screen::render(xm, ym, a);
}
