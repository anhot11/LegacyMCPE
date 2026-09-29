#pragma once
#include <string>

#include "Button.h"
#include "minecraft/client/Options.h"

class Minecraft;

class SlideButton : public Button {
public:
    float value;
    bool sliding;

private:
    const Options::Option* option;

public:
    SlideButton(int id, int x, int y, const Options::Option* option,
                const std::string& msg, float value);

protected:
    virtual int getYImage(bool hovered) override;
    virtual void renderBg(Minecraft* minecraft, int xm, int ym) override;

public:
    virtual bool clicked(Minecraft* minecraft, int mx, int my) override;
    virtual void released(int mx, int my) override;
};