#pragma once
#include <vector>

#include "minecraft/client/gui/GuiComponent.h"

class GuiParticle;
class Minecraft;

class GuiParticles : public GuiComponent {
private:
    std::vector<GuiParticle*> particles;
    Minecraft* mc;

public:
    GuiParticles(Minecraft* mc);
    void tick();
    void add(GuiParticle* guiParticle);
    void render(float a);
};
