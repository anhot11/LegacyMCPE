#include "ChatScreen.h"

#include <memory>

#include "minecraft/SharedConstants.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/gui/Gui.h"
#include "minecraft/client/gui/Screen.h"
#include "minecraft/client/gui/Button.h"
#include "minecraft/client/multiplayer/MultiPlayerLocalPlayer.h"
#include "platform/input/input.h"
#include "platform/stubs.h"
#include "util/StringHelpers.h"

const std::string ChatScreen::allowedChars = SharedConstants::acceptableLetters;

ChatScreen::ChatScreen() : btnSend(nullptr), btnClose(nullptr) { frame = 0; }

void ChatScreen::init() {
    Keyboard::enableRepeatEvents(true);
    PlatformInput.StartTextInput();
    buttons.clear();

    int sendW = (width >= 400) ? 65 : 55;
    int inputH = 22;
    int sendX = width - sendW - 4;
    int sendY = height - inputH - 2;

    btnSend = new Button(1, sendX, sendY, sendW, inputH, "Enviar");
    btnClose = new Button(2, width - 26, 4, 22, 20, "X");

    buttons.push_back(btnSend);
    buttons.push_back(btnClose);
}

void ChatScreen::removed() {
    Keyboard::enableRepeatEvents(false);
    PlatformInput.StopTextInput();
}

void ChatScreen::sendMessage() {
    std::string msg = trimString(message);
    if (msg.length() > 0) {
        if (!minecraft->handleClientSideCommand(msg)) {
            if (minecraft->player != nullptr) {
                minecraft->player->chat(msg);
            }
        }
    }
    message.clear();
    minecraft->setScreen(nullptr);
}

void ChatScreen::tick() {
    frame++;

    // Poll virtual / soft keyboard input from Android/SDL
    std::string typed = PlatformInput.PollTextInput();
    if (!typed.empty()) {
        for (char ch : typed) {
            if (allowedChars.find(ch) != std::string::npos &&
                message.length() < SharedConstants::maxChatLength) {
                message += ch;
            }
        }
    }

    if (PlatformInput.PollBackspacePressed() && message.length() > 0) {
        message = message.substr(0, message.length() - 1);
    }

    if (PlatformInput.PollReturnPressed()) {
        sendMessage();
    }
}

void ChatScreen::buttonClicked(Button* button) {
    if (!button->active) return;
    if (button->id == 1) {
        sendMessage();
    } else if (button->id == 2) {
        minecraft->setScreen(nullptr);
    }
}

void ChatScreen::keyPressed(char ch, int eventKey) {
    if (eventKey == Keyboard::KEY_ESCAPE) {
        minecraft->setScreen(nullptr);
        return;
    }
    if (eventKey == Keyboard::KEY_RETURN) {
        sendMessage();
        return;
    }
    if (eventKey == Keyboard::KEY_BACK && message.length() > 0)
        message = message.substr(0, message.length() - 1);
    if (allowedChars.find(ch) != std::string::npos &&
        message.length() < SharedConstants::maxChatLength) {
        message += ch;
    }
}

void ChatScreen::render(int xm, int ym, float a) {
    // Dimmed upper area
    fill(0, 0, width, height - 26, 0x40000000);

    // Chat input bar background
    int sendW = (width >= 400) ? 65 : 55;
    fill(2, height - 25, width - sendW - 8, height - 2, 0xc0000000);
    fill(1, height - 26, width - sendW - 7, height - 1, 0x80555555);

    drawString(font, "> " + message + (frame / 6 % 2 == 0 ? "_" : ""), 6,
               height - 18, 0xffffff);

    // Hint at top
    drawString(font, "Chat [Toca fondo o 'X' para salir]", 6, 6, 0x888888);

    Screen::render(xm, ym, a);
}

void ChatScreen::mouseClicked(int x, int y, int buttonNum) {
    Screen::mouseClicked(x, y, buttonNum);

    if (buttonNum == 0) {
        // If clicked on any button, buttonClicked handles it
        if ((btnClose != nullptr && x >= btnClose->x && x < btnClose->x + btnClose->w && y >= btnClose->y && y < btnClose->y + btnClose->h) ||
            (btnSend != nullptr && x >= btnSend->x && x < btnSend->x + btnSend->w && y >= btnSend->y && y < btnSend->y + btnSend->h)) {
            return;
        }

        // Tapping in the upper half of screen dismisses the chat
        if (y < height - 30) {
            minecraft->setScreen(nullptr);
            return;
        }

        // Clicking on an entity name in chat
        if (minecraft->gui != nullptr && minecraft->gui->selectedName != "") {
            if (message.length() > 0 && message[message.length() - 1] != ' ') {
                message += " ";
            }
            message += minecraft->gui->selectedName;
            unsigned int maxLength = SharedConstants::maxChatLength;
            if (message.length() > maxLength) {
                message = message.substr(0, maxLength);
            }
        }
    }
}