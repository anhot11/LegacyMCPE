#include "Button.h"
#include "Font.h"

#include "minecraft/client/Lighting.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/renderer/Textures.h"
#include "minecraft/client/renderer/entity/ItemRenderer.h"
#include "minecraft/client/resources/ResourceLocation.h"
#include "minecraft/world/item/ItemInstance.h"
#include "platform/renderer/renderer.h"
#include "platform/stubs.h"
class Minecraft;

#ifdef ENABLE_JAVA_GUIS
ResourceLocation GUI_GUI_LOCATION = ResourceLocation(TN_GUI_GUI);
#endif

Button::Button(int id, int x, int y, const std::string& msg) {
    init(id, x, y, 200, 20, msg);
}

Button::Button(int id, int x, int y, int w, int h, const std::string& msg) {
    init(id, x, y, w, h, msg);
}

// 4J - added
void Button::init(int id, int x, int y, int w, int h, const std::string& msg) {
    active = true;
    visible = true;

    // this bit of code from original ctor
    this->id = id;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->msg = msg;
}

int Button::getYImage(bool hovered) {
    int res = 1;
    if (!active)
        res = 0;
    else if (hovered)
        res = 2;
    return res;
}

void Button::render(Minecraft* minecraft, int xm, int ym) {
#ifdef ENABLE_JAVA_GUIS
    if (!visible) return;

    Font* font = minecraft->font;

    // glBindTexture(GL_TEXTURE_2D, minecraft->textures->loadTexture(
    //  TN_GUI_GUI));  // 4J was "/gui/gui.png"
    minecraft->textures->bindTexture(&GUI_GUI_LOCATION);
    glColor4f(1, 1, 1, 1);

    bool hovered = xm >= x && ym >= y && xm < x + w && ym < y + h;
    int yImage = getYImage(hovered);
    int v0 = 46 + yImage * 20;

    if (h == 20 && w <= 200) {
        blit(x, y, 0, v0, w / 2, h);
        blit(x + w / 2, y, 200 - w / 2, v0, w - w / 2, h);
    } else {
        int hTop = h / 2;
        int hBot = h - hTop;
        int twHalf = std::min(w / 2, 100);

        // Top half (samples texture v0 .. v0 + 10)
        blit(x, y, 0, v0, w / 2, hTop, twHalf, 10);
        blit(x + w / 2, y, 200 - twHalf, v0, w - w / 2, hTop, twHalf, 10);

        // Bottom half (samples texture v0 + 10 .. v0 + 20)
        blit(x, y + hTop, 0, v0 + 10, w / 2, hBot, twHalf, 10);
        blit(x + w / 2, y + hTop, 200 - twHalf, v0 + 10, w - w / 2, hBot, twHalf, 10);
    }

    if (textureIcon.enabled) {
        minecraft->textures->bindTexture("gui/icons_menu.png");
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 1.0f, active ? 1.0f : 0.5f);

        int iconSize = textureIcon.size;
        int iconX = x + 6;
        int iconY = y + (h - iconSize) / 2;
        blit(iconX, iconY, textureIcon.u, textureIcon.v, iconSize, iconSize);

        glDisable(GL_BLEND);
        minecraft->textures->bindTexture(&GUI_GUI_LOCATION);
    } else if (iconItem != nullptr) {
        static ItemRenderer* s_btnItemRenderer = nullptr;
        if (!s_btnItemRenderer) s_btnItemRenderer = new ItemRenderer();

        Lighting::turnOnGui();
        glEnable(GL_RESCALE_NORMAL);
        glEnable(GL_COLOR_MATERIAL);

        float iconScale = (h >= 28) ? 1.2f : 0.9f;
        float iconX = (float)(x + 6);
        float iconY = (float)(y + (h - (int)(16.0f * iconScale)) / 2);

        s_btnItemRenderer->renderGuiItem(font, minecraft->textures, iconItem,
                                         iconX, iconY, iconScale, 1.0f);

        Lighting::turnOff();
        glDisable(GL_RESCALE_NORMAL);
        glDisable(GL_COLOR_MATERIAL);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        minecraft->textures->bindTexture(&GUI_GUI_LOCATION);
    }

    renderBg(minecraft, xm, ym);

    int textY = y + (h - 8) / 2;
    int color = !active ? 0xffa0a0a0 : (hovered ? 0xffffa0 : 0xe0e0e0);

    if (hasIcon()) {
        int availStart = x + 24;
        int availW = w - 28;
        int strW = font->width(msg);
        if (strW >= availW) {
            drawString(font, msg, availStart, textY, color);
        } else {
            drawCenteredString(font, msg, availStart + availW / 2, textY, color);
        }
    } else {
        drawCenteredString(font, msg, x + w / 2, textY, color);
    }
#endif
}

void Button::renderBg(Minecraft* minecraft, int xm, int ym) {}

void Button::released(int mx, int my) {}

bool Button::clicked(Minecraft* minecraft, int mx, int my) {
    return visible && active && mx >= x && my >= y && mx < x + w && my < y + h;
}