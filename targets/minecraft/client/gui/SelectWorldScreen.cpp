#include "SelectWorldScreen.h"

#include <stdint.h>
#include <time.h>
#include <wchar.h>

#include <chrono>
#include <ctime>
#include <vector>

#include "Button.h"
#include "ConfirmScreen.h"
#include "CreateWorldScreen.h"
#include "EditBox.h"
#include "RenameWorldScreen.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/Options.h"
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
#include "platform/input/input.h"
#include "util/StringHelpers.h"

SelectWorldScreen::SelectWorldScreen(Screen* lastScreen) {
    title = "Select world";
    done = false;
    selectedWorld = 0;
    worldSelectionList = nullptr;
    isDeleting = false;
    deleteButton = nullptr;
    selectButton = nullptr;
    renameButton = nullptr;
    createButton = nullptr;
    cancelButton = nullptr;
    tabWorldsButton = nullptr;
    tabServersButton = nullptr;
    serverIpEdit = nullptr;
    connectServerButton = nullptr;
    currentTab = TAB_WORLDS;

    this->lastScreen = lastScreen;
}

void SelectWorldScreen::init() {
    Log::info("SelectWorldScreen::init() START\n");
    Language* language = Language::getInstance();
    title = language->getElement("selectWorld.title");

    worldLang = language->getElement("selectWorld.world");
    conversionLang = language->getElement("selectWorld.conversion");
    loadLevelList();

    worldSelectionList = new WorldSelectionList(this);
    worldSelectionList->init(&buttons, BUTTON_UP_ID, BUTTON_DOWN_ID);

    postInit();
}

void SelectWorldScreen::tick() {
    if (currentTab == TAB_SERVERS && serverIpEdit != nullptr) {
        serverIpEdit->tick();
    }
}

void SelectWorldScreen::removed() {
    Keyboard::enableRepeatEvents(false);
}

void SelectWorldScreen::keyPressed(char ch, int eventKey) {
    if (currentTab == TAB_SERVERS && serverIpEdit != nullptr && serverIpEdit->inFocus) {
        serverIpEdit->keyPressed(ch, eventKey);
        if (eventKey == 28 || eventKey == 156) { // Enter key
            buttonClicked(connectServerButton);
        }
    } else {
        Screen::keyPressed(ch, eventKey);
    }
}

void SelectWorldScreen::mouseClicked(int x, int y, int buttonNum) {
    Screen::mouseClicked(x, y, buttonNum);
    if (currentTab == TAB_SERVERS && serverIpEdit != nullptr) {
        serverIpEdit->mouseClicked(x, y, buttonNum);
    }
}

void SelectWorldScreen::loadLevelList() {
    LevelStorageSource* levelSource = minecraft->getLevelSource();
    levelList = levelSource->getLevelList();
    selectedWorld = -1;
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
                          BUTTON_RENAME_ID, width / 2 - 154, height - 28, 70,
                          20, language->getElement("selectWorld.rename")));
    deleteButton->setTextureIcon(32, 0, 16);
    deleteButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::feather)));

    buttons.push_back(renameButton = new Button(
                          BUTTON_DELETE_ID, width / 2 - 74, height - 28, 70, 20,
                          language->getElement("selectWorld.delete")));
    renameButton->setTextureIcon(48, 0, 16);
    renameButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::tnt)));

    buttons.push_back(createButton = new Button(BUTTON_CREATE_ID, width / 2 + 4, height - 52,
                                 150, 20,
                                 language->getElement("selectWorld.create")));
    createButton->setTextureIcon(16, 0, 16);
    createButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance((Tile*)Tile::workbench)));

    buttons.push_back(cancelButton = new Button(BUTTON_CANCEL_ID, width / 2 + 4, height - 28,
                                 150, 20, language->getElement("gui.cancel")));
    cancelButton->setTextureIcon(64, 32, 16);
    cancelButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::door_wood)));

    // Server tab direct connect elements
    std::string ip = replaceAll(minecraft->options->lastMpIp, "_", ":");
    serverIpEdit = new EditBox(this, font, width / 2 - 120, height / 2 - 20, 240, 22, ip);
    serverIpEdit->setMaxLength(128);

    buttons.push_back(connectServerButton = new Button(BUTTON_CONNECT_SERVER_ID, width / 2 - 120, height / 2 + 10, 240, 24, "Conectar al Servidor"));
    connectServerButton->setTextureIcon(64, 0, 16);
    connectServerButton->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::enderPearl)));

    updateTabVisibility();
}

void SelectWorldScreen::updateTabVisibility() {
    bool isWorlds = (currentTab == TAB_WORLDS);
    if (tabWorldsButton) tabWorldsButton->active = !isWorlds;
    if (tabServersButton) tabServersButton->active = isWorlds;

    if (selectButton) selectButton->visible = isWorlds;
    if (deleteButton) deleteButton->visible = isWorlds;
    if (renameButton) renameButton->visible = isWorlds;
    if (createButton) createButton->visible = isWorlds;

    if (isWorlds) {
        bool hasSelection = (selectedWorld >= 0 && levelList != nullptr && selectedWorld < (int)levelList->size());
        if (selectButton) selectButton->active = hasSelection;
        if (deleteButton) deleteButton->active = hasSelection;
        if (renameButton) renameButton->active = hasSelection;
        if (cancelButton) {
            cancelButton->x = width / 2 + 4;
            cancelButton->y = height - 28;
            cancelButton->w = 150;
        }
    } else {
        if (cancelButton) {
            cancelButton->x = width / 2 - 60;
            cancelButton->y = height / 2 + 42;
            cancelButton->w = 120;
        }
    }

    if (connectServerButton) connectServerButton->visible = !isWorlds;
    if (serverIpEdit != nullptr) {
        serverIpEdit->inFocus = !isWorlds;
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
        if (serverIpEdit) {
            std::string ip = trimString(serverIpEdit->getValue());
            if (!ip.empty()) {
                minecraft->options->lastMpIp = replaceAll(ip, ":", "_");
                minecraft->options->save();

                std::string host = ip;
                int port = 25565;
                size_t colonPos = ip.find(':');
                if (colonPos != std::string::npos) {
                    host = ip.substr(0, colonPos);
                    try {
                        port = std::stoi(ip.substr(colonPos + 1));
                    } catch (...) {
                        port = 25565;
                    }
                }
                minecraft->setScreen(new ConnectScreen(this, host, port));
            }
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
        if (worldSelectionList) {
            worldSelectionList->buttonClicked(button);
        }
    }
}

void SelectWorldScreen::worldSelected(int id) {
    minecraft->setScreen(nullptr);
    if (done) return;
    done = true;
    minecraft->gameMode = nullptr;

    std::string worldFolderName = getWorldId(id);
    if (worldFolderName == "")
    {
        worldFolderName = "World" + toWString<int>(id);
    }
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
        drawCenteredString(font, "Direccion del Servidor / Server IP", width / 2, height / 2 - 35, 0xa0a0a0);
        if (serverIpEdit != nullptr) {
            serverIpEdit->render();
        }
    }

    Screen::render(xm, ym, a);
}

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
    bool active = (this->parent->selectedWorld >= 0 &&
                   this->parent->selectedWorld < getNumberOfItems());
    parent->selectButton->active = active;
    parent->deleteButton->active = active;
    parent->renameButton->active = active;

    if (doubleClick && active) {
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
    parent->renderBackground();  // 4J - was
                                 // SelectWorldScreen.this.renderBackground();
}

void SelectWorldScreen::WorldSelectionList::renderItem(int i, int x, int y,
                                                       int h, Tesselator* t) {
    LevelSummary* levelSummary = parent->levelList->at(i);

    std::string name = levelSummary->getLevelName();
    if (name.length() == 0) {
        name = parent->worldLang + " " + toWString<int>(i + 1);
    }

    std::string id = levelSummary->getLevelId();

    // levelSummary->getLastPlayed() is milliseconds since the FILETIME
    // epoch (1601-01-01 UTC). Convert to chrono::system_clock (1970
    // epoch) by subtracting the constant offset, then break down with
    // gmtime_r for display.
    constexpr int64_t kFileTimeEpochToUnixEpochMs = 11644473600000LL;
    const int64_t lastPlayedUnixMs =
        levelSummary->getLastPlayed() - kFileTimeEpochToUnixEpochMs;
    const auto tp = std::chrono::system_clock::time_point{
        std::chrono::milliseconds{lastPlayedUnixMs}};
    auto dp = std::chrono::floor<std::chrono::days>(tp);
    std::chrono::year_month_day ymd{dp};
    std::chrono::hh_mm_ss hms{
        std::chrono::floor<std::chrono::minutes>(tp - dp)};

    // 4J Stu - Currently shows years as 4 digits, where java only showed 2
    // 4J - TODO Localise this
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
