#include "JoinMultiplayerScreen.h"

#include <fstream>
#include <sstream>
#include <vector>

#include "Button.h"
#include "EditBox.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/Options.h"
#include "minecraft/client/gui/Screen.h"
#include "minecraft/client/multiplayer/ConnectScreen.h"
#include "minecraft/locale/Language.h"
#include "platform/stubs.h"
#include "util/StringHelpers.h"

JoinMultiplayerScreen::JoinMultiplayerScreen(Screen* lastScreen) {
    this->lastScreen = lastScreen;
    this->currentMode = MODE_LIST;
    this->selectedServer = -1;
    this->selectionList = nullptr;

    this->btnJoin = nullptr;
    this->btnDirect = nullptr;
    this->btnAdd = nullptr;
    this->btnDelete = nullptr;
    this->btnCancel = nullptr;

    this->addServerNameEdit = nullptr;
    this->addServerIpEdit = nullptr;
    this->btnAddSave = nullptr;
    this->btnAddCancel = nullptr;

    this->directIpEdit = nullptr;
    this->btnDirectJoin = nullptr;
    this->btnDirectCancel = nullptr;
}

JoinMultiplayerScreen::~JoinMultiplayerScreen() {
    if (selectionList != nullptr) {
        delete selectionList;
        selectionList = nullptr;
    }
    if (addServerNameEdit != nullptr) {
        delete addServerNameEdit;
        addServerNameEdit = nullptr;
    }
    if (addServerIpEdit != nullptr) {
        delete addServerIpEdit;
        addServerIpEdit = nullptr;
    }
    if (directIpEdit != nullptr) {
        delete directIpEdit;
        directIpEdit = nullptr;
    }
}

void JoinMultiplayerScreen::loadServerList() {
    serverList.clear();
    std::string path = File(minecraft->getWorkingDirectory(), "servers.txt").getPath();
    std::ifstream infile(path);
    if (infile.is_open()) {
        std::string line;
        while (std::getline(infile, line)) {
            line = trimString(line);
            if (line.empty() || line[0] == '#') continue;

            // format: name;ip;port or name;ip:port
            size_t semi1 = line.find(';');
            if (semi1 != std::string::npos) {
                std::string name = line.substr(0, semi1);
                std::string rest = line.substr(semi1 + 1);
                size_t semi2 = rest.find(';');
                std::string ip;
                int port = 19132;
                if (semi2 != std::string::npos) {
                    ip = rest.substr(0, semi2);
                    std::string portStr = rest.substr(semi2 + 1);
                    if (!portStr.empty()) port = std::atoi(portStr.c_str());
                } else {
                    parseIpPort(rest, ip, port);
                }
                if (!name.empty() && !ip.empty()) {
                    serverList.push_back({name, ip, port});
                }
            } else {
                std::string ip;
                int port = 19132;
                parseIpPort(line, ip, port);
                if (!ip.empty()) {
                    serverList.push_back({ip, ip, port});
                }
            }
        }
        infile.close();
    }

    if (serverList.empty()) {
        // Preload MCPE 0.15 Community Servers
        serverList.push_back({"HYDRA Remastered", "hydrahcf.net", 19132});
        serverList.push_back({"MineBox PE", "mbox.shockbyte.me", 8116});
        serverList.push_back({"InPvP Network", "play.inpvp.net", 19132});
        serverList.push_back({"Lifeboat Network", "play.lbsg.net", 19132});
        serverList.push_back({"CookieBuild Brasil", "br.cookie-build.com", 19132});
        serverList.push_back({"Local Server", "127.0.0.1", 19132});
        saveServerList();
    }
}

void JoinMultiplayerScreen::saveServerList() {
    std::string path = File(minecraft->getWorkingDirectory(), "servers.txt").getPath();
    std::ofstream outfile(path);
    if (outfile.is_open()) {
        for (const auto& s : serverList) {
            outfile << s.name << ";" << s.ip << ";" << s.port << "\n";
        }
        outfile.close();
    }
}

void JoinMultiplayerScreen::parseIpPort(const std::string& input, std::string& outIp, int& outPort) {
    outIp = input;
    outPort = 19132;
    size_t col = input.find(':');
    if (col != std::string::npos) {
        outIp = input.substr(0, col);
        std::string p = input.substr(col + 1);
        if (!p.empty()) {
            outPort = std::atoi(p.c_str());
            if (outPort <= 0 || outPort > 65535) outPort = 19132;
        }
    }
}

void JoinMultiplayerScreen::init() {
    loadServerList();
    if (currentMode == MODE_LIST) {
        initModeList();
    } else if (currentMode == MODE_ADD_SERVER) {
        initModeAddServer();
    } else if (currentMode == MODE_DIRECT_CONNECT) {
        initModeDirectConnect();
    }
}

void JoinMultiplayerScreen::initModeList() {
    buttons.clear();
    Keyboard::enableRepeatEvents(false);

    if (selectionList != nullptr) {
        delete selectionList;
    }
    selectionList = new ServerSelectionList(this);

    int btnW = 95;
    int btnH = 20;
    int gap = 6;
    int row1Y = height - 52;
    int row2Y = height - 28;

    // Row 1: Join (0), Direct Connect (1), Add Server (2)
    int row1TotalW = btnW * 3 + gap * 2;
    int row1StartX = (width - row1TotalW) / 2;

    btnJoin = new Button(0, row1StartX, row1Y, btnW, btnH, "Join Server");
    btnDirect = new Button(1, row1StartX + btnW + gap, row1Y, btnW, btnH, "Direct IP");
    btnAdd = new Button(2, row1StartX + (btnW + gap) * 2, row1Y, btnW, btnH, "Add Server");

    // Row 2: Delete (3), Cancel (4)
    int row2TotalW = btnW * 2 + gap;
    int row2StartX = (width - row2TotalW) / 2;

    btnDelete = new Button(3, row2StartX, row2Y, btnW, btnH, "Delete");
    btnCancel = new Button(4, row2StartX + btnW + gap, row2Y, btnW, btnH, "Cancel");

    buttons.push_back(btnJoin);
    buttons.push_back(btnDirect);
    buttons.push_back(btnAdd);
    buttons.push_back(btnDelete);
    buttons.push_back(btnCancel);

    updateButtonState();
}

void JoinMultiplayerScreen::initModeAddServer() {
    buttons.clear();
    Keyboard::enableRepeatEvents(true);

    int boxW = 200;
    int boxH = 20;
    int startX = (width - boxW) / 2;
    int startY = height / 4;

    if (addServerNameEdit != nullptr) delete addServerNameEdit;
    addServerNameEdit = new EditBox(this, font, startX, startY + 10, boxW, boxH, "Minecraft Server");
    addServerNameEdit->focus(true);
    addServerNameEdit->setMaxLength(64);

    if (addServerIpEdit != nullptr) delete addServerIpEdit;
    addServerIpEdit = new EditBox(this, font, startX, startY + 60, boxW, boxH, "");
    addServerIpEdit->focus(false);
    addServerIpEdit->setMaxLength(128);

    int btnW = 95;
    int btnH = 20;
    int gap = 10;
    int btnStartX = (width - (btnW * 2 + gap)) / 2;
    int btnY = startY + 100;

    btnAddSave = new Button(10, btnStartX, btnY, btnW, btnH, "Save");
    btnAddCancel = new Button(11, btnStartX + btnW + gap, btnY, btnW, btnH, "Cancel");

    buttons.push_back(btnAddSave);
    buttons.push_back(btnAddCancel);
}

void JoinMultiplayerScreen::initModeDirectConnect() {
    buttons.clear();
    Keyboard::enableRepeatEvents(true);

    int boxW = 200;
    int boxH = 20;
    int startX = (width - boxW) / 2;
    int startY = height / 3;

    if (directIpEdit != nullptr) delete directIpEdit;
    std::string lastIp = replaceAll(minecraft->options->lastMpIp, "_", ":");
    directIpEdit = new EditBox(this, font, startX, startY, boxW, boxH, lastIp);
    directIpEdit->focus(true);
    directIpEdit->setMaxLength(128);

    int btnW = 95;
    int btnH = 20;
    int gap = 10;
    int btnStartX = (width - (btnW * 2 + gap)) / 2;
    int btnY = startY + 40;

    btnDirectJoin = new Button(20, btnStartX, btnY, btnW, btnH, "Connect");
    btnDirectCancel = new Button(21, btnStartX + btnW + gap, btnY, btnW, btnH, "Cancel");

    buttons.push_back(btnDirectJoin);
    buttons.push_back(btnDirectCancel);
}

void JoinMultiplayerScreen::updateButtonState() {
    bool hasSelection = (selectedServer >= 0 && selectedServer < (int)serverList.size());
    if (btnJoin != nullptr) btnJoin->active = hasSelection;
    if (btnDelete != nullptr) btnDelete->active = hasSelection;
}

void JoinMultiplayerScreen::tick() {
    if (currentMode == MODE_ADD_SERVER) {
        if (addServerNameEdit) addServerNameEdit->tick();
        if (addServerIpEdit) addServerIpEdit->tick();
    } else if (currentMode == MODE_DIRECT_CONNECT) {
        if (directIpEdit) directIpEdit->tick();
    }
}

void JoinMultiplayerScreen::removed() {
    Keyboard::enableRepeatEvents(false);
}

void JoinMultiplayerScreen::connectToServer(const std::string& ip, int port) {
    minecraft->options->lastMpIp = replaceAll(ip + ":" + std::to_string(port), ":", "_");
    minecraft->options->save();
    minecraft->setScreen(new ConnectScreen(minecraft, ip, port));
}

void JoinMultiplayerScreen::serverSelected(int index) {
    if (index >= 0 && index < (int)serverList.size()) {
        connectToServer(serverList[index].ip, serverList[index].port);
    }
}

void JoinMultiplayerScreen::buttonClicked(Button* button) {
    if (!button->active) return;

    if (button->id == 0) {
        // Join Selected Server
        if (selectedServer >= 0 && selectedServer < (int)serverList.size()) {
            serverSelected(selectedServer);
        }
    } else if (button->id == 1) {
        // Direct Connect Mode
        currentMode = MODE_DIRECT_CONNECT;
        initModeDirectConnect();
    } else if (button->id == 2) {
        // Add Server Mode
        currentMode = MODE_ADD_SERVER;
        initModeAddServer();
    } else if (button->id == 3) {
        // Delete Selected Server
        if (selectedServer >= 0 && selectedServer < (int)serverList.size()) {
            serverList.erase(serverList.begin() + selectedServer);
            selectedServer = -1;
            saveServerList();
            updateButtonState();
        }
    } else if (button->id == 4) {
        // Cancel / Back to Main Menu
        minecraft->setScreen(lastScreen);
    } else if (button->id == 10) {
        // Save New Server
        std::string sName = addServerNameEdit ? trimString(addServerNameEdit->getValue()) : "";
        std::string sAddr = addServerIpEdit ? trimString(addServerIpEdit->getValue()) : "";
        if (sName.empty()) sName = "Minecraft Server";
        if (!sAddr.empty()) {
            std::string ip;
            int port = 25565;
            parseIpPort(sAddr, ip, port);
            serverList.push_back({sName, ip, port});
            saveServerList();
            selectedServer = (int)serverList.size() - 1;
        }
        currentMode = MODE_LIST;
        initModeList();
    } else if (button->id == 11) {
        // Cancel Add Server
        currentMode = MODE_LIST;
        initModeList();
    } else if (button->id == 20) {
        // Direct Connect Submit
        if (directIpEdit) {
            std::string addr = trimString(directIpEdit->getValue());
            if (!addr.empty()) {
                std::string ip;
                int port = 25565;
                parseIpPort(addr, ip, port);
                connectToServer(ip, port);
            }
        }
    } else if (button->id == 21) {
        // Cancel Direct Connect
        currentMode = MODE_LIST;
        initModeList();
    }
}

void JoinMultiplayerScreen::keyPressed(char ch, int eventKey) {
    if (currentMode == MODE_ADD_SERVER) {
        if (addServerNameEdit && addServerNameEdit->inFocus) {
            if (ch == 9 || eventKey == Keyboard::KEY_TAB) {
                addServerNameEdit->focus(false);
                if (addServerIpEdit) addServerIpEdit->focus(true);
                return;
            }
            addServerNameEdit->keyPressed(ch, eventKey);
        } else if (addServerIpEdit && addServerIpEdit->inFocus) {
            if (ch == 9 || eventKey == Keyboard::KEY_TAB) {
                addServerIpEdit->focus(false);
                if (addServerNameEdit) addServerNameEdit->focus(true);
                return;
            }
            if (ch == 13 || eventKey == Keyboard::KEY_RETURN) {
                buttonClicked(btnAddSave);
                return;
            }
            addServerIpEdit->keyPressed(ch, eventKey);
        }
    } else if (currentMode == MODE_DIRECT_CONNECT) {
        if (directIpEdit) {
            directIpEdit->keyPressed(ch, eventKey);
            if (ch == 13 || eventKey == Keyboard::KEY_RETURN) {
                buttonClicked(btnDirectJoin);
                return;
            }
        }
    } else if (currentMode == MODE_LIST) {
        if (eventKey == Keyboard::KEY_ESCAPE) {
            minecraft->setScreen(lastScreen);
        }
    }
}

void JoinMultiplayerScreen::mouseClicked(int x, int y, int buttonNum) {
    Screen::mouseClicked(x, y, buttonNum);

    if (currentMode == MODE_ADD_SERVER) {
        if (addServerNameEdit) addServerNameEdit->mouseClicked(x, y, buttonNum);
        if (addServerIpEdit) addServerIpEdit->mouseClicked(x, y, buttonNum);
    } else if (currentMode == MODE_DIRECT_CONNECT) {
        if (directIpEdit) directIpEdit->mouseClicked(x, y, buttonNum);
    }
}

void JoinMultiplayerScreen::render(int xm, int ym, float a) {
    renderDirtBackground(0);

    if (currentMode == MODE_LIST) {
        if (selectionList != nullptr) {
            selectionList->render(xm, ym, a);
        }
        drawCenteredString(font, "Servidores Multijugador / Servers", width / 2, 14, 0xffffff);
    } else if (currentMode == MODE_ADD_SERVER) {
        drawCenteredString(font, "Añadir Servidor / Add Server", width / 2, 16, 0xffffff);
        int startX = (width - 200) / 2;
        int startY = height / 4;
        drawString(font, "Nombre del Servidor / Server Name:", startX, startY - 2, 0xa0a0a0);
        if (addServerNameEdit) addServerNameEdit->render();
        drawString(font, "Dirección (IP:Puerto) / Server Address:", startX, startY + 48, 0xa0a0a0);
        if (addServerIpEdit) addServerIpEdit->render();
    } else if (currentMode == MODE_DIRECT_CONNECT) {
        drawCenteredString(font, "Conexión Directa / Direct Connect", width / 2, 24, 0xffffff);
        int startX = (width - 200) / 2;
        int startY = height / 3;
        drawString(font, "Dirección del Servidor (IP:Puerto):", startX, startY - 14, 0xa0a0a0);
        if (directIpEdit) directIpEdit->render();
    }

    Screen::render(xm, ym, a);
}

// =========================================================================
// ServerSelectionList Implementation
// =========================================================================

JoinMultiplayerScreen::ServerSelectionList::ServerSelectionList(JoinMultiplayerScreen* parent)
    : ScrolledSelectionList(parent->minecraft, parent->width, parent->height, 32, parent->height - 60, 34) {
    this->parent = parent;
}

int JoinMultiplayerScreen::ServerSelectionList::getNumberOfItems() {
    return (int)parent->serverList.size();
}

void JoinMultiplayerScreen::ServerSelectionList::selectItem(int item, bool doubleClick) {
    parent->selectedServer = item;
    parent->updateButtonState();
    if (doubleClick && item >= 0 && item < (int)parent->serverList.size()) {
        parent->serverSelected(item);
    }
}

bool JoinMultiplayerScreen::ServerSelectionList::isSelectedItem(int item) {
    return item == parent->selectedServer;
}

int JoinMultiplayerScreen::ServerSelectionList::getMaxPosition() {
    return (int)parent->serverList.size() * 34;
}

void JoinMultiplayerScreen::ServerSelectionList::renderBackground() {
    parent->renderBackground();
}

void JoinMultiplayerScreen::ServerSelectionList::renderItem(int i, int x, int y, int h, Tesselator* t) {
    if (i < 0 || i >= (int)parent->serverList.size()) return;
    const ServerEntry& s = parent->serverList[i];
    parent->drawString(parent->font, s.name, x + 2, y + 2, 0xffffff);
    std::string addr = s.ip + ":" + std::to_string(s.port);
    parent->drawString(parent->font, addr, x + 2, y + 16, 0x808080);
}