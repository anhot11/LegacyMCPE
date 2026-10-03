#include "SelectWorldScreen.h"

#include <stdint.h>
#include <time.h>
#include <wchar.h>

#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "Button.h"
#include "ConfirmScreen.h"
#include "CreateWorldScreen.h"
#include "EditBox.h"
#include "EditServerScreen.h"
#include "RenameWorldScreen.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/Options.h"
#include "minecraft/client/gui/Font.h"
#include "minecraft/client/gui/Screen.h"
#include "minecraft/client/gui/ScrolledSelectionList.h"
#include "minecraft/client/multiplayer/ConnectScreen.h"
#include "minecraft/locale/Language.h"
#include "minecraft/util/Log.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/storage/LevelStorageSource.h"
#include "minecraft/world/level/storage/LevelSummary.h"
#include "minecraft/world/level/tile/Tile.h"
#include "MessageScreen.h"
#include "minecraft/GameEnums.h"
#include "minecraft/IGameServices.h"
#include "minecraft/network/INetworkService.h"
#include "platform/fs/fs.h"
#include "platform/storage/storage.h"
#include "app/common/Network/GameNetworkManager.h"
#include "app/common/UI/All Platforms/UIStructs.h"
#include "app/common/UI/ConsoleUIController.h"
#include "minecraft/SharedConstants.h"
#include "platform/network/NetTypes.h"
#include "platform/input/input.h"
#include "util/StringHelpers.h"

static std::filesystem::path getServersFilePath() {
    std::filesystem::path base = PlatformFilesystem.getBasePath();
    return base / "servers.txt";
}

static void executePing(ServerData* s) {
    if (!s) return;

    struct addrinfo hints = {};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo* res = nullptr;
    std::string portStr = std::to_string(s->port);
    if (getaddrinfo(s->ip.c_str(), portStr.c_str(), &hints, &res) != 0 || res == nullptr) {
        s->isOnline = false;
        s->pingDone = true;
        s->motd = "Host no encontrado";
        return;
    }

    bool pingSuccess = false;
    auto start = std::chrono::steady_clock::now();

    // 1. Try Java 0xFE 0x01 TCP Ping
    int sock = socket(res->ai_family, SOCK_STREAM, 0);
    if (sock >= 0) {
        int flags = fcntl(sock, F_GETFL, 0);
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);

        connect(sock, res->ai_addr, res->ai_addrlen);

        struct pollfd pfd;
        pfd.fd = sock;
        pfd.events = POLLOUT;
        int ret = poll(&pfd, 1, 1200);
        if (ret > 0 && (pfd.revents & POLLOUT)) {
            int err = 0;
            socklen_t len = sizeof(err);
            getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &len);
            if (err == 0) {
                uint8_t pingReq[2] = {0xFE, 0x01};
                send(sock, pingReq, 2, 0);

                pfd.events = POLLIN;
                ret = poll(&pfd, 1, 1200);
                if (ret > 0 && (pfd.revents & POLLIN)) {
                    uint8_t header[3];
                    int n = recv(sock, header, 3, 0);
                    if (n >= 3 && header[0] == 0xFF) {
                        uint16_t strChars = (header[1] << 8) | header[2];
                        int byteCount = strChars * 2;
                        std::vector<uint8_t> strBuf(byteCount);
                        int received = 0;
                        while (received < byteCount) {
                            int r = recv(sock, strBuf.data() + received, byteCount - received, 0);
                            if (r <= 0) break;
                            received += r;
                        }
                        if (received == byteCount) {
                            std::string fullStr;
                            for (size_t i = 0; i + 1 < strBuf.size(); i += 2) {
                                uint16_t u = (strBuf[i] << 8) | strBuf[i + 1];
                                if (u == 0) {
                                    fullStr.push_back('\0');
                                } else if (u < 128) {
                                    fullStr.push_back((char)u);
                                } else {
                                    fullStr.push_back('?');
                                }
                            }
                            auto finish = std::chrono::steady_clock::now();
                            s->pingMs = (int)std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();

                            std::vector<std::string> tokens;
                            std::string token;
                            for (char c : fullStr) {
                                if (c == '\0') {
                                    tokens.push_back(token);
                                    token.clear();
                                } else {
                                    token.push_back(c);
                                }
                            }
                            tokens.push_back(token);

                            if (tokens.size() >= 6 && tokens[0] == "\xa7\x31") {
                                s->version = tokens[2];
                                s->motd = tokens[3];
                                try { s->players = std::stoi(tokens[4]); } catch (...) {}
                                try { s->maxPlayers = std::stoi(tokens[5]); } catch (...) {}
                                s->isOnline = true;
                                pingSuccess = true;
                            } else if (tokens.size() >= 1) {
                                s->motd = tokens[0];
                                s->isOnline = true;
                                pingSuccess = true;
                            }
                        }
                    }
                }
            }
        }
        close(sock);
    }

    // 2. Try Bedrock UDP RakNet Unconnected Ping
    if (!pingSuccess) {
        int uSock = socket(res->ai_family, SOCK_DGRAM, 0);
        if (uSock >= 0) {
            int flags = fcntl(uSock, F_GETFL, 0);
            fcntl(uSock, F_SETFL, flags | O_NONBLOCK);

            uint8_t raknetPing[25] = {0};
            raknetPing[0] = 0x01; // ID_UNCONNECTED_PING
            static const uint8_t rakMagic[16] = {0x00, 0xff, 0xff, 0x00, 0xfe, 0xfe, 0xfe, 0xfe, 0xfd, 0xfd, 0xfd, 0xfd, 0x12, 0x34, 0x56, 0x78};
            memcpy(&raknetPing[9], rakMagic, 16);

            auto uStart = std::chrono::steady_clock::now();
            sendto(uSock, raknetPing, 25, 0, res->ai_addr, res->ai_addrlen);

            struct pollfd pfd;
            pfd.fd = uSock;
            pfd.events = POLLIN;
            int ret = poll(&pfd, 1, 1200);
            if (ret > 0 && (pfd.revents & POLLIN)) {
                uint8_t respBuf[1024];
                int n = recvfrom(uSock, respBuf, sizeof(respBuf), 0, nullptr, nullptr);
                if (n >= 35 && respBuf[0] == 0x1c) {
                    uint16_t strLen = (respBuf[33] << 8) | respBuf[34];
                    if (35 + strLen <= (uint16_t)n) {
                        std::string pongStr((char*)&respBuf[35], strLen);
                        auto uFinish = std::chrono::steady_clock::now();
                        s->pingMs = (int)std::chrono::duration_cast<std::chrono::milliseconds>(uFinish - uStart).count();

                        std::vector<std::string> tokens;
                        std::stringstream pss(pongStr);
                        std::string t;
                        while (std::getline(pss, t, ';')) {
                            tokens.push_back(t);
                        }
                        if (tokens.size() >= 6) {
                            s->motd = tokens[1];
                            s->version = tokens[3];
                            try { s->players = std::stoi(tokens[4]); } catch (...) {}
                            try { s->maxPlayers = std::stoi(tokens[5]); } catch (...) {}
                            s->isOnline = true;
                            pingSuccess = true;
                        }
                    }
                }
            }
            close(uSock);
        }
    }

    freeaddrinfo(res);
    s->isOnline = pingSuccess;
    if (!pingSuccess) {
        s->motd = "Offline";
        s->pingMs = -1;
    }
    s->pingDone = true;
}

SelectWorldScreen::SelectWorldScreen(Screen* lastScreen) {
    title = "Select world";
    done = false;
    selectedWorld = 0;
    worldSelectionList = nullptr;
    serverSelectionList = nullptr;
    isDeleting = false;
    deleteButton = nullptr;
    selectButton = nullptr;
    renameButton = nullptr;
    createButton = nullptr;
    cancelButton = nullptr;
    tabWorldsButton = nullptr;
    tabServersButton = nullptr;
    connectServerButton = nullptr;
    addServerButton = nullptr;
    deleteServerButton = nullptr;
    editServerButton = nullptr;
    selectedServer = -1;
    currentTab = TAB_WORLDS;

    this->lastScreen = lastScreen;
}

SelectWorldScreen::~SelectWorldScreen() {
    if (worldSelectionList != nullptr) {
        delete worldSelectionList;
        worldSelectionList = nullptr;
    }
    if (serverSelectionList != nullptr) {
        delete serverSelectionList;
        serverSelectionList = nullptr;
    }
}

void SelectWorldScreen::init() {
    Log::info("SelectWorldScreen::init() START\n");
    Language* language = Language::getInstance();
    title = language->getElement("selectWorld.title");

    worldLang = language->getElement("selectWorld.world");
    conversionLang = language->getElement("selectWorld.conversion");
    loadLevelList();
    loadServers();

    worldSelectionList = new WorldSelectionList(this);
    worldSelectionList->init(&buttons, BUTTON_UP_ID, BUTTON_DOWN_ID);

    serverSelectionList = new ServerSelectionList(this);
    serverSelectionList->init(&buttons, BUTTON_UP_ID, BUTTON_DOWN_ID);

    postInit();
}

void SelectWorldScreen::tick() {}

void SelectWorldScreen::removed() {
    Keyboard::enableRepeatEvents(false);
}

void SelectWorldScreen::keyPressed(char ch, int eventKey) {
    Screen::keyPressed(ch, eventKey);
}

void SelectWorldScreen::mouseClicked(int x, int y, int buttonNum) {
    Screen::mouseClicked(x, y, buttonNum);
}

void SelectWorldScreen::loadLevelList() {
    LevelStorageSource* levelSource = minecraft->getLevelSource();
    levelList = levelSource->getLevelList();
    selectedWorld = -1;
}

void SelectWorldScreen::loadServers() {
    serverList.clear();
    std::filesystem::path path = getServersFilePath();
    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            line = trimString(line);
            if (line.empty() || line[0] == '#') continue;

            std::stringstream ss(line);
            std::string name, ip, portStr;
            if (std::getline(ss, name, '|') && std::getline(ss, ip, '|')) {
                int port = 25565;
                if (std::getline(ss, portStr, '|')) {
                    try { port = std::stoi(trimString(portStr)); } catch (...) {}
                }
                ServerData s;
                s.name = trimString(name);
                s.ip = trimString(ip);
                s.port = port;
                s.motd = "Buscando servidor...";
                s.version = "";
                s.pingMs = -1;
                s.players = 0;
                s.maxPlayers = 0;
                s.isOnline = false;
                s.pingDone = false;
                serverList.push_back(s);
            }
        }
        file.close();
    }

    if (serverList.empty()) {
        ServerData s;
        s.name = "Servidor Local";
        s.ip = "127.0.0.1";
        s.port = 25565;
        s.motd = "Servidor local";
        s.version = "";
        s.pingMs = -1;
        s.players = 0;
        s.maxPlayers = 0;
        s.isOnline = false;
        s.pingDone = false;
        serverList.push_back(s);
        saveServers();
    }

    selectedServer = -1;
    refreshServerPings();
}

void SelectWorldScreen::saveServers() {
    std::filesystem::path path = getServersFilePath();
    std::ofstream file(path);
    if (file.is_open()) {
        for (const auto& s : serverList) {
            file << s.name << "|" << s.ip << "|" << s.port << "\n";
        }
        file.close();
    }
}

void SelectWorldScreen::refreshServerPings() {
    std::thread([this]() {
        for (size_t i = 0; i < serverList.size(); i++) {
            executePing(&serverList[i]);
        }
    }).detach();
}

void SelectWorldScreen::addServer(const std::string& name, const std::string& ip, int port) {
    ServerData s;
    s.name = name;
    s.ip = ip;
    s.port = port;
    s.motd = "Buscando servidor...";
    s.version = "";
    s.pingMs = -1;
    s.players = 0;
    s.maxPlayers = 0;
    s.isOnline = false;
    s.pingDone = false;
    serverList.push_back(s);
    saveServers();
    selectedServer = (int)serverList.size() - 1;
    refreshServerPings();
    updateTabVisibility();
}

void SelectWorldScreen::updateServer(int index, const std::string& name, const std::string& ip, int port) {
    if (index >= 0 && index < (int)serverList.size()) {
        serverList[index].name = name;
        serverList[index].ip = ip;
        serverList[index].port = port;
        serverList[index].motd = "Buscando servidor...";
        serverList[index].isOnline = false;
        serverList[index].pingDone = false;
        saveServers();
        refreshServerPings();
        updateTabVisibility();
    }
}

void SelectWorldScreen::deleteServer(int index) {
    if (index >= 0 && index < (int)serverList.size()) {
        serverList.erase(serverList.begin() + index);
        saveServers();
        selectedServer = -1;
        updateTabVisibility();
    }
}

void SelectWorldScreen::serverSelected(int id) {
    if (id < 0 || id >= (int)serverList.size()) return;
    const ServerData& s = serverList[id];
    minecraft->options->lastMpIp = replaceAll(s.ip + ":" + std::to_string(s.port), ":", "_");
    minecraft->options->save();
    minecraft->setScreen(new ConnectScreen(minecraft, s.ip, s.port));
}

std::string SelectWorldScreen::getWorldId(int id) {
    return levelList->at(id)->getLevelId();
}

std::string SelectWorldScreen::getWorldName(int id) {
    std::string levelName = levelList->at(id)->getLevelName();

    if (levelName.length() == 0) {
        Language* language = Language::getInstance();
        levelName = language->getElement("selectWorld.world") + " " +
                    toWString<int>(id + 1);
    }

    return levelName;
}

void SelectWorldScreen::postInit() {
    Language* language = Language::getInstance();

    int tabW = 120;
    int tabH = 22;
    int tabY = 6;

    // Top Tabs: Mundos / Servidores
    buttons.push_back(tabWorldsButton = new Button(BUTTON_TAB_WORLDS_ID, width / 2 - tabW - 4, tabY, tabW, tabH, "Mundos"));
    tabWorldsButton->setTextureIcon(0, 16, 16);
    tabWorldsButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::grass)));

    buttons.push_back(tabServersButton = new Button(BUTTON_TAB_SERVERS_ID, width / 2 + 4, tabY, tabW, tabH, "Servidores"));
    tabServersButton->setTextureIcon(16, 16, 16);
    tabServersButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::compass)));

    // World list action buttons (bottom)
    buttons.push_back(selectButton = new Button(
                          BUTTON_SELECT_ID, width / 2 - 154, height - 52, 150,
                          20, language->getElement("selectWorld.select")));
    selectButton->setTextureIcon(0, 0, 16);
    selectButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::arrow)));

    buttons.push_back(deleteButton = new Button(
                          BUTTON_DELETE_ID, width / 2 - 154, height - 28, 70,
                          20, language->getElement("selectWorld.delete")));
    deleteButton->setTextureIcon(32, 0, 16);
    deleteButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::feather)));

    buttons.push_back(renameButton = new Button(
                          BUTTON_RENAME_ID, width / 2 - 74, height - 28, 70, 20,
                          language->getElement("selectWorld.rename")));
    renameButton->setTextureIcon(48, 0, 16);
    renameButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::tnt)));

    buttons.push_back(createButton = new Button(BUTTON_CREATE_ID, width / 2 + 4, height - 52,
                                 150, 20,
                                 language->getElement("selectWorld.create")));
    createButton->setTextureIcon(16, 0, 16);
    createButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::workBench)));

    buttons.push_back(cancelButton = new Button(BUTTON_CANCEL_ID, width / 2 + 4, height - 28,
                                 150, 20, language->getElement("gui.cancel")));
    cancelButton->setTextureIcon(64, 32, 16);
    cancelButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::door_wood)));

    // Server tab action buttons (bottom)
    buttons.push_back(connectServerButton = new Button(BUTTON_CONNECT_SERVER_ID, width / 2 - 154, height - 52, 150, 20, "Unirse al Servidor"));
    connectServerButton->setTextureIcon(64, 0, 16);
    connectServerButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::enderPearl)));

    buttons.push_back(addServerButton = new Button(BUTTON_ADD_SERVER_ID, width / 2 + 4, height - 52, 150, 20, "Agregar Servidor"));
    addServerButton->setTextureIcon(16, 0, 16);
    addServerButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::workBench)));

    buttons.push_back(deleteServerButton = new Button(BUTTON_DELETE_SERVER_ID, width / 2 - 154, height - 28, 70, 20, "Eliminar"));
    deleteServerButton->setTextureIcon(32, 0, 16);
    deleteServerButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::feather)));

    buttons.push_back(editServerButton = new Button(BUTTON_EDIT_SERVER_ID, width / 2 - 74, height - 28, 70, 20, "Editar"));
    editServerButton->setTextureIcon(48, 0, 16);
    editServerButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::tnt)));

    updateTabVisibility();
}

void SelectWorldScreen::updateTabVisibility() {
    bool isWorlds = (currentTab == TAB_WORLDS);
    int tabW = 120;
    int tabH = 22;
    int tabY = 6;

    if (tabWorldsButton) {
        tabWorldsButton->x = width / 2 - tabW - 4;
        tabWorldsButton->y = tabY;
        tabWorldsButton->w = tabW;
        tabWorldsButton->h = tabH;
        tabWorldsButton->active = !isWorlds;
    }
    if (tabServersButton) {
        tabServersButton->x = width / 2 + 4;
        tabServersButton->y = tabY;
        tabServersButton->w = tabW;
        tabServersButton->h = tabH;
        tabServersButton->active = isWorlds;
    }

    // Worlds controls
    bool hasSelection = (selectedWorld >= 0 && levelList != nullptr && selectedWorld < (int)levelList->size());
    if (selectButton) {
        selectButton->visible = isWorlds;
        selectButton->active = isWorlds && hasSelection;
    }
    if (createButton) {
        createButton->visible = isWorlds;
        createButton->active = isWorlds;
    }
    if (deleteButton) {
        deleteButton->visible = isWorlds;
        deleteButton->active = isWorlds && hasSelection;
    }
    if (renameButton) {
        renameButton->visible = isWorlds;
        renameButton->active = isWorlds && hasSelection;
    }

    // Server controls
    bool hasServerSelection = (selectedServer >= 0 && selectedServer < (int)serverList.size());
    if (connectServerButton) {
        connectServerButton->visible = !isWorlds;
        connectServerButton->active = !isWorlds && hasServerSelection;
    }
    if (addServerButton) {
        addServerButton->visible = !isWorlds;
        addServerButton->active = !isWorlds;
    }
    if (deleteServerButton) {
        deleteServerButton->visible = !isWorlds;
        deleteServerButton->active = !isWorlds && hasServerSelection;
    }
    if (editServerButton) {
        editServerButton->visible = !isWorlds;
        editServerButton->active = !isWorlds && hasServerSelection;
    }

    // Cancel / Back button
    if (cancelButton) {
        cancelButton->visible = true;
        cancelButton->active = true;
        cancelButton->x = width / 2 + 4;
        cancelButton->y = height - 28;
        cancelButton->w = 150;
        cancelButton->h = 20;
    }
}

void SelectWorldScreen::buttonClicked(Button* button) {
    Log::info("SelectWorldScreen::buttonClicked START\n");
    if (!button->active) return;

    if (button->id == BUTTON_TAB_WORLDS_ID) {
        currentTab = TAB_WORLDS;
        updateTabVisibility();
        return;
    }
    if (button->id == BUTTON_TAB_SERVERS_ID) {
        currentTab = TAB_SERVERS;
        updateTabVisibility();
        return;
    }
    if (button->id == BUTTON_CONNECT_SERVER_ID) {
        if (selectedServer >= 0 && selectedServer < (int)serverList.size()) {
            serverSelected(selectedServer);
        }
        return;
    }
    if (button->id == BUTTON_ADD_SERVER_ID) {
        minecraft->setScreen(new EditServerScreen(this, -1));
        return;
    }
    if (button->id == BUTTON_EDIT_SERVER_ID) {
        if (selectedServer >= 0 && selectedServer < (int)serverList.size()) {
            const ServerData& s = serverList[selectedServer];
            std::string addr = s.port == 25565 ? s.ip : (s.ip + ":" + std::to_string(s.port));
            minecraft->setScreen(new EditServerScreen(this, selectedServer, s.name, addr));
        }
        return;
    }
    if (button->id == BUTTON_DELETE_SERVER_ID) {
        if (selectedServer >= 0 && selectedServer < (int)serverList.size()) {
            deleteServer(selectedServer);
        }
        return;
    }

    if (button->id == BUTTON_DELETE_ID) {
        std::string worldName = getWorldName(selectedWorld);
        if (worldName != "") {
            isDeleting = true;

            Language* language = Language::getInstance();
            std::string title =
                language->getElement("selectWorld.deleteQuestion");
            std::string warning =
                "'" + worldName + "' " +
                language->getElement("selectWorld.deleteWarning");
            std::string yes = language->getElement("selectWorld.deleteButton");
            std::string no = language->getElement("gui.cancel");

            ConfirmScreen* confirmScreen =
                new ConfirmScreen(this, title, warning, yes, no, selectedWorld);
            minecraft->setScreen(confirmScreen);
        }
    } else if (button->id == BUTTON_SELECT_ID) {
        worldSelected(selectedWorld);
    } else if (button->id == BUTTON_CREATE_ID) {
        minecraft->setScreen(new CreateWorldScreen(this));
    } else if (button->id == BUTTON_RENAME_ID) {
        minecraft->setScreen(
            new RenameWorldScreen(this, getWorldId(selectedWorld)));
    } else if (button->id == BUTTON_CANCEL_ID) {
        Log::info(
            "SelectWorldScreen::buttonClicked 'Cancel' "
            "minecraft->setScreen(lastScreen)\n");
        minecraft->setScreen(lastScreen);
    } else {
        if (currentTab == TAB_WORLDS && worldSelectionList) {
            worldSelectionList->buttonClicked(button);
        } else if (currentTab == TAB_SERVERS && serverSelectionList) {
            serverSelectionList->buttonClicked(button);
        }
    }
}

void SelectWorldScreen::worldSelected(int id) {
    if (done) return;
    done = true;

    std::string worldFolderName = getWorldId(id);
    std::string worldName = getWorldName(id);
    if (worldName.empty()) {
        worldName = worldFolderName.empty() ? ("World" + toWString<int>(id)) : worldFolderName;
    }

    PlatformStorage.ResetSaveData();
    PlatformStorage.SetSaveTitle((char*)worldName.c_str());

    NetworkGameInitData* param = new NetworkGameInitData();
    param->seed = 0;
    param->saveData = nullptr;
    param->texturePackId = 0;
    param->settings = gameServices().getGameHostOption(eGameHostOption_All);
    param->xzSize = LEVEL_MAX_WIDTH;
    param->hellScale = HELL_LEVEL_MAX_SCALE;

    NetworkService.HostGame(0, false, false, MINECRAFT_NET_MAX_PLAYERS, 0);
    NetworkService.FakeLocalPlayerJoined();

    LoadingInputParams* loadingParams = new LoadingInputParams();
    loadingParams->func = &CGameNetworkManager::RunNetworkGameThreadProc;
    loadingParams->lpParam = param;

    gameServices().setAutosaveTimerTime();

    UIFullscreenProgressCompletionData* completionData =
        new UIFullscreenProgressCompletionData();
    completionData->bShowBackground = true;
    completionData->bShowLogo = true;
    completionData->type = e_ProgressCompletion_CloseAllPlayersUIScenes;
    completionData->iPad = 0;
    loadingParams->completionData = completionData;

    ui.NavigateToScene(0, eUIScene_FullscreenProgress, loadingParams);
    Language* language = Language::getInstance();
    minecraft->setScreen(
        new MessageScreen(language->getElement("menu.generatingLevel")));
}

void SelectWorldScreen::confirmResult(bool result, int id) {
    if (isDeleting) {
        isDeleting = false;
        if (result) {
            LevelStorageSource* levelSource = minecraft->getLevelSource();
            levelSource->clearAll();
            levelSource->deleteLevel(getWorldId(id));

            loadLevelList();
        }
        minecraft->setScreen(this);
    }
}

void SelectWorldScreen::render(int xm, int ym, float a) {
    renderDirtBackground(0);

    if (currentTab == TAB_WORLDS) {
        if (worldSelectionList != nullptr) {
            worldSelectionList->render(xm, ym, a);
        }
    } else {
        if (serverSelectionList != nullptr) {
            serverSelectionList->render(xm, ym, a);
        }
    }

    Screen::render(xm, ym, a);
}

// =========================================================================
// WorldSelectionList Implementation
// =========================================================================

SelectWorldScreen::WorldSelectionList::WorldSelectionList(
    SelectWorldScreen* sws)
    : ScrolledSelectionList(sws->minecraft, sws->width, sws->height, 32,
                            sws->height - 64, 36) {
    parent = sws;
}

int SelectWorldScreen::WorldSelectionList::getNumberOfItems() {
    return (int)this->parent->levelList->size();
}

void SelectWorldScreen::WorldSelectionList::selectItem(int item,
                                                       bool doubleClick) {
    parent->selectedWorld = item;
    parent->updateTabVisibility();

    if (doubleClick && item >= 0 && item < getNumberOfItems()) {
        parent->worldSelected(item);
    }
}

bool SelectWorldScreen::WorldSelectionList::isSelectedItem(int item) {
    return item == parent->selectedWorld;
}

int SelectWorldScreen::WorldSelectionList::getMaxPosition() {
    return (int)parent->levelList->size() * 36;
}

void SelectWorldScreen::WorldSelectionList::renderBackground() {
    parent->renderBackground();
}

void SelectWorldScreen::WorldSelectionList::renderItem(int i, int x, int y,
                                                       int h, Tesselator* t) {
    LevelSummary* levelSummary = parent->levelList->at(i);

    std::string name = levelSummary->getLevelName();
    if (name.length() == 0) {
        name = parent->worldLang + " " + toWString<int>(i + 1);
    }

    std::string id = levelSummary->getLevelId();

    constexpr int64_t kFileTimeEpochToUnixEpochMs = 11644473600000LL;
    const int64_t lastPlayedUnixMs =
        levelSummary->getLastPlayed() - kFileTimeEpochToUnixEpochMs;
    const auto tp = std::chrono::system_clock::time_point{
        std::chrono::milliseconds{lastPlayedUnixMs}};
    auto dp = std::chrono::floor<std::chrono::days>(tp);
    std::chrono::year_month_day ymd{dp};
    std::chrono::hh_mm_ss hms{
        std::chrono::floor<std::chrono::minutes>(tp - dp)};

    id += std::format(" ({}/{}/{} {}:{:02d}", (unsigned)ymd.day(),
                      (unsigned)ymd.month(), (int)ymd.year(),
                      (int)hms.hours().count(), (int)hms.minutes().count());

    int64_t size = levelSummary->getSizeOnDisk();
    id = id + ", " + toWString<float>(size / 1024 * 100 / 1024 / 100.0f) +
         " MB)";
    std::string info;

    if (levelSummary->isRequiresConversion()) {
        info = parent->conversionLang + " " + info;
    }

    parent->drawString(parent->font, name, x + 2, y + 1, 0xffffff);
    parent->drawString(parent->font, id, x + 2, y + 12, 0x808080);
    parent->drawString(parent->font, info, x + 2, y + 12 + 10, 0x808080);
}

// =========================================================================
// ServerSelectionList Implementation
// =========================================================================

SelectWorldScreen::ServerSelectionList::ServerSelectionList(
    SelectWorldScreen* sws)
    : ScrolledSelectionList(sws->minecraft, sws->width, sws->height, 32,
                            sws->height - 64, 36) {
    parent = sws;
}

int SelectWorldScreen::ServerSelectionList::getNumberOfItems() {
    return (int)this->parent->serverList.size();
}

void SelectWorldScreen::ServerSelectionList::selectItem(int item,
                                                        bool doubleClick) {
    parent->selectedServer = item;
    parent->updateTabVisibility();

    if (doubleClick && item >= 0 && item < getNumberOfItems()) {
        parent->serverSelected(item);
    }
}

bool SelectWorldScreen::ServerSelectionList::isSelectedItem(int item) {
    return item == parent->selectedServer;
}

int SelectWorldScreen::ServerSelectionList::getMaxPosition() {
    return (int)parent->serverList.size() * 36;
}

void SelectWorldScreen::ServerSelectionList::renderBackground() {
    parent->renderBackground();
}

void SelectWorldScreen::ServerSelectionList::renderItem(int i, int x, int y,
                                                        int h, Tesselator* t) {
    if (i < 0 || i >= (int)parent->serverList.size()) return;
    const ServerData& s = parent->serverList[i];

    // 1. Server Name on top left
    parent->drawString(parent->font, s.name, x + 2, y + 1, 0xffffff);

    // 2. Right side status
    if (s.isOnline) {
        std::string pingStr = std::to_string(s.pingMs) + "ms";
        std::string playerStr = std::to_string(s.players) + "/" + std::to_string(s.maxPlayers);
        int pW = parent->font->width(pingStr);
        int plW = parent->font->width(playerStr);
        parent->drawString(parent->font, playerStr, x + 215 - plW, y + 1, 0xaaaaaa);
        parent->drawString(parent->font, pingStr, x + 215 - plW - pW - 6, y + 1, 0x55ff55);
    } else {
        std::string offStr = "[X] Offline";
        int oW = parent->font->width(offStr);
        parent->drawString(parent->font, offStr, x + 215 - oW, y + 1, 0xff5555);
    }

    // 3. MOTD
    std::string motd = s.isOnline ? s.motd : "No se puede conectar al servidor";
    if (motd.length() > 36) motd = motd.substr(0, 33) + "...";
    parent->drawString(parent->font, motd, x + 2, y + 12, s.isOnline ? 0xdddddd : 0x888888);

    // 4. Version + Address
    std::string addrStr = s.ip + ":" + std::to_string(s.port);
    std::string line3 = s.isOnline ? (s.version + " (" + addrStr + ")") : addrStr;
    parent->drawString(parent->font, line3, x + 2, y + 22, 0x777777);
}
