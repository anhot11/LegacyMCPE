#pragma once
#include <string>

#include "GuiComponent.h"

class Minecraft;

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
