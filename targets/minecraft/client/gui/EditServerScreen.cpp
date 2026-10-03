#include "EditServerScreen.h"

#include <vector>

#include "Button.h"
#include "EditBox.h"
#include "SelectWorldScreen.h"
#include "minecraft/client/Minecraft.h"
#include "platform/input/input.h"
#include "platform/stubs.h"
#include "util/StringHelpers.h"

EditServerScreen::EditServerScreen(SelectWorldScreen* lastScreen, int editIndex,
                                   const std::string& name,
                                   const std::string& ip) {
    this->lastScreen = lastScreen;
    this->editIndex = editIndex;
    this->initialName = name.empty() ? "Minecraft Server" : name;
    this->initialIp = ip;
    this->nameEdit = nullptr;
    this->ipEdit = nullptr;
    this->saveButton = nullptr;
    this->cancelButton = nullptr;
}

EditServerScreen::~EditServerScreen() {
    if (nameEdit != nullptr) {
        delete nameEdit;
        nameEdit = nullptr;
    }
    if (ipEdit != nullptr) {
        delete ipEdit;
        ipEdit = nullptr;
    }
}

void EditServerScreen::init() {
    buttons.clear();
    Keyboard::enableRepeatEvents(true);

    int boxW = 220;
    int boxH = 18;
    int startX = width / 2 - boxW / 2;
    int startY = 8; // Top aligned to stay fully visible above the virtual keyboard

    nameEdit = new EditBox(this, font, startX, startY + 22, boxW, boxH, initialName);
    nameEdit->setMaxLength(64);

    ipEdit = new EditBox(this, font, startX, startY + 54, boxW, boxH, initialIp);
    ipEdit->setMaxLength(128);

    // Default focus to IP input and trigger virtual keyboard
    ipEdit->focus(true);

    int btnW = 105;
    int btnH = 20;
    int gap = 10;
    int btnStartX = width / 2 - (btnW * 2 + gap) / 2;
    int btnY = startY + 76;

    buttons.push_back(saveButton = new Button(0, btnStartX, btnY, btnW, btnH, "Guardar"));
    buttons.push_back(cancelButton = new Button(1, btnStartX + btnW + gap, btnY, btnW, btnH, "Cancelar"));

    saveButton->active = !trimString(ipEdit->getValue()).empty();
}

void EditServerScreen::tick() {
    if (nameEdit != nullptr) nameEdit->tick();
    if (ipEdit != nullptr) ipEdit->tick();
    if (saveButton != nullptr && ipEdit != nullptr) {
        saveButton->active = !trimString(ipEdit->getValue()).empty();
    }
}

void EditServerScreen::removed() {
    Keyboard::enableRepeatEvents(false);
    if (nameEdit != nullptr) nameEdit->focus(false);
    if (ipEdit != nullptr) ipEdit->focus(false);
    PlatformInput.StopTextInput();
}

void EditServerScreen::buttonClicked(Button* button) {
    if (!button->active) return;

    if (button->id == 1) { // Cancel
        minecraft->setScreen(lastScreen);
    } else if (button->id == 0) { // Save
        std::string name = nameEdit ? trimString(nameEdit->getValue()) : "";
        if (name.empty()) name = "Minecraft Server";

        std::string rawAddr = ipEdit ? trimString(ipEdit->getValue()) : "";
        if (!rawAddr.empty()) {
            std::string host = rawAddr;
            int port = 25565;
            size_t colon = rawAddr.find(':');
            if (colon != std::string::npos) {
                host = rawAddr.substr(0, colon);
                std::string pStr = rawAddr.substr(colon + 1);
                try {
                    port = std::stoi(pStr);
                    if (port <= 0 || port > 65535) port = 25565;
                } catch (...) {
                    port = 25565;
                }
            }

            if (editIndex < 0) {
                lastScreen->addServer(name, host, port);
            } else {
                lastScreen->updateServer(editIndex, name, host, port);
            }
            minecraft->setScreen(lastScreen);
        }
    }
}

void EditServerScreen::keyPressed(char ch, int eventKey) {
    if (nameEdit != nullptr && nameEdit->inFocus) {
        if (ch == 9 || eventKey == Keyboard::KEY_TAB) {
            nameEdit->focus(false);
            if (ipEdit != nullptr) ipEdit->focus(true);
            return;
        }
        if (ch == 13 || eventKey == Keyboard::KEY_RETURN) {
            nameEdit->focus(false);
            if (ipEdit != nullptr) ipEdit->focus(true);
            return;
        }
        nameEdit->keyPressed(ch, eventKey);
    } else if (ipEdit != nullptr && ipEdit->inFocus) {
        if (ch == 9 || eventKey == Keyboard::KEY_TAB) {
            ipEdit->focus(false);
            if (nameEdit != nullptr) nameEdit->focus(true);
            return;
        }
        if (ch == 13 || eventKey == Keyboard::KEY_RETURN) {
            buttonClicked(saveButton);
            return;
        }
        ipEdit->keyPressed(ch, eventKey);
    } else {
        if (eventKey == Keyboard::KEY_ESCAPE) {
            minecraft->setScreen(lastScreen);
            return;
        }
        Screen::keyPressed(ch, eventKey);
    }
}

void EditServerScreen::mouseClicked(int x, int y, int buttonNum) {
    Screen::mouseClicked(x, y, buttonNum);
    int startY = 8;
    int boxW = 220;
    int startX = width / 2 - boxW / 2;

    bool hitName = (nameEdit != nullptr && x >= startX - 10 && x < startX + boxW + 10 &&
                    y >= startY + 10 && y < startY + 44);
    bool hitIp = (ipEdit != nullptr && x >= startX - 10 && x < startX + boxW + 10 &&
                  y >= startY + 44 && y < startY + 74);

    if (hitName) {
        if (ipEdit != nullptr) ipEdit->focus(false);
        if (nameEdit != nullptr) nameEdit->focus(true);
    } else if (hitIp) {
        if (nameEdit != nullptr) nameEdit->focus(false);
        if (ipEdit != nullptr) ipEdit->focus(true);
    }
}

void EditServerScreen::render(int xm, int ym, float a) {
    renderDirtBackground(0);

    int startX = width / 2 - 110;
    int startY = 8;

    drawCenteredString(font, editIndex < 0 ? "Agregar Servidor" : "Editar Servidor", width / 2, startY, 0xffffff);

    drawString(font, "Nombre del Servidor:", startX, startY + 11, 0xa0a0a0);
    if (nameEdit != nullptr) nameEdit->render();

    drawString(font, "Direccion IP / Host:Puerto:", startX, startY + 43, 0xa0a0a0);
    if (ipEdit != nullptr) ipEdit->render();

    Screen::render(xm, ym, a);
}
