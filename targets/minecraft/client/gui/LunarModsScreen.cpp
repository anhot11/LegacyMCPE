#include "LunarModsScreen.h"

#include "minecraft/client/Minecraft.h"
#include "minecraft/client/Options.h"
#include "minecraft/client/gui/Button.h"
#include "minecraft/client/gui/Font.h"
#include "minecraft/client/renderer/LevelRenderer.h"
#include "minecraft/client/renderer/Textures.h"
#include "platform/renderer/renderer.h"

LunarModsScreen::LunarModsScreen(Screen* lastScreen)
    : lastScreen(lastScreen),
      currentTab(0),
      tabSodiumBtn(nullptr),
      tabOptifineBtn(nullptr),
      tabFullbrightBtn(nullptr),
      tabShadersBtn(nullptr),
      sodiumChunkEngineBtn(nullptr),
      sodiumEntityCullingBtn(nullptr),
      sodiumFogOcclusionBtn(nullptr),
      optifineFastMathBtn(nullptr),
      optifineDynamicLightsBtn(nullptr),
      optifineClearWaterBtn(nullptr),
      optifineBetterGrassBtn(nullptr),
      fullbrightToggleBtn(nullptr),
      shaderPresetBtn(nullptr),
      texturePackBtn(nullptr),
      doneBtn(nullptr) {}

void LunarModsScreen::init() {
    buttons.clear();

    int tabW = 95;
    if (width < 450) tabW = 80;
    int tabH = 20;
    int tabY = 32;
    int startTabX = width / 2 - (tabW * 4 + 12) / 2;

    // Tabs: IDs 10 to 13
    tabSodiumBtn = new Button(10, startTabX, tabY, tabW, tabH, "\xC2\xA7" "eSODIUM");
    tabOptifineBtn = new Button(11, startTabX + tabW + 4, tabY, tabW, tabH, "\xC2\xA7" "6OPTIFINE");
    tabFullbrightBtn = new Button(12, startTabX + (tabW + 4) * 2, tabY, tabW, tabH, "\xC2\xA7" "eFULLBRIGHT");
    tabShadersBtn = new Button(13, startTabX + (tabW + 4) * 3, tabY, tabW, tabH, "\xC2\xA7" "bSHADERS");

    buttons.push_back(tabSodiumBtn);
    buttons.push_back(tabOptifineBtn);
    buttons.push_back(tabFullbrightBtn);
    buttons.push_back(tabShadersBtn);

    int cardW = (width >= 560) ? 460 : (width - 40);
    int cardH = 34;
    int cardX = width / 2 - cardW / 2;
    int startY = 62;
    int spacing = 38;

    int btnW = 140;
    int btnH = 24;
    int btnX = cardX + cardW - btnW - 8;

    // Tab 0: Sodium (IDs 20, 21, 22)
    sodiumChunkEngineBtn = new Button(20, btnX, startY + 5, btnW, btnH, "");
    sodiumEntityCullingBtn = new Button(21, btnX, startY + spacing + 5, btnW, btnH, "");
    sodiumFogOcclusionBtn = new Button(22, btnX, startY + spacing * 2 + 5, btnW, btnH, "");
    buttons.push_back(sodiumChunkEngineBtn);
    buttons.push_back(sodiumEntityCullingBtn);
    buttons.push_back(sodiumFogOcclusionBtn);

    // Tab 1: OptiFine (IDs 30, 31, 32, 33)
    optifineDynamicLightsBtn = new Button(30, btnX, startY + 5, btnW, btnH, "");
    optifineFastMathBtn = new Button(31, btnX, startY + spacing + 5, btnW, btnH, "");
    optifineClearWaterBtn = new Button(32, btnX, startY + spacing * 2 + 5, btnW, btnH, "");
    optifineBetterGrassBtn = new Button(33, btnX, startY + spacing * 3 + 5, btnW, btnH, "");
    buttons.push_back(optifineDynamicLightsBtn);
    buttons.push_back(optifineFastMathBtn);
    buttons.push_back(optifineClearWaterBtn);
    buttons.push_back(optifineBetterGrassBtn);

    // Tab 2: Fullbright (ID 40)
    int fbBtnW = (cardW > 300) ? 240 : (cardW - 20);
    int fbBtnX = width / 2 - fbBtnW / 2;
    fullbrightToggleBtn = new Button(40, fbBtnX, startY + 50, fbBtnW, 30, "");
    buttons.push_back(fullbrightToggleBtn);

    // Tab 3: Shaders & Textures (IDs 50, 51)
    shaderPresetBtn = new Button(50, btnX, startY + 5, btnW, btnH, "");
    texturePackBtn = new Button(51, btnX, startY + spacing + 5, btnW, btnH, "");
    buttons.push_back(shaderPresetBtn);
    buttons.push_back(texturePackBtn);

    // Done button (ID 200)
    int doneW = (width >= 400) ? 240 : 180;
    doneBtn = new Button(200, width / 2 - doneW / 2, height - 34, doneW, 26, "GUARDAR Y VOLVER");
    buttons.push_back(doneBtn);

    updateButtonLabels();
    updateButtonVisibility();
}

void LunarModsScreen::updateButtonVisibility() {
    sodiumChunkEngineBtn->visible = (currentTab == 0);
    sodiumEntityCullingBtn->visible = (currentTab == 0);
    sodiumFogOcclusionBtn->visible = (currentTab == 0);

    optifineDynamicLightsBtn->visible = (currentTab == 1);
    optifineFastMathBtn->visible = (currentTab == 1);
    optifineClearWaterBtn->visible = (currentTab == 1);
    optifineBetterGrassBtn->visible = (currentTab == 1);

    fullbrightToggleBtn->visible = (currentTab == 2);

    shaderPresetBtn->visible = (currentTab == 3);
    texturePackBtn->visible = (currentTab == 3);
}

void LunarModsScreen::updateButtonLabels() {
    Options* opt = minecraft->options;

    // Sodium labels
    sodiumChunkEngineBtn->msg = opt->modSodiumChunkEngine ? "\xC2\xA7" "a[ ACTIVADO ]" : "\xC2\xA7" "c[ DESACTIVADO ]";
    sodiumEntityCullingBtn->msg = opt->modSodiumEntityCulling ? "\xC2\xA7" "a[ ACTIVADO ]" : "\xC2\xA7" "c[ DESACTIVADO ]";
    sodiumFogOcclusionBtn->msg = opt->modSodiumFogOcclusion ? "\xC2\xA7" "a[ ACTIVADO ]" : "\xC2\xA7" "c[ DESACTIVADO ]";

    // OptiFine labels
    optifineDynamicLightsBtn->msg = opt->modOptifineDynamicLights ? "\xC2\xA7" "a[ ACTIVADO ]" : "\xC2\xA7" "c[ DESACTIVADO ]";
    optifineFastMathBtn->msg = opt->modOptifineFastMath ? "\xC2\xA7" "a[ ACTIVADO ]" : "\xC2\xA7" "c[ DESACTIVADO ]";
    optifineClearWaterBtn->msg = opt->modOptifineClearWater ? "\xC2\xA7" "a[ ACTIVADO ]" : "\xC2\xA7" "c[ DESACTIVADO ]";
    optifineBetterGrassBtn->msg = opt->modOptifineBetterGrass ? "\xC2\xA7" "a[ ACTIVADO ]" : "\xC2\xA7" "c[ DESACTIVADO ]";

    // Fullbright label
    fullbrightToggleBtn->msg = opt->modFullbright ? "\xC2\xA7" "aBRILLO SIEMPRE: ENCENDIDO" : "\xC2\xA7" "cBRILLO SIEMPRE: APAGADO";

    // Shaders label
    static const char* s_shaderNames[] = {
        "Original Vanilla",
        "Colores Vivos",
        "Cel-Shaded Comic",
        "Vision Nocturna",
        "Atardecer Calido"
    };
    int preset = opt->modShaderPreset;
    if (preset < 0 || preset > 4) preset = 0;
    shaderPresetBtn->msg = s_shaderNames[preset];

    // Texture Pack label
    static const char* s_textureNames[] = {
        "Default 16x",
        "Faithful 32x",
        "Bare Bones"
    };
    int pack = opt->modTexturePack;
    if (pack < 0 || pack > 2) pack = 0;
    texturePackBtn->msg = s_textureNames[pack];
}

void LunarModsScreen::buttonClicked(Button* button) {
    if (!button->active) return;
    Options* opt = minecraft->options;

    // Tab buttons
    if (button->id >= 10 && button->id <= 13) {
        currentTab = button->id - 10;
        updateButtonVisibility();
        return;
    }

    // Sodium toggles
    if (button->id == 20) {
        opt->modSodiumChunkEngine = !opt->modSodiumChunkEngine;
    } else if (button->id == 21) {
        opt->modSodiumEntityCulling = !opt->modSodiumEntityCulling;
    } else if (button->id == 22) {
        opt->modSodiumFogOcclusion = !opt->modSodiumFogOcclusion;
    }

    // OptiFine toggles
    else if (button->id == 30) {
        opt->modOptifineDynamicLights = !opt->modOptifineDynamicLights;
    } else if (button->id == 31) {
        opt->modOptifineFastMath = !opt->modOptifineFastMath;
    } else if (button->id == 32) {
        opt->modOptifineClearWater = !opt->modOptifineClearWater;
    } else if (button->id == 33) {
        opt->modOptifineBetterGrass = !opt->modOptifineBetterGrass;
    }

    // Fullbright toggle
    else if (button->id == 40) {
        opt->modFullbright = !opt->modFullbright;
        if (opt->modFullbright) {
            opt->gamma = 1.0f;
        }
    }

    // Shaders & Textures cycle
    else if (button->id == 50) {
        opt->modShaderPreset = (opt->modShaderPreset + 1) % 5;
        PlatformRenderer_SetShaderPreset(opt->modShaderPreset);
    } else if (button->id == 51) {
        opt->modTexturePack = (opt->modTexturePack + 1) % 3;
    }

    // Save and Exit
    else if (button->id == 200) {
        opt->save();
        if (minecraft->level) {
            minecraft->levelRenderer->allChanged();
        }
        minecraft->setScreen(lastScreen);
        return;
    }

    opt->save();
    updateButtonLabels();
}

void LunarModsScreen::render(int xm, int ym, float a) {
    fillGradient(0, 0, width, height, 0xf00b0e14, 0xf8111622);

    // Top Header Bar
    fill(0, 0, width, 56, 0xdd080b10);
    hLine(0, width, 56, 0xff253042);

    drawCenteredString(font, "\xC2\xA7" "b\xC2\xA7" "lLUNAR CLIENT \xC2\xA7" "7| \xC2\xA7" "fMODS & OPTIMIZACIONES", width / 2, 8, 0xffffff);
    drawCenteredString(font, "\xC2\xA7" "8Motor Sodium & OptiFine Nativo para Minecraft PE", width / 2, 20, 0x999999);

    // Draw active tab indicator
    int tabW = 95;
    if (width < 450) tabW = 80;
    int startTabX = width / 2 - (tabW * 4 + 12) / 2;
    int curTabX = startTabX + currentTab * (tabW + 4);
    fill(curTabX, 52, curTabX + tabW, 55, 0xff38bdf8); // Sky blue accent line

    int cardW = (width >= 560) ? 460 : (width - 40);
    int cardH = 34;
    int cardX = width / 2 - cardW / 2;
    int startY = 62;
    int spacing = 38;

    // Render Tab Content Cards
    if (currentTab == 0) {
        // Tab 0: Sodium Cards
        fill(cardX, startY, cardX + cardW, startY + cardH, 0xc0141923);
        hLine(cardX, cardX + cardW, startY, 0xff263345);
        drawString(font, "\xC2\xA7" "f\xC2\xA7" "lSodium Chunk Engine", cardX + 10, startY + 6, 0xffffff);
        drawString(font, "\xC2\xA7" "7Malla multihilo y cache de chunks sin tirones", cardX + 10, startY + 18, 0x8899aa);

        fill(cardX, startY + spacing, cardX + cardW, startY + spacing + cardH, 0xc0141923);
        hLine(cardX, cardX + cardW, startY + spacing, 0xff263345);
        drawString(font, "\xC2\xA7" "f\xC2\xA7" "lEntity Culling (+FPS)", cardX + 10, startY + spacing + 6, 0xffffff);
        drawString(font, "\xC2\xA7" "7Oculta entidades detras de bloques y fuera de campo", cardX + 10, startY + spacing + 18, 0x8899aa);

        fill(cardX, startY + spacing * 2, cardX + cardW, startY + spacing * 2 + cardH, 0xc0141923);
        hLine(cardX, cardX + cardW, startY + spacing * 2, 0xff263345);
        drawString(font, "\xC2\xA7" "f\xC2\xA7" "lFog Occlusion", cardX + 10, startY + spacing * 2 + 6, 0xffffff);
        drawString(font, "\xC2\xA7" "7Descarta calculos de geometria oculta por la niebla", cardX + 10, startY + spacing * 2 + 18, 0x8899aa);
    } else if (currentTab == 1) {
        // Tab 1: OptiFine Cards
        fill(cardX, startY, cardX + cardW, startY + cardH, 0xc0141923);
        hLine(cardX, cardX + cardW, startY, 0xff263345);
        drawString(font, "\xC2\xA7" "f\xC2\xA7" "lDynamic Lights", cardX + 10, startY + 6, 0xffffff);
        drawString(font, "\xC2\xA7" "7Antorcha e items emiten luz dinamica en mano", cardX + 10, startY + 18, 0x8899aa);

        fill(cardX, startY + spacing, cardX + cardW, startY + spacing + cardH, 0xc0141923);
        hLine(cardX, cardX + cardW, startY + spacing, 0xff263345);
        drawString(font, "\xC2\xA7" "f\xC2\xA7" "lFast Math", cardX + 10, startY + spacing + 6, 0xffffff);
        drawString(font, "\xC2\xA7" "7Trigonometria acelerada por CPU sin fmodf (+15% FPS)", cardX + 10, startY + spacing + 18, 0x8899aa);

        fill(cardX, startY + spacing * 2, cardX + cardW, startY + spacing * 2 + cardH, 0xc0141923);
        hLine(cardX, cardX + cardW, startY + spacing * 2, 0xff263345);
        drawString(font, "\xC2\xA7" "f\xC2\xA7" "lClear Water", cardX + 10, startY + spacing * 2 + 6, 0xffffff);
        drawString(font, "\xC2\xA7" "7Agua cristalina y sin niebla espesa bajo el agua", cardX + 10, startY + spacing * 2 + 18, 0x8899aa);

        fill(cardX, startY + spacing * 3, cardX + cardW, startY + spacing * 3 + cardH, 0xc0141923);
        hLine(cardX, cardX + cardW, startY + spacing * 3, 0xff263345);
        drawString(font, "\xC2\xA7" "f\xC2\xA7" "lBetter Grass", cardX + 10, startY + spacing * 3 + 6, 0xffffff);
        drawString(font, "\xC2\xA7" "7Pasto completo conectado en los bordes", cardX + 10, startY + spacing * 3 + 18, 0x8899aa);
    } else if (currentTab == 2) {
        // Tab 2: Fullbright Card
        int fbCardH = 100;
        fill(cardX, startY, cardX + cardW, startY + fbCardH, 0xc0141923);
        hLine(cardX, cardX + cardW, startY, 0xff263345);
        drawCenteredString(font, "\xC2\xA7" "e\xC2\xA7" "lMOD BRILLO SIEMPRE (FULLBRIGHT)", width / 2, startY + 12, 0xffffff);
        drawCenteredString(font, "\xC2\xA7" "7Mantiene la iluminacion ambiental al 100% de dia y noche.", width / 2, startY + 26, 0xaabbcc);
        drawCenteredString(font, "\xC2\xA7" "8Permite ver dentro de cuevas oscuras y minerias sin colocar antorchas.", width / 2, startY + 38, 0x8899aa);
    } else if (currentTab == 3) {
        // Tab 3: Shaders & Textures Cards
        fill(cardX, startY, cardX + cardW, startY + cardH, 0xc0141923);
        hLine(cardX, cardX + cardW, startY, 0xff263345);
        drawString(font, "\xC2\xA7" "b\xC2\xA7" "lShader Preset (GLSL)", cardX + 10, startY + 6, 0xffffff);
        drawString(font, "\xC2\xA7" "7Post-procesado de color e iluminacion en GPU", cardX + 10, startY + 18, 0x8899aa);

        fill(cardX, startY + spacing, cardX + cardW, startY + spacing + cardH, 0xc0141923);
        hLine(cardX, cardX + cardW, startY + spacing, 0xff263345);
        drawString(font, "\xC2\xA7" "6\xC2\xA7" "lTexture Pack Integrado", cardX + 10, startY + spacing + 6, 0xffffff);
        drawString(font, "\xC2\xA7" "7Paquete visual seleccionado", cardX + 10, startY + spacing + 18, 0x8899aa);
    }

    // Bottom bar divider
    hLine(0, width, height - 42, 0xff253042);
    fill(0, height - 42, width, height, 0xdd080b10);

    Screen::render(xm, ym, a);
}
