#include "TitleScreen.h"

#include <stdint.h>
#include <time.h>

#include <chrono>
#include <cmath>
#include <ctime>
#include <vector>
#include <numbers>

#include "java/InputOutputStream/BufferedReader.h"
#include "java/InputOutputStream/ByteArrayInputStream.h"
#include "java/InputOutputStream/InputStreamReader.h"
#include "java/Random.h"
#include "minecraft/IGameServices.h"
#include "minecraft/client/BufferedImage.h"
#include "minecraft/client/ClientConstants.h"
#include "minecraft/client/Lighting.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/gui/Button.h"
#include "minecraft/client/gui/Font.h"
#include "minecraft/client/gui/JoinMultiplayerScreen.h"
#include "minecraft/client/gui/LunarModsScreen.h"
#include "minecraft/client/gui/OptionsScreen.h"
#include "minecraft/client/gui/SelectWorldScreen.h"
#include "minecraft/client/model/HumanoidModel.h"
#include "minecraft/client/renderer/Tesselator.h"
#include "minecraft/client/renderer/Textures.h"
#include "minecraft/client/resources/ResourceLocation.h"
#include "minecraft/locale/Language.h"
#include "minecraft/util/Log.h"
#include "minecraft/client/renderer/entity/ItemRenderer.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/level/tile/GrassTile.h"
#include "platform/renderer/renderer.h"
#include "platform/stubs.h"
#include "util/StringHelpers.h"

Random* TitleScreen::random = new Random();

TitleScreen::TitleScreen() {
    // 4J - added initialisers
    vo = 0;
    multiplayerButton = nullptr;
    playerModel = new HumanoidModel(0.0f);
    itemRenderer = new ItemRenderer();

    splash = "missingno";
    //    try {	// 4J - removed try/catch
    std::vector<std::string> splashes;

    // 4jcraft: copied over from UIScene_MainMenu
    int splashIndex;

    std::string filename = "splashes.txt";
    if (gameServices().hasArchiveFile(filename)) {
        std::vector<uint8_t> splashesArray =
            gameServices().getArchiveFile(filename);
        ByteArrayInputStream bais(splashesArray);
        InputStreamReader isr(&bais);
        BufferedReader br(&isr);

        std::string line = "";
        while (!(line = br.readLine()).empty()) {
            line = trimString(line);
            if (line.length() > 0) {
                splashes.push_back(line);
            }
        }

        br.close();
    }

    splashIndex =
        eSplashRandomStart + 1 +
        random->nextInt((int)splashes.size() - (eSplashRandomStart + 1));

    // Override splash text on certain dates
    auto now = std::chrono::system_clock::now();
    auto dp = std::chrono::floor<std::chrono::days>(now);
    std::chrono::year_month_day ymd{dp};
    const auto month = ymd.month();
    const uint32_t day = static_cast<uint32_t>(ymd.day());

    if (month == std::chrono::November && day == 9) {
        splashIndex = eSplashHappyBirthdayEx;
    } else if (month == std::chrono::June && day == 1) {
        splashIndex = eSplashHappyBirthdayNotch;
    } else if (month == std::chrono::December && day == 24) {
        // the Java game shows this on Christmas Eve, so we will too
        splashIndex = eSplashMerryXmas;
    } else if (month == std::chrono::January && day == 1) {
        splashIndex = eSplashHappyNewYear;
    }

    splash = splashes.at(splashIndex);
}

void TitleScreen::tick() {
    vo += 1.0f;
    // if( vo > 100.0f ) minecraft->setScreen(new SelectWorldScreen(this));
    // // 4J - temp testing
}

void TitleScreen::keyPressed(char eventCharacter, int eventKey) {}

void TitleScreen::init() {
    Log::info("TitleScreen::init() START\n");

    // 4jcraft: this is for the blured panorama background
    viewportTexture =
        minecraft->textures->getTexture(new BufferedImage(256, 256, 2));
    /* 4J - removed
Calendar c = Calendar.getInstance();
c.setTime(new Date());

if (c.get(Calendar.MONTH) + 1 == 11 && c.get(Calendar.DAY_OF_MONTH) == 9) {
    splash = "Happy birthday, ez!";
} else if (c.get(Calendar.MONTH) + 1 == 6 && c.get(Calendar.DAY_OF_MONTH) == 1)
{ splash = "Happy birthday, Notch!"; } else if (c.get(Calendar.MONTH) + 1 == 12
&& c.get(Calendar.DAY_OF_MONTH) == 24) { splash = "Merry X-mas!"; } else if
(c.get(Calendar.MONTH) + 1 == 1 && c.get(Calendar.DAY_OF_MONTH) == 1) { splash =
"Happy new year!";
}
    */

    Language* language = Language::getInstance();

    // Bedrock / PE style: Enlarge buttons and place them on the LEFT side
    int btnWidth = 225;
    if (width >= 600) {
        btnWidth = 235;
    } else if (width < 400) {
        btnWidth = 190;
    }
    int btnHeight = 32;
    int spacing = 38;
    int btnX = 35;
    if (width >= 600) {
        btnX = 45;
    }

    int topPos = height / 2 - (spacing * 4) / 2 + 10;
    if (topPos < 38) topPos = 38;

    std::string playText = language->getElement("menu.play");
    if (playText.empty() || playText == "menu.play") {
        playText = "Jugar";
    }

    Button* btnPlay = new Button(1, btnX, topPos, btnWidth, btnHeight, playText);
    btnPlay->setTextureIcon(0, 0, 16);
    btnPlay->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::grass)));
    buttons.push_back(btnPlay);

    Button* btnMods = new Button(5, btnX, topPos + spacing * 1, btnWidth, btnHeight, "Mods (Lunar Client)");
    btnMods->setTextureIcon(32, 16, 16);
    btnMods->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::netherStar)));
    buttons.push_back(btnMods);

    Button* btnOptions = new Button(0, btnX, topPos + spacing * 2, btnWidth, btnHeight, language->getElement("menu.options"));
    btnOptions->setTextureIcon(48, 16, 16);
    btnOptions->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::repeater)));
    buttons.push_back(btnOptions);

    Button* btnQuit = new Button(4, btnX, topPos + spacing * 3, btnWidth, btnHeight, language->getElement("menu.quit"));
    btnQuit->setTextureIcon(64, 16, 16);
    btnQuit->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::door_iron)));
    buttons.push_back(btnQuit);
}

void TitleScreen::buttonClicked(Button* button) {
    if (button->id == 0) {
        Log::info(
            "TitleScreen::buttonClicked() 'Options...' if (button->id == 0)\n");
        minecraft->setScreen(new OptionsScreen(this, minecraft->options));
    }
    if (button->id == 1) {
        Log::info(
            "TitleScreen::buttonClicked() 'Play' if (button->id == 1)\n");
        minecraft->setScreen(new SelectWorldScreen(this));
    }
    if (button->id == 4) {
        Log::info(
            "TitleScreen::buttonClicked() Exit Game if (button->id == 4)\n");
        PlatformRenderer.Close();  // minecraft->stop();
    }
    if (button->id == 5) {
        Log::info(
            "TitleScreen::buttonClicked() 'Mods (Lunar Client)' if (button->id == 5)\n");
        minecraft->setScreen(new LunarModsScreen(this));
    }
}

// 4jcraft: render our panorama
// uses the TU panorama instead of JE panorama and as such a different rendering
// method
void TitleScreen::renderPanorama(float a) {
#ifdef ENABLE_JAVA_GUIS

    Tesselator* t = Tesselator::getInstance();
#ifdef CLASSIC_PANORAMA
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(120.0f, 1.0f, 0.05f, 10.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_CULL_FACE);
    glDepthMask(false);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    char offsetPasses = 8;

    for (int i = 0; i < (offsetPasses * offsetPasses); i++) {
        glPushMatrix();
        float x =
            ((float)(i % offsetPasses) / (float)offsetPasses - 0.5f) / 64.0f;
        float y =
            ((float)(i / offsetPasses) / (float)offsetPasses - 0.5f) / 64.0f;
        float z = 0.0f;
        glTranslatef(x, y, z);
        glRotatef(sin((vo + a) / 400.0f) * 25.0f + 20.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(-(vo + a) * 0.1f, 0.0f, 1.0f, 0.0f);

        for (int j = 0; j < 6; j++) {
            glPushMatrix();

            switch (j) {
                case 1:
                    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
                    break;
                case 2:
                    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
                    break;
                case 3:
                    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
                    break;
                case 4:
                    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
                    break;
                case 5:
                    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
                    break;
                default:
                    break;
            }

            glBindTexture(GL_TEXTURE_2D, minecraft->textures->loadTexture(
                                             TN_TITLE_BG_PANORAMA0 + j));
            t->begin();
            t->color(16777215, 255 / (i + 1));
            t->vertexUV(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f);
            t->vertexUV(1.0f, -1.0f, 1.0f, 1.0f, 0.0f);
            t->vertexUV(1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
            t->vertexUV(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f);
            t->end();
            glPopMatrix();
        }
        glPopMatrix();
        glColorMask(true, true, true, false);
    }

    t->offset(0.0f, 0.0f, 0.0f);
    glColorMask(true, true, true, true);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glDepthMask(true);
    glEnable(GL_CULL_FACE);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
#else
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, width, height, 0, 1000, 3000);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0, 0, -2000);

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(false);

    glBindTexture(GL_TEXTURE_2D,
                  minecraft->textures->loadTexture(TN_TITLE_BG_PANORAMA));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    float off = vo * 0.0004f;

    float screenAspect = (float)width / (float)height;
    float texAspect = 1748.0f / 144.0f;
    float scale;
    if (screenAspect > texAspect) {
        scale = (float)width / 1748.0f;
    } else {
        scale = (float)height / 144.0f;
    }

    float texWidth = 1748.0f * scale;
    float texHeight = 144.0f * scale;
    float yOff = (height - texHeight) / 2.0f;

    float uMax = off + (texWidth / 1748.0f);

    t->begin(GL_QUADS);
    t->color(0xffffff, 255);
    t->vertexUV(0, yOff + texHeight, 0, off, 1.0f);
    t->vertexUV(texWidth, yOff + texHeight, 0, uMax, 1.0f);
    t->vertexUV(texWidth, yOff, 0, uMax, 0.0f);
    t->vertexUV(0, yOff, 0, off, 0.0f);
    t->end();

    glDepthMask(true);
    glDisable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
#endif
#endif
}

// 4jcraft
void TitleScreen::renderSkybox(float a) {
#ifdef ENABLE_JAVA_GUIS
#ifdef CLASSIC_PANORAMA
    glViewport(0, 0, 256, 256);
#endif
    renderPanorama(a);
#ifdef CLASSIC_PANORAMA
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);

    for (int i = 0; i < 8; i++) {
        rotateAndBlur(a);
    }

    glViewport(0, 0, minecraft->width, minecraft->height);

    Tesselator* t = Tesselator::getInstance();
    t->begin();
    float aspect =
        width > height ? 120.0f / (float)width : 120.0f / (float)height;
    float sWidth = (float)height * aspect / 256.0f;
    float sHeight = (float)width * aspect / 256.0f;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    t->color(1.0f, 1.0f, 1.0f, 1.0f);
    t->vertexUV(0.0f, height, 0.0f, (0.5f - sWidth), (0.5f + sHeight));
    t->vertexUV(width, height, 0.0f, (0.5f - sWidth), (0.5f - sHeight));
    t->vertexUV(width, 0.0f, 0.0f, (0.5f + sWidth), (0.5f - sHeight));
    t->vertexUV(0.0f, 0.0f, 0.0f, (0.5f + sWidth), (0.5f + sHeight));
    t->end();
#endif
#endif
}

// 4jcraft
void TitleScreen::rotateAndBlur(float a) {
#if defined(ENABLE_JAVA_GUIS) && defined(CLASSIC_PANORAMA)
    glBindTexture(GL_TEXTURE_2D, viewportTexture);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 256, 256);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMask(true, true, true, false);
    Tesselator* t = Tesselator::getInstance();
    t->begin();
    char blurPasses = 3;

    for (int i = 0; i < blurPasses; i++) {
        t->color(1.0f, 1.0f, 1.0f, 1.0f / (float)(i + 1));
        float offset = (float)(i - blurPasses / 2) / 256.0f;
        t->vertexUV(width, height, 0.0f, (0.0f + offset), 0.0f);
        t->vertexUV(width, 0.0f, 0.0f, (1.0f + offset), 0.0f);
        t->vertexUV(0.0f, 0.0f, 0.0f, (1.0f + offset), 1.0f);
        t->vertexUV(0.0f, height, 0.0f, (0.0f + offset), 1.0f);
    }

    t->end();
    glColorMask(true, true, true, true);
#endif
}

void TitleScreen::render(int xm, int ym, float a) {
#ifdef ENABLE_JAVA_GUIS
    // 4jcraft: panorama
    renderSkybox(a);

    Tesselator* t = Tesselator::getInstance();

    int logoWidth = 155 + 119;
    int logoX = width / 2 - logoWidth / 2;
    int logoY = 16;

    // 4jcraft: gradient for classic panorama
#ifdef CLASSIC_PANORAMA
    fillGradient(0, 0, width, height, -2130706433, 16777215);
    fillGradient(0, 0, width, height, 0, INT_MIN);
#endif

    glBindTexture(GL_TEXTURE_2D,
                  minecraft->textures->loadTexture(TN_TITLE_MCLOGO));
    glColor4f(1, 1, 1, 1);
    blit(logoX + 0, logoY + 0, 0, 0, 155, 44);
    blit(logoX + 155, logoY + 0, 0, 45, 155, 44);
    t->color(0xffffff);
    glPushMatrix();
    glTranslatef((float)width / 2 + 90, (float)logoY + 40, 0);

    glRotatef(-20, 0, 0, 1);
    float sss = 1.8f - std::abs(sinf(System::currentTimeMillis() % 1000 /
                                     1000.0f * std::numbers::pi * 2) *
                                0.1f);

    sss = sss * 100 / (font->width(splash) + 8 * 4);
    glScalef(sss, sss, sss);
    drawCenteredString(font, splash, 0, -8, 0xffff00);
    glPopMatrix();

    // 3D Player Character on the right side (Bedrock / PE style)
    if (playerModel != nullptr) {
        float ss = (height >= 300) ? 96.0f : 75.0f;
        int playerX = width * 3 / 4;
        if (width < 450) {
            playerX = width - 80;
        }
        // Feet anchored near bottom, above copyright bar
        int playerY = height - 20;

        // Reset OpenGL color so yellow splash text or previous GUI draw doesn't tint the skin
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glDisable(GL_COLOR_MATERIAL);
        glEnable(GL_RESCALE_NORMAL);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(true);
        glClear(GL_DEPTH_BUFFER_BIT);

        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.1f);
        glDisable(GL_CULL_FACE);

        Lighting::turnOnGui();

        glPushMatrix();
        // Canonical Minecraft GUI player transformation stack:
        glTranslatef((float)playerX, (float)playerY, 50.0f);
        glScalef(-ss, ss, ss);
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);

        // Interactive touch/mouse tracking
        // Steve's eyes are at roughly 1.6 * ss above his feet
        float steveEyeX = (float)playerX;
        float steveEyeY = (float)playerY - 1.6f * ss;

        float dx = 0.0f;
        float dy = 0.0f;
        if (xm <= 0 && ym <= 0) {
            // Default idle orientation: look slightly towards the screen center buttons
            dx = -(float)width * 0.25f;
            dy = 0.0f;
        } else {
            dx = (float)xm - steveEyeX;
            dy = (float)ym - steveEyeY;
        }

        // Body rotation: base angle of 20 deg facing slightly towards center,
        // turning naturally with touch
        float bodyTurn = (float)atan2(dx, 160.0f) * 57.29578f * 0.35f;
        float bodyRot = 20.0f - bodyTurn;
        if (bodyRot > 60.0f) bodyRot = 60.0f;
        if (bodyRot < -20.0f) bodyRot = -20.0f;

        glRotatef(180.0f - bodyRot, 0.0f, 1.0f, 0.0f);
        glScalef(-1.0f, -1.0f, 1.0f);
        glTranslatef(0.0f, -24.0f * (1.0f / 16.0f), 0.0f);

        // Head tracking
        float targetYaw = -(float)atan2(dx, 100.0f) * 57.29578f;
        float relYaw = targetYaw - bodyRot;
        if (relYaw > 45.0f) relYaw = 45.0f;
        if (relYaw < -45.0f) relYaw = -45.0f;
        float headYaw = relYaw + sinf(vo * 0.04f) * 1.5f;

        float targetPitch = -(float)atan2(dy, 100.0f) * 57.29578f;
        if (targetPitch > 28.0f) targetPitch = 28.0f;
        if (targetPitch < -28.0f) targetPitch = -28.0f;
        float headPitch = targetPitch + cosf(vo * 0.04f) * 1.0f;

        glBindTexture(GL_TEXTURE_2D,
                      minecraft->textures->loadTexture(TN_MOB_CHAR));

        playerModel->attackTime = 0.0f;
        playerModel->holdingRightHand = 0;
        playerModel->holdingLeftHand = 0;
        playerModel->sneaking = false;
        playerModel->idle = false;
        playerModel->eating = false;
        playerModel->eating_swing = 0.0f;
        playerModel->eating_t = 0.0f;
        playerModel->young = false;
        playerModel->riding = false;

        // Render Steve using compiled lists (canonical across the engine)
        playerModel->render(nullptr, 0.0f, 0.0f, 0.0f, headYaw, headPitch, 1.0f / 16.0f, true);

        glPopMatrix();
        Lighting::turnOff();
        glDisable(GL_RESCALE_NORMAL);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(false);
        glEnable(GL_CULL_FACE);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }

    drawString(
        font, ClientConstants::VERSION_STRING, 2, height - 10,
        0xffffff);  // 4jcraft: use the same height as the copyright message
    std::string msg = "Copyright Mojang AB. Do not distribute.";
    drawString(font, msg, width - font->width(msg) - 2, height - 10, 0xffffff);

    Screen::render(xm, ym, a);
#endif
}
