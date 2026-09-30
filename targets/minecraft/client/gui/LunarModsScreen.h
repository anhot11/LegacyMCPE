#pragma once

#include <string>
#include <vector>

#include "minecraft/client/gui/Button.h"
#include "minecraft/client/gui/Screen.h"

class Options;

class LunarModsScreen : public Screen {
private:
    Screen* lastScreen;
    int currentTab; // 0=Sodium, 1=OptiFine, 2=Fullbright, 3=Shaders & Textures

    // Tab navigation buttons
    Button* tabSodiumBtn;
    Button* tabOptifineBtn;
    Button* tabFullbrightBtn;
    Button* tabShadersBtn;

    // Sodium buttons
    Button* sodiumChunkEngineBtn;
    Button* sodiumEntityCullingBtn;
    Button* sodiumFogOcclusionBtn;

    // OptiFine buttons
    Button* optifineFastMathBtn;
    Button* optifineDynamicLightsBtn;
    Button* optifineClearWaterBtn;
    Button* optifineBetterGrassBtn;

    // Fullbright button
    Button* fullbrightToggleBtn;

    // Shaders & Textures buttons
    Button* shaderPresetBtn;
    Button* texturePackBtn;

    // Bottom action
    Button* doneBtn;

    void updateButtonVisibility();
    void updateButtonLabels();

public:
    LunarModsScreen(Screen* lastScreen);
    virtual ~LunarModsScreen() = default;

    virtual void init() override;
    virtual void buttonClicked(Button* button) override;
    virtual void render(int xm, int ym, float a) override;
};
