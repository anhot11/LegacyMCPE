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
#include "minecraft/world/level/tile/GrassTile.h"
#include "minecraft/util/Mth.h"
#include "platform/renderer/renderer.h"
#include "platform/input/input.h"

ItemRenderer* LunarModsScreen::itemRenderer = nullptr;

LunarModsScreen::LunarModsScreen(Screen* lastScreen)
    : lastScreen(lastScreen),
      currentTab(0),
      scrollY(0.0f),
      maxScroll(0.0f),
      isDragging(false),
      dragStartY(0),
      dragStartScroll(0.0f),
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
      tabThermalBtn(nullptr),
      thermalProtectionBtn(nullptr),
      splitControlsBtn(nullptr),
      doneBtn(nullptr) {
    if (!itemRenderer) {
        itemRenderer = new ItemRenderer();
    }
}

float LunarModsScreen::getDeviceTemperature() {
    FILE* f = fopen("/sys/class/power_supply/battery/temp", "r");
    if (!f) return 36.5f;
    int raw = 0;
    if (fscanf(f, "%d", &raw) == 1) {
        fclose(f);
        if (raw > 1000) return (float)raw / 1000.0f;
        if (raw > 100) return (float)raw / 10.0f;
        return (float)raw;
    }
    fclose(f);
    return 36.5f;
}

void LunarModsScreen::init() {
    buttons.clear();

    if (!itemRenderer) {
        itemRenderer = new ItemRenderer();
    }

    // Initialize large Minecraft item icons for each mod card
    if (!iconSodiumEngine) iconSodiumEngine = std::shared_ptr<ItemInstance>(new ItemInstance(Item::repeater));
    if (!iconEntityCulling) iconEntityCulling = std::shared_ptr<ItemInstance>(new ItemInstance(Item::eyeOfEnder));
    if (!iconFogOcclusion) iconFogOcclusion = std::shared_ptr<ItemInstance>(new ItemInstance(Item::netherStar));

    if (!iconDynamicLights) iconDynamicLights = std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::torch));
    if (!iconFastMath) iconFastMath = std::shared_ptr<ItemInstance>(new ItemInstance(Item::compass));
    if (!iconClearWater) iconClearWater = std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::glass));
    if (!iconBetterGrass) iconBetterGrass = std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::grass));

    if (!iconFullbright) iconFullbright = std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::glowstone));
    if (!iconShaders) iconShaders = std::shared_ptr<ItemInstance>(new ItemInstance(Item::painting));
    if (!iconTexturePack) iconTexturePack = std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::workBench));
    if (!iconLunarStar) iconLunarStar = std::shared_ptr<ItemInstance>(new ItemInstance(Item::netherStar));
    if (!iconThermal) iconThermal = std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::ice));
    if (!iconControls) iconControls = std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::lever));

    int tabW = 66;
    if (width < 450) tabW = 54;
    int tabH = 20;
    int tabY = 32;
    int startTabX = width / 2 - (tabW * 6 + 20) / 2;

    // Tabs: IDs 10 to 15
    tabSodiumBtn = new Button(10, startTabX, tabY, tabW, tabH, "SODIUM");
    tabOptifineBtn = new Button(11, startTabX + (tabW + 4) * 1, tabY, tabW, tabH, "OPTIFINE");
    tabFullbrightBtn = new Button(12, startTabX + (tabW + 4) * 2, tabY, tabW, tabH, "FULLBRIGHT");
    tabShadersBtn = new Button(13, startTabX + (tabW + 4) * 3, tabY, tabW, tabH, "SHADERS");
    tabThermalBtn = new Button(14, startTabX + (tabW + 4) * 4, tabY, tabW, tabH, "TERMAL");
    tabControlsBtn = new Button(15, startTabX + (tabW + 4) * 5, tabY, tabW, tabH, "CONTROLES");

    buttons.push_back(tabSodiumBtn);
    buttons.push_back(tabOptifineBtn);
    buttons.push_back(tabFullbrightBtn);
    buttons.push_back(tabShadersBtn);
    buttons.push_back(tabThermalBtn);
    buttons.push_back(tabControlsBtn);

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

    // Tab 4: Thermal Protection (ID 60)
    int thBtnW = (cardW > 300) ? 270 : (cardW - 20);
    int thBtnX = width / 2 - thBtnW / 2;
    thermalProtectionBtn = new Button(60, thBtnX, startY + 95, thBtnW, 30, "");
    buttons.push_back(thermalProtectionBtn);

    // Tab 5: Controls settings (IDs 70, 71, 72, 73)
    controlStyleBtn = new Button(70, btnX, startY + 6, btnW, btnH, "");
    controlScaleBtn = new Button(71, btnX, startY + spacing + 6, btnW, btnH, "");
    controlOpacityBtn = new Button(72, btnX, startY + spacing * 2 + 6, btnW, btnH, "");
    splitControlsBtn = new Button(73, btnX, startY + spacing * 3 + 6, btnW, btnH, "");
    buttons.push_back(controlStyleBtn);
    buttons.push_back(controlScaleBtn);
    buttons.push_back(controlOpacityBtn);
    buttons.push_back(splitControlsBtn);

    // Done button (ID 200)
    int doneW = (width >= 400) ? 240 : 180;
    doneBtn = new Button(200, width / 2 - doneW / 2, height - 34, doneW, 26, "GUARDAR Y VOLVER");
    buttons.push_back(doneBtn);

    updateButtonLabels();
    updateButtonVisibility();
}

void LunarModsScreen::updateButtonPositions() {
    int cardW = (width >= 560) ? 480 : (width - 30);
    int cardX = width / 2 - cardW / 2;
    int startY = 60 - (int)scrollY;
    int spacing = 43;

    int btnW = 125;
    int btnH = 26;
    int btnX = cardX + cardW - btnW - 6;

    // Tab 0: Sodium
    if (sodiumChunkEngineBtn) {
        sodiumChunkEngineBtn->x = btnX;
        sodiumChunkEngineBtn->y = startY + 6;
    }
    if (sodiumEntityCullingBtn) {
        sodiumEntityCullingBtn->x = btnX;
        sodiumEntityCullingBtn->y = startY + spacing + 6;
    }
    if (sodiumFogOcclusionBtn) {
        sodiumFogOcclusionBtn->x = btnX;
        sodiumFogOcclusionBtn->y = startY + spacing * 2 + 6;
    }

    // Tab 1: OptiFine
    if (optifineDynamicLightsBtn) {
        optifineDynamicLightsBtn->x = btnX;
        optifineDynamicLightsBtn->y = startY + 6;
    }
    if (optifineFastMathBtn) {
        optifineFastMathBtn->x = btnX;
        optifineFastMathBtn->y = startY + spacing + 6;
    }
    if (optifineClearWaterBtn) {
        optifineClearWaterBtn->x = btnX;
        optifineClearWaterBtn->y = startY + spacing * 2 + 6;
    }
    if (optifineBetterGrassBtn) {
        optifineBetterGrassBtn->x = btnX;
        optifineBetterGrassBtn->y = startY + spacing * 3 + 6;
    }

    // Tab 2: Fullbright
    int fbBtnW = (cardW > 300) ? 260 : (cardW - 20);
    if (fullbrightToggleBtn) {
        fullbrightToggleBtn->x = width / 2 - fbBtnW / 2;
        fullbrightToggleBtn->y = startY + 95;
    }

    // Tab 3: Shaders
    if (shaderPresetBtn) {
        shaderPresetBtn->x = btnX;
        shaderPresetBtn->y = startY + 6;
    }
    if (texturePackBtn) {
        texturePackBtn->x = btnX;
        texturePackBtn->y = startY + spacing + 6;
    }

    // Tab 4: Thermal
    int thBtnW = (cardW > 300) ? 270 : (cardW - 20);
    if (thermalProtectionBtn) {
        thermalProtectionBtn->x = width / 2 - thBtnW / 2;
        thermalProtectionBtn->y = startY + 95;
    }

    // Tab 5: Controls
    if (controlStyleBtn) {
        controlStyleBtn->x = btnX;
        controlStyleBtn->y = startY + 6;
    }
    if (controlScaleBtn) {
        controlScaleBtn->x = btnX;
        controlScaleBtn->y = startY + spacing + 6;
    }
    if (controlOpacityBtn) {
        controlOpacityBtn->x = btnX;
        controlOpacityBtn->y = startY + spacing * 2 + 6;
    }
    if (splitControlsBtn) {
        splitControlsBtn->x = btnX;
        splitControlsBtn->y = startY + spacing * 3 + 6;
    }

    // Deactivate buttons scrolled out of the visible vertical viewport
    int topClip = 56;
    int botClip = height - 42;
    for (Button* btn : buttons) {
        if (!btn || btn == doneBtn || (btn->id >= 10 && btn->id <= 15)) continue;
        if (btn->visible) {
            btn->active = (btn->y >= topClip - 4 && btn->y + btn->h <= botClip + 4);
        }
    }
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

    thermalProtectionBtn->visible = (currentTab == 4);

    controlStyleBtn->visible = (currentTab == 5);
    controlScaleBtn->visible = (currentTab == 5);
    controlOpacityBtn->visible = (currentTab == 5);
    splitControlsBtn->visible = (currentTab == 5);

    updateButtonPositions();
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

    // Thermal Protection label
    static const char* s_thermalModes[] = {
        "[ DESACTIVADO ]",
        "[ MODERADO: LIMITE 45*C ]",
        "[ EQUILIBRADO: LIMITE 42*C ]",
        "[ MAXIMO AHORRO: LIMITE 38*C ]"
    };
    int tmode = opt->modThermalProtection;
    if (tmode < 0 || tmode > 3) tmode = 0;
    thermalProtectionBtn->msg = s_thermalModes[tmode];

    // Controls labels
    static const char* s_ctrlStyles[] = {
        "[ MODERNO BEDROCK ]",
        "[ CRUCETA CLASICA PE ]",
        "[ JOYSTICK + ACCION ]"
    };
    int cStyle = opt->touchControlStyle;
    if (cStyle < 0 || cStyle > 2) cStyle = 0;
    controlStyleBtn->msg = s_ctrlStyles[cStyle];

    static const char* s_ctrlScales[] = {
        "[ PEQUENO (80%) ]",
        "[ NORMAL (100%) ]",
        "[ GRANDE (125%) ]",
        "[ EXTRA (150%) ]"
    };
    int cScale = opt->touchControlScale;
    if (cScale < 0 || cScale > 3) cScale = 1;
    controlScaleBtn->msg = s_ctrlScales[cScale];

    static const char* s_ctrlOpacities[] = {
        "[ BAJA (35%) ]",
        "[ MEDIA (65%) ]",
        "[ ALTA (90%) ]",
        "[ SOLIDA (100%) ]"
    };
    int cOpacity = opt->touchControlOpacity;
    if (cOpacity < 0 || cOpacity > 3) cOpacity = 1;
    controlOpacityBtn->msg = s_ctrlOpacities[cOpacity];

    splitControlsBtn->msg = opt->splitControls ? "[ ACTIVADO ]" : "[ DESACTIVADO ]";
}

void LunarModsScreen::buttonClicked(Button* button) {
    if (!button->active) return;
    Options* opt = minecraft->options;

    // Tab buttons
    if (button->id >= 10 && button->id <= 15) {
        currentTab = button->id - 10;
        scrollY = 0.0f;
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
        g_optifineFastMath = opt->modOptifineFastMath;
    } else if (button->id == 32) {
        opt->modOptifineClearWater = !opt->modOptifineClearWater;
    } else if (button->id == 33) {
        opt->modOptifineBetterGrass = !opt->modOptifineBetterGrass;
        if (minecraft && minecraft->levelRenderer) {
            minecraft->levelRenderer->allChanged();
        }
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

    // Thermal Protection toggle
    else if (button->id == 60) {
        opt->modThermalProtection = (opt->modThermalProtection + 1) % 4;
    }

    // Controls settings
    else if (button->id == 70) {
        opt->touchControlStyle = (opt->touchControlStyle + 1) % 3;
    } else if (button->id == 71) {
        opt->touchControlScale = (opt->touchControlScale + 1) % 4;
    } else if (button->id == 72) {
        opt->touchControlOpacity = (opt->touchControlOpacity + 1) % 4;
    } else if (button->id == 73) {
        opt->splitControls = !opt->splitControls;
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

    // Compute content height and max scroll
    int viewportH = (height - 42) - 60;
    int contentH = 0;
    if (currentTab == 0) contentH = 3 * 43 + 30;
    else if (currentTab == 1) contentH = 4 * 43 + 60; // Extra room so Better Grass can be scrolled fully into view
    else if (currentTab == 2) contentH = 145 + 30;
    else if (currentTab == 3) contentH = 2 * 43 + 30;
    else if (currentTab == 4) contentH = 145 + 30;
    else if (currentTab == 5) contentH = 4 * 43 + 40;

    contentH += 16; // Bottom margin breathing room
    maxScroll = (contentH > viewportH) ? (float)(contentH - viewportH) : 0.0f;

    // Touch / drag scroll handling
    bool isDown = PlatformInput.ButtonDown(0, MINECRAFT_ACTION_ACTION);
    if (isDown) {
        if (!isDragging) {
            if (ym >= 56 && ym <= height - 42) {
                isDragging = true;
                dragStartY = ym;
                dragStartScroll = scrollY;
            }
        } else if (maxScroll > 0.0f) {
            int dy = ym - dragStartY;
            scrollY = dragStartScroll - (float)dy;
            if (scrollY < 0.0f) scrollY = 0.0f;
            if (scrollY > maxScroll) scrollY = maxScroll;
        }
    } else {
        isDragging = false;
    }

    if (scrollY > maxScroll) scrollY = maxScroll;
    if (scrollY < 0.0f) scrollY = 0.0f;

    updateButtonPositions();

    int cardW = (width >= 560) ? 480 : (width - 30);
    int cardH = 38;
    int cardX = width / 2 - cardW / 2;
    int startY = 60 - (int)scrollY;
    int spacing = 43;

    // Viewport Scissor Clipping between header and bottom bar
    int fbw = minecraft->width, fbh = minecraft->height;
    PlatformRenderer.GetFramebufferSize(fbw, fbh);

    int y0 = 56;
    int y1 = height - 42;

    glEnable(GL_SCISSOR_TEST);
    int scX = 0;
    int scW = fbw;
    int scY = (height - y1) * fbh / height;
    int scH = (y1 - y0) * fbh / height;
    glScissor(scX, scY < 0 ? 0 : scY, scW, scH < 0 ? 0 : scH);

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
    } else if (currentTab == 4) {
        // Tab 4: Thermal Protection Card with large ice block icon
        int thCardH = 145;
        fill(cardX, startY, cardX + cardW, startY + thCardH, 0xd0101522);
        hLine(cardX, cardX + cardW, startY, 0xff253347);
        vLine(cardX, startY, startY + thCardH, 0xff202c3e);
        vLine(cardX + cardW, startY, startY + thCardH, 0xff141c27);
        hLine(cardX, cardX + cardW, startY + thCardH, 0xff141c27);

        // Center Large Ice Block Icon (2.2x scale)
        if (iconThermal && itemRenderer) {
            glEnable(GL_RESCALE_NORMAL);
            glEnable(GL_COLOR_MATERIAL);
            Lighting::turnOnGui();
            float thIconX = (float)width / 2.0f - (16.0f * 2.2f) / 2.0f;
            itemRenderer->renderGuiItem(font, minecraft->textures, iconThermal, thIconX, (float)(startY + 10), 2.2f, 1.0f);
            Lighting::turnOff();
            glDisable(GL_RESCALE_NORMAL);
            glDisable(GL_COLOR_MATERIAL);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }

        drawCenteredString(font, "PROTECTOR TERMICO DE BATERIA Y CPU", width / 2, startY + 52, 0x38bdf8);

        float curTemp = getDeviceTemperature();
        char tempStr[64];
        if (curTemp > 0.0f) {
            snprintf(tempStr, sizeof(tempStr), "Temperatura Actual del Dispositivo: %.1f *C", curTemp);
        } else {
            snprintf(tempStr, sizeof(tempStr), "Sensor Termico: Estado Normal");
        }

        int tempColor = 0x22c55e; // Green
        if (curTemp >= 45.0f) tempColor = 0xef4444; // Red
        else if (curTemp >= 40.0f) tempColor = 0xf59e0b; // Orange/Yellow

        drawCenteredString(font, tempStr, width / 2, startY + 66, tempColor);
        drawCenteredString(font, "Regula los FPS y carga si el telefono se sobrecalienta.", width / 2, startY + 78, 0x8899aa);
    } else if (currentTab == 5) {
        // Tab 5: Controls settings cards
        renderCard(cardX, startY, cardW, cardH, iconControls, 1.6f,
                   "Estilo de Controles", "Moderno (Bedrock), Cruceta Clasica o Joystick");
        renderCard(cardX, startY + spacing, cardW, cardH, iconControls, 1.6f,
                   "Tamano de Botones", "Escala de los controles virtuales en pantalla");
        renderCard(cardX, startY + spacing * 2, cardW, cardH, iconControls, 1.6f,
                   "Transparencia / Opacidad", "Nivel de visibilidad de los controles tactiles");
        renderCard(cardX, startY + spacing * 3, cardW, cardH, iconControls, 1.6f,
                   "Control Dividido (Mira)", "Mira central fija vs tocar bloques directamente");
    }

    // Render active tab buttons inside scissor test
    for (Button* btn : buttons) {
        if (!btn || btn == doneBtn || (btn->id >= 10 && btn->id <= 15)) continue;
        if (btn->visible) {
            btn->render(minecraft, xm, ym);
        }
    }

    glDisable(GL_SCISSOR_TEST);

    // Slim scrollbar indicator
    if (maxScroll > 0.0f) {
        int trackX = cardX + cardW + 4;
        if (trackX + 5 > width) trackX = width - 5;
        int trackY0 = 60;
        int trackY1 = height - 46;
        int trackH = trackY1 - trackY0;
        int thumbH = std::max(20, (int)((float)trackH * (float)viewportH / (float)contentH));
        int thumbY = trackY0 + (int)(scrollY * (float)(trackH - thumbH) / maxScroll);

        fill(trackX, trackY0, trackX + 3, trackY1, 0x40000000);
        fill(trackX, thumbY, trackX + 3, thumbY + thumbH, 0xaa38bdf8);
    }

    // Top Header Bar (renders OVER scrolled cards)
    glDisable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    fill(0, 0, width, 56, 0xdd080b10);
    hLine(0, width, 56, 0xff253042);

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

    // Render tab buttons
    if (tabSodiumBtn) tabSodiumBtn->render(minecraft, xm, ym);
    if (tabOptifineBtn) tabOptifineBtn->render(minecraft, xm, ym);
    if (tabFullbrightBtn) tabFullbrightBtn->render(minecraft, xm, ym);
    if (tabShadersBtn) tabShadersBtn->render(minecraft, xm, ym);
    if (tabThermalBtn) tabThermalBtn->render(minecraft, xm, ym);
    if (tabControlsBtn) tabControlsBtn->render(minecraft, xm, ym);

    // Active tab indicator
    int tabW = 66;
    if (width < 450) tabW = 54;
    int startTabX = width / 2 - (tabW * 6 + 20) / 2;
    int curTabX = startTabX + currentTab * (tabW + 4);
    fill(curTabX, 52, curTabX + tabW, 55, 0xff38bdf8);

    // Bottom Bar (renders OVER scrolled cards)
    glDisable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    hLine(0, width, height - 42, 0xff253042);
    fill(0, height - 42, width, height, 0xdd080b10);
    if (doneBtn) doneBtn->render(minecraft, xm, ym);
}
