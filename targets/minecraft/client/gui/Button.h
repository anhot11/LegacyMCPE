#pragma once
#include <memory>
#include <string>

#include "GuiComponent.h"

class Minecraft;
class ItemInstance;

class Button : public GuiComponent {
protected:
    int w;
    int h;

public:
    int x, y;
    std::string msg;
    int id;
    bool active;
    bool visible;

    struct TextureIcon {
        bool enabled = false;
        int u = 0;
        int v = 0;
        int size = 16;
    };
    TextureIcon textureIcon;
    std::shared_ptr<ItemInstance> iconItem = nullptr;

    void setIconItem(std::shared_ptr<ItemInstance> item) { this->iconItem = item; }
    void setTextureIcon(int u, int v, int size = 16) {
        textureIcon.enabled = true;
        textureIcon.u = u;
        textureIcon.v = v;
        textureIcon.size = size;
    }
    bool hasIcon() const { return textureIcon.enabled || (iconItem != nullptr); }

    Button(int id, int x, int y, const std::string& msg);
    Button(int id, int x, int y, int w, int h, const std::string& msg);
    void init(int id, int x, int y, int w, int h,
              const std::string& msg);  // 4J - added

protected:
    virtual int getYImage(bool hovered);

public:
    virtual void render(Minecraft* minecraft, int xm, int ym);

protected:
    virtual void renderBg(Minecraft* minecraft, int xm, int ym);

public:
    virtual void released(int mx, int my);
    virtual bool clicked(Minecraft* minecraft, int mx, int my);
};
