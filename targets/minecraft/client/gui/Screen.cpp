#include "Screen.h"

#include "Button.h"
#include "app/common/Audio/ConsoleSoundEngine.h"
#include "app/common/Audio/SoundTypes.h"
#include "minecraft/GameEnums.h"
#include "minecraft/IGameServices.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/gui/Screen.h"
#include "minecraft/client/gui/ScreenSizeCalculator.h"
#include "minecraft/client/gui/particle/GuiParticles.h"
#include "minecraft/client/renderer/Tesselator.h"
#include "minecraft/network/INetworkService.h"
#include "minecraft/server/MinecraftServer.h"
#include "minecraft/server/ServerAction.h"
#include "platform/input/input.h"
#include "platform/profile/profile.h"
#include "platform/stubs.h"

Screen::Screen()  // 4J added
{
    minecraft = nullptr;
    width = 0;
    height = 0;
    passEvents = false;
    font = nullptr;
    particles = nullptr;
    clickedButton = nullptr;
}

void Screen::render(int xm, int ym, float a) {
    auto itEnd = buttons.end();
    for (auto it = buttons.begin(); it != itEnd; it++) {
        Button* button = *it;  // buttons[i];
        button->render(minecraft, xm, ym);
    }
}

void Screen::keyPressed(char eventCharacter, int eventKey) {
    if (eventKey == Keyboard::KEY_ESCAPE) {
        minecraft->setScreen(nullptr);
        //    minecraft->grabMouse();	// 4J - removed
        // 4jcraft: moved here from PauseScreen to ensure that serverside
        // unpausing is done in all scenarios
        if (NetworkService.IsLocalGame() &&
            NetworkService.GetPlayerCount() == 1)
            MinecraftServer::getInstance()->queueServerAction(
                minecraft::server::PauseServer{false});
    }
}

std::string Screen::getClipboard() {
    // 4J - removed
    return std::string();
}

void Screen::setClipboard(const std::string& str) {
    // 4J - removed
}

void Screen::mouseClicked(int x, int y, int buttonNum) {
    if (buttonNum == 0) {
        auto itEnd = buttons.end();
        for (auto it = buttons.begin(); it != itEnd; it++) {
            Button* button = *it;  // buttons[i];
            if (button->clicked(minecraft, x, y)) {
                clickedButton = button;
                minecraft->soundEngine->playUI(eSoundType_RANDOM_CLICK, 1, 1);
                buttonClicked(button);
                break;
            }
        }
    }
}

void Screen::mouseReleased(int x, int y, int buttonNum) {
    if (clickedButton != nullptr && buttonNum == 0) {
        clickedButton->released(x, y);
        clickedButton = nullptr;
    }
}

void Screen::buttonClicked(Button* button) {}

static bool s_prevLeftState = false;
static bool s_prevRightState = false;

void Screen::resetInputState() {
    s_prevLeftState = PlatformInput.ButtonDown(0, MINECRAFT_ACTION_ACTION);
    s_prevRightState = PlatformInput.ButtonDown(0, MINECRAFT_ACTION_USE);
}

void Screen::init(Minecraft* minecraft, int width, int height) {
    particles = new GuiParticles(minecraft);
    this->minecraft = minecraft;
    this->font = minecraft->font;
    this->width = width;
    this->height = height;
    clickedButton = nullptr;
    buttons.clear();
    resetInputState();
    init();
}

void Screen::setSize(int width, int height) {
    this->width = width;
    this->height = height;
}

void Screen::init() {}

void Screen::updateEvents() {
// TODO: update for SDL if we ever get around to that
#if (defined(ENABLE_JAVA_GUIS))
    int fbw = minecraft->width, fbh = minecraft->height;
    PlatformRenderer.GetFramebufferSize(fbw, fbh);
    if (fbw <= 0) fbw = minecraft->width;
    if (fbh <= 0) fbh = minecraft->height;
    glViewport(0, 0, fbw, fbh);
    ScreenSizeCalculator ssc(minecraft->options, fbw, fbh);
    int screenWidth = ssc.getWidth();
    int screenHeight = ssc.getHeight();
    int xMouse = PlatformInput.GetMouseX() * screenWidth / fbw;
    int yMouse = PlatformInput.GetMouseY() * screenHeight / fbh;

    bool leftState = PlatformInput.ButtonDown(0, MINECRAFT_ACTION_ACTION);
    bool rightState = PlatformInput.ButtonDown(0, MINECRAFT_ACTION_USE);

    if (leftState && !s_prevLeftState) {
        s_prevLeftState = leftState;
        s_prevRightState = rightState;
        mouseClicked(xMouse, yMouse, 0);
        return;
    } else if (!leftState && s_prevLeftState) {
        s_prevLeftState = leftState;
        s_prevRightState = rightState;
        mouseReleased(xMouse, yMouse, 0);
        return;
    }

    if (rightState && !s_prevRightState) {
        s_prevLeftState = leftState;
        s_prevRightState = rightState;
        mouseClicked(xMouse, yMouse, 1);
        return;
    } else if (!rightState && s_prevRightState) {
        s_prevLeftState = leftState;
        s_prevRightState = rightState;
        mouseReleased(xMouse, yMouse, 1);
        return;
    }

    if (PlatformInput.ButtonPressed(0, ACTION_MENU_CANCEL) ||
        PlatformInput.ButtonPressed(0, ACTION_MENU_B)) {
        keyPressed(0, Keyboard::KEY_ESCAPE);
    }

    if (PlatformInput.ButtonPressed(0, MINECRAFT_ACTION_INVENTORY)) {
        keyPressed('e', Keyboard::KEY_E);
    }

    // Process typed characters from Android soft keyboard (IME) and physical
    // keyboard
    std::string text = PlatformInput.PollTextInput();
    for (char c : text) {
        keyPressed(c, 0);
    }
    if (PlatformInput.PollBackspacePressed()) {
        keyPressed(0, Keyboard::KEY_BACK);
    }
    if (PlatformInput.PollReturnPressed()) {
        keyPressed(13, Keyboard::KEY_RETURN);
    }

    s_prevLeftState = leftState;
    s_prevRightState = rightState;
#else
    /* 4J - TODO
while (Mouse.next()) {
    mouseEvent();
}

while (Keyboard.next()) {
    keyboardEvent();
}
    */
#endif
}

void Screen::mouseEvent() {
    /* 4J - TODO
if (Mouse.getEventButtonState()) {
    int xm = Mouse.getEventX() * width / minecraft.width;
    int ym = height - Mouse.getEventY() * height / minecraft.height - 1;
    mouseClicked(xm, ym, Mouse.getEventButton());
} else {
    int xm = Mouse.getEventX() * width / minecraft.width;
    int ym = height - Mouse.getEventY() * height / minecraft.height - 1;
    mouseReleased(xm, ym, Mouse.getEventButton());
}
    */
}

void Screen::keyboardEvent() {
    /* 4J - TODO
if (Keyboard.getEventKeyState()) {
    if (Keyboard.getEventKey() == Keyboard.KEY_F11) {
        minecraft.toggleFullScreen();
        return;
    }
    keyPressed(Keyboard.getEventCharacter(), Keyboard.getEventKey());
}
    */
}

void Screen::tick() {}

void Screen::removed() {}

void Screen::renderBackground() { renderBackground(0); }

void Screen::renderBackground(int vo) {
    if (minecraft->level != nullptr) {
        fillGradient(0, 0, width, height, 0xc0101010, 0xd0101010);
    } else {
        renderDirtBackground(vo);
    }
}

void Screen::renderDirtBackground(int vo) {
#ifdef ENABLE_JAVA_GUIS
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    Tesselator* t = Tesselator::getInstance();
    glBindTexture(GL_TEXTURE_2D,
                  minecraft->textures->loadTexture(TN_GUI_BACKGROUND));
    glColor4f(1, 1, 1, 1);
    float s = 32;
    t->begin();
    t->color(0x404040);
    t->vertexUV(static_cast<float>(0), static_cast<float>(height),
                static_cast<float>(0), static_cast<float>(0),
                static_cast<float>(height / s + vo));
    t->vertexUV(static_cast<float>(width), static_cast<float>(height),
                static_cast<float>(0), static_cast<float>(width / s),
                static_cast<float>(height / s + vo));
    t->vertexUV(static_cast<float>(width), static_cast<float>(0),
                static_cast<float>(0), static_cast<float>(width / s),
                static_cast<float>(0 + vo));
    t->vertexUV(static_cast<float>(0), static_cast<float>(0),
                static_cast<float>(0), static_cast<float>(0),
                static_cast<float>(0 + vo));
    t->end();
#endif
}

bool Screen::isPauseScreen() { return true; }

void Screen::confirmResult(bool result, int id) {}

void Screen::tabPressed() {}
