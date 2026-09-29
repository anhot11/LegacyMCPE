#pragma once
#include "minecraft/client/gui/Button.h"

class ResourceLocation;

class AbstractBeaconButton : public Button {
protected:
    bool hovered;
    bool selected;
    ResourceLocation* iconRes;
    int iconU, iconV;

public:
    AbstractBeaconButton(int id, int x, int y);

    void setSelected(bool sel) { selected = sel; }
    bool isSelected() const { return selected; }
    bool isHovered() const { return hovered; }

    virtual void renderTooltip(int xm, int ym) = 0;

protected:
    virtual void renderBg(Minecraft* minecraft, int xm, int ym) override;
};