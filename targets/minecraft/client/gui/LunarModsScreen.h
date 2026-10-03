#pragma once

#include <memory>
#include <string>
#include <vector>

#include "minecraft/client/gui/Button.h"
#include "minecraft/client/gui/Screen.h"

class Options;
class ItemInstance;
class ItemRenderer;

class LunarModsScreen : public Screen {
private:
    Screen* lastScreen;
    int currentTab; // 0=Sodium, 1=OptiFine, 2=Fullbright, 3=Shaders & Textures

    static ItemRenderer* itemRenderer;

    // Mod Category & Feature Icons
    std::shared_ptr<ItemInstance> iconSodiumEngine;
    std::shared_ptr<ItemInstance> iconEntityCulling;
    std::shared_ptr<ItemInstance> iconFogOcclusion;

    std::shared_ptr<ItemInstance> iconDynamicLights;
    std::shared_ptr<ItemInstance> iconFastMath;
    std::shared_ptr<ItemInstance> iconClearWater;
    std::shared_ptr<ItemInstance> iconBetterGrass;

    std::shared_ptr<ItemInstance> iconFullbright;
    std::shared_ptr<ItemInstance> iconShaders;
    std::shared_ptr<ItemInstance> iconTexturePack;
    std::shared_ptr<ItemInstance> iconLunarStar;
    std::shared_ptr<ItemInstance> iconThermal;
    std::shared_ptr<ItemInstance> iconControls;

    // Tab navigation buttons
    Button* tabSodiumBtn;
    Button* tabOptifineBtn;
    Button* tabFullbrightBtn;
    Button* tabShadersBtn;
    Button* tabThermalBtn;
    Button* tabControlsBtn;

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

    // Thermal Protection button
    Button* thermalProtectionBtn;

    // Controls settings buttons
    Button* controlStyleBtn;
    Button* controlScaleBtn;
    Button* controlOpacityBtn;
    Button* splitControlsBtn;

    // Bottom action
    Button* doneBtn;

    float getDeviceTemperature();

    // Scroll state for cards
    float scrollY;
    float maxScroll;
    bool isDragging;
    int dragStartY;
    float dragStartScroll;

    void updateButtonPositions();
    void updateButtonVisibility();
    void updateButtonLabels();
    void renderCard(int x, int y, int w, int h, std::shared_ptr<ItemInstance> icon,
                    float iconScale, const std::string& title, const std::string& desc);

public:
    LunarModsScreen(Screen* lastScreen);
    virtual ~LunarModsScreen() = default;

    virtual void init() override;
    virtual void buttonClicked(Button* button) override;
    virtual void render(int xm, int ym, float a) override;
};
