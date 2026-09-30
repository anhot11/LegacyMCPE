#include "LunarModsScreen.h"

#include "minecraft/client/Lighting.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/Options.h"
#include "minecraft/client/gui/Button.h"
#include "minecraft/client/gui/Font.h"
#include "minecraft/client/renderer/LevelRenderer.h"
#include "minecraft/client/renderer/Textures.h"
#include "minecraft/client/renderer/entity/ItemRenderer.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/tile/Tile.h"
#include "platform/renderer/renderer.h"

ItemRenderer* LunarModsScreen::itemRenderer = nullptr;

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
      doneBtn(nullptr) {
    if (!itemRenderer) {
        itemRenderer = new ItemRenderer();
    }
}

void LunarModsScreen::init() {
    buttons.clear();

    if (!itemRenderer) {
        itemRenderer = new ItemRenderer();
    }

    // Initialize large Minecraft item icons for each mod card
    if (!iconSodiumEngine) iconSodiumEngine = std::make_shared<ItemInstance>(Item::repeater);
    if (!iconEntityCulling) iconEntityCulling = std::make_shared<ItemInstance>(Item::eyeOfEnder);
    if (!iconFogOcclusion) iconFogOcclusion = std::make_shared<ItemInstance>(Item::netherStar);

    if (!iconDynamicLights) iconDynamicLights = std::make_shared<ItemInstance>(Tile::torch);
    if (!iconFastMath) iconFastMath = std::make_shared<ItemInstance>(Item::compass);
    if (!iconClearWater) iconClearWater = std::make_shared<ItemInstance>(Tile::glass);
    if (!iconBetterGrass) iconBetterGrass = std::make_shared<ItemInstance>(Tile::grass);

    if (!iconFullbright) iconFullbright = std::make_shared<ItemInstance>(Tile::glowstone);
    if (!iconShaders) iconShaders = std::make_shared<ItemInstance>(Item::painting);
    if (!iconTexturePack) iconTexturePack = std::make_shared<ItemInstance>(Tile::workBench);
    if (!iconLunarStar) iconLunarStar = std::make_shared<ItemInstance>(Item::netherStar);

    int tabW = 95;
    if (width < 450) tabW = 80;
    int tabH = 20;
    int tabY = 32;
    int startTabX = width / 2 - (tabW * 4 + 12) / 2;

    // Tabs: IDs 10 to 13
    tabSodiumBtn = new Button(10, startTabX, tabY, tabW, tabH, "SODIUM");
    tabOptifineBtn = new Button(11, startTabX + tabW + 4, tabY, tabW, tabH, "OPTIFINE");
    tabFullbrightBtn = new Button(12, startTabX + (tabW + 4) * 2, tabY, tabW, tabH, "FULLBRIGHT");
    tabShadersBtn = new Button(13, startTabX + (tabW + 4) * 3, tabY, tabW, tabH, "SHADERS");

    buttons.push_back(tabSodiumBtn);
    buttons.push_back(tabOptifineBtn);
    buttons.push_back(tabFullbrightBtn);
    buttons.push_back(tabShadersBtn);

    int cardW = (width >= 560) ? 480 : (width - 30);
    int cardX = width / 2 - cardW / 2;
    int startY = 60;
    int spacing = 43;

    int btnW = 125;
    int btnH = 26;
    int btnX = cardX + cardW - btnW - 6;

    // Tab 0: Sodium (IDs 20, 21, 22)
    sodiumChunkEngineBtn = new Button(20, btnX, startY + 6, btnW, btnH, "");
    sodiumEntityCullingBtn = new Button(21, btnX, startY + spacing + 6, btnW, btnH, "");
    sodiumFogOcclusionBtn = new Button(22, btnX, startY + spacing * 2 + 6, btnW, btnH, "");
    buttons.push_back(sodiumChunkEngineBtn);
    buttons.push_back(sodiumEntityCullingBtn);
    buttons.push_back(sodiumFogOcclusionBtn);

    // Tab 1: OptiFine (IDs 30, 31, 32, 33)
    optifineDynamicLightsBtn = new Button(30, btnX, startY + 6, btnW, btnH, "");
    optifineFastMathBtn = new Button(31, btnX, startY + spacing + 6, btnW, btnH, "");
    optifineClearWaterBtn = new Button(32, btnX, startY + spacing * 2 + 6, btnW, btnH, "");
    optifineBetterGrassBtn = new Button(33, btnX, startY + spacing * 3 + 6, btnW, btnH, "");
    buttons.push_back(optifineDynamicLightsBtn);
    buttons.push_back(optifineFastMathBtn);
    buttons.push_back(optifineClearWaterBtn);
    buttons.push_back(optifineBetterGrassBtn);

    // Tab 2: Fullbright (ID 40)
    int fbBtnW = (cardW > 300) ? 260 : (cardW - 20);
    int fbBtnX = width / 2 - fbBtnW / 2;
    fullbrightToggleBtn = new Button(40, fbBtnX, startY + 95, fbBtnW, 30, "");
    buttons.push_back(fullbrightToggleBtn);

    // Tab 3: Shaders & Textures (IDs 50, 51)
    shaderPresetBtn = new Button(50, btnX, startY + 6, btnW, btnH, "");
    texturePackBtn = new Button(51, btnX, startY + spacing + 6, btnW, btnH, "");
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
    sodiumChunkEngineBtn->msg = opt->modSodiumChunkEngine ? "[ ACTIVADO ]" : "[ DESACTIVADO ]";
    sodiumEntityCullingBtn->msg = opt->modSodiumEntityCulling ? "[ ACTIVADO ]" : "[ DESACTIVADO ]";
    sodiumFogOcclusionBtn->msg = opt->modSodiumFogOcclusion ? "[ ACTIVADO ]" : "[ DESACTIVADO ]";

    // OptiFine labels
    optifineDynamicLightsBtn->msg = opt->modOptifineDynamicLights ? "[ ACTIVADO ]" : "[ DESACTIVADO ]";
    optifineFastMathBtn->msg = opt->modOptifineFastMath ? "[ ACTIVADO ]" : "[ DESACTIVADO ]";
    optifineClearWaterBtn->msg = opt->modOptifineClearWater ? "[ ACTIVADO ]" : "[ DESACTIVADO ]";
    optifineBetterGrassBtn->msg = opt->modOptifineBetterGrass ? "[ ACTIVADO ]" : "[ DESACTIVADO ]";

    // Fullbright label
    fullbrightToggleBtn->msg = opt->modFullbright ? "BRILLO SIEMPRE: ENCENDIDO" : "BRILLO SIEMPRE: APAGADO";

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

void LunarModsScreen::renderCard(int x, int y, int w, int h, std::shared_ptr<ItemInstance> icon,
                                 float iconScale, const std::string& title, const std::string& desc) {
    // Card background: Dark Lunar slate with highlight borders
    fill(x, y, x + w, y + h, 0xd0101522);
    hLine(x, x + w, y, 0xff253347);
    vLine(x, y, y + h, 0xff202c3e);
    vLine(x + w, y, y + h, 0xff141c27);
    hLine(x, x + w, y + h, 0xff141c27);

    // Left Icon Badge Frame
    int boxSize = 30;
    int boxX = x + 4;
    int boxY = y + (h - boxSize) / 2;
    fill(boxX, boxY, boxX + boxSize, boxY + boxSize, 0xf00a0d14);
    hLine(boxX, boxX + boxSize, boxY, 0xff2c3d55);
    vLine(boxX, boxY, boxY + boxSize, 0xff2c3d55);
    vLine(boxX + boxSize, boxY, boxY + boxSize, 0xff16202c);
    hLine(boxX, boxX + boxSize, boxY + boxSize, 0xff16202c);

    // Render large 3D/2D Minecraft item icon
    if (icon && itemRenderer) {
        glEnable(GL_RESCALE_NORMAL);
        glEnable(GL_COLOR_MATERIAL);
        Lighting::turnOnGui();

        float iconPixelSize = 16.0f * iconScale;
        float iconOffX = (float)boxX + ((float)boxSize - iconPixelSize) / 2.0f;
        float iconOffY = (float)boxY + ((float)boxSize - iconPixelSize) / 2.0f;
        itemRenderer->renderGuiItem(font, minecraft->textures, icon, iconOffX, iconOffY, iconScale, 1.0f);

        Lighting::turnOff();
        glDisable(GL_RESCALE_NORMAL);
        glDisable(GL_COLOR_MATERIAL);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }

    // Clean, crisp typography without any formatting codes
    drawString(font, title, boxX + boxSize + 8, y + 7, 0xffffff);
    drawString(font, desc, boxX + boxSize + 8, y + 21, 0x889fa5);
}

void LunarModsScreen::render(int xm, int ym, float a) {
    fillGradient(0, 0, width, height, 0xf00b0e14, 0xf8111622);

    // Top Header Bar
    fill(0, 0, width, 56, 0xdd080b10);
    hLine(0, width, 56, 0xff253042);

    // Header Star Icon
    if (iconLunarStar && itemRenderer) {
        glEnable(GL_RESCALE_NORMAL);
        glEnable(GL_COLOR_MATERIAL);
        Lighting::turnOnGui();
        int starX = width / 2 - 145;
        if (width < 450) starX = width / 2 - 120;
        itemRenderer->renderGuiItem(font, minecraft->textures, iconLunarStar, (float)starX, 5.0f, 1.3f, 1.0f);
        Lighting::turnOff();
        glDisable(GL_RESCALE_NORMAL);
        glDisable(GL_COLOR_MATERIAL);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }

    drawCenteredString(font, "LUNAR CLIENT | MODS & OPTIMIZACIONES", width / 2 + 6, 8, 0xffffff);
    drawCenteredString(font, "Motor Sodium & OptiFine Nativo para Minecraft PE", width / 2, 20, 0x999999);

    // Active tab indicator
    int tabW = 95;
    if (width < 450) tabW = 80;
    int startTabX = width / 2 - (tabW * 4 + 12) / 2;
    int curTabX = startTabX + currentTab * (tabW + 4);
    fill(curTabX, 52, curTabX + tabW, 55, 0xff38bdf8); // Lunar Sky-Blue accent line

    int cardW = (width >= 560) ? 480 : (width - 30);
    int cardH = 38;
    int cardX = width / 2 - cardW / 2;
    int startY = 60;
    int spacing = 43;

    if (currentTab == 0) {
        // Tab 0: Sodium Cards with large icons
        renderCard(cardX, startY, cardW, cardH, iconSodiumEngine, 1.6f,
                   "Sodium Chunk Engine", "Malla multihilo y cache de chunks sin tirones");
        renderCard(cardX, startY + spacing, cardW, cardH, iconEntityCulling, 1.6f,
                   "Entity Culling (+FPS)", "Oculta entidades fuera de campo y tras bloques");
        renderCard(cardX, startY + spacing * 2, cardW, cardH, iconFogOcclusion, 1.6f,
                   "Fog Occlusion", "Descarta calculos de geometria oculta por niebla");
    } else if (currentTab == 1) {
        // Tab 1: OptiFine Cards with large icons
        renderCard(cardX, startY, cardW, cardH, iconDynamicLights, 1.6f,
                   "Dynamic Lights", "Antorcha e items emiten luz dinamica en mano");
        renderCard(cardX, startY + spacing, cardW, cardH, iconFastMath, 1.6f,
                   "Fast Math", "Trigonometria CPU optimizada (+15% FPS)");
        renderCard(cardX, startY + spacing * 2, cardW, cardH, iconClearWater, 1.6f,
                   "Clear Water", "Agua cristalina y sin niebla espesa marina");
        renderCard(cardX, startY + spacing * 3, cardW, cardH, iconBetterGrass, 1.6f,
                   "Better Grass", "Pasto completo conectado en los bordes");
    } else if (currentTab == 2) {
        // Tab 2: Fullbright Card with large glowstone icon
        int fbCardH = 145;
        fill(cardX, startY, cardX + cardW, startY + fbCardH, 0xd0101522);
        hLine(cardX, cardX + cardW, startY, 0xff253347);
        vLine(cardX, startY, startY + fbCardH, 0xff202c3e);
        vLine(cardX + cardW, startY, startY + fbCardH, 0xff141c27);
        hLine(cardX, cardX + cardW, startY + fbCardH, 0xff141c27);

        // Center Large Glowstone Icon (2.2x scale)
        if (iconFullbright && itemRenderer) {
            glEnable(GL_RESCALE_NORMAL);
            glEnable(GL_COLOR_MATERIAL);
            Lighting::turnOnGui();
            float fbIconX = (float)width / 2.0f - (16.0f * 2.2f) / 2.0f;
            itemRenderer->renderGuiItem(font, minecraft->textures, iconFullbright, fbIconX, (float)(startY + 10), 2.2f, 1.0f);
            Lighting::turnOff();
            glDisable(GL_RESCALE_NORMAL);
            glDisable(GL_COLOR_MATERIAL);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }

        drawCenteredString(font, "MOD BRILLO SIEMPRE (FULLBRIGHT)", width / 2, startY + 52, 0xffea00);
        drawCenteredString(font, "Mantiene la iluminacion ambiental al 100% de forma permanente.", width / 2, startY + 66, 0xaabbcc);
        drawCenteredString(font, "Permite explorar cuevas oscuras y minerias sin colocar antorchas.", width / 2, startY + 78, 0x8899aa);
    } else if (currentTab == 3) {
        // Tab 3: Shaders & Textures Cards with large icons
        renderCard(cardX, startY, cardW, cardH, iconShaders, 1.6f,
                   "Shader Preset (GLSL)", "Post-procesado de color e iluminacion en GPU");
        renderCard(cardX, startY + spacing, cardW, cardH, iconTexturePack, 1.6f,
                   "Texture Pack Integrado", "Paquete visual seleccionado en tiempo real");
    }

    // Bottom bar divider
    hLine(0, width, height - 42, 0xff253042);
    fill(0, height - 42, width, height, 0xdd080b10);

    Screen::render(xm, ym, a);
}
