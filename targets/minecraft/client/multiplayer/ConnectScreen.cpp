#include "ConnectScreen.h"

#include <memory>
#include <vector>

#include "ClientConnection.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/User.h"
#include "minecraft/client/gui/Button.h"
#include "minecraft/client/gui/Screen.h"
#include "minecraft/client/title/TitleScreen.h"
#include "minecraft/locale/Language.h"
#include "minecraft/network/packet/PreLoginPacket.h"

ConnectScreen::ConnectScreen(Minecraft* minecraft, const std::string& ip,
                             int port) {
    aborted = false;
    //    System.out.println("Connecting to " + ip + ", " + port);
    minecraft->setLevel(nullptr);
    // 4J - removed from separate thread, but need to investigate what we
    // actually need here
    connection = new ClientConnection(minecraft, ip, port);
    if (aborted) return;
    connection->send(std::shared_ptr<PreLoginPacket>(
        new PreLoginPacket(minecraft->user->name)));
}

void ConnectScreen::tick() {
    if (connection != nullptr) {
        connection->tick();
    }
}

void ConnectScreen::keyPressed(char eventCharacter, int eventKey) {}

void ConnectScreen::init() {
    Language* language = Language::getInstance();

    buttons.clear();
    buttons.push_back(new Button(0, width / 2 - 100, height / 4 + 24 * 5 + 12,
                                 language->getElement("gui.cancel")));
}

void ConnectScreen::buttonClicked(Button* button) {
    if (button->id == 0) {
        aborted = true;
        if (connection != nullptr) connection->close();
        minecraft->setScreen(new TitleScreen());
    }
}

void ConnectScreen::render(int xm, int ym, float a) {
    renderBackground();

    Language* language = Language::getInstance();

    if (connection == nullptr) {
        drawCenteredString(font, language->getElement("connect.connecting"),
                           width / 2, height / 2 - 50, 0xffffff);
        drawCenteredString(font, "", width / 2, height / 2 - 10, 0xffffff);
    } else {
        drawCenteredString(font, language->getElement("connect.authorizing"),
                           width / 2, height / 2 - 50, 0xffffff);
        drawCenteredString(font, connection->message, width / 2,
                           height / 2 - 10, 0xffffff);
    }

    Screen::render(xm, ym, a);
}