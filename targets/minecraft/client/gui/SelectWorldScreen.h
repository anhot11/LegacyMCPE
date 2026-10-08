#pragma once
#include <format>
#include <string>
#include <vector>

#include "Screen.h"
#include "ScrolledSelectionList.h"

class LevelSummary;
class Button;
class EditBox;
class Tesselator;

struct ServerData {
    std::string name;
    std::string ip;
    int port;

    // Ping info
    std::string motd;
    std::string version;
    int pingMs;         // -1 = pending or offline
    int players;
    int maxPlayers;
    bool isOnline;
    bool pingDone;
};

class SelectWorldScreen : public Screen {
public:
    class WorldSelectionList;
    class ServerSelectionList;

    enum Tab {
        TAB_WORLDS = 0,
        TAB_SERVERS = 1
    };

protected:
    static const int BUTTON_CANCEL_ID = 0;
    static const int BUTTON_SELECT_ID = 1;
    static const int BUTTON_DELETE_ID = 2;
    static const int BUTTON_CREATE_ID = 3;
    static const int BUTTON_UP_ID = 4;
    static const int BUTTON_DOWN_ID = 5;
    static const int BUTTON_RENAME_ID = 6;
    static const int BUTTON_TAB_WORLDS_ID = 10;
    static const int BUTTON_TAB_SERVERS_ID = 11;
    static const int BUTTON_CONNECT_SERVER_ID = 12;
    static const int BUTTON_ADD_SERVER_ID = 13;
    static const int BUTTON_DELETE_SERVER_ID = 14;
    static const int BUTTON_EDIT_SERVER_ID = 15;

protected:
    Screen* lastScreen;
    std::string title;
    Tab currentTab;

private:
    bool done = false;
    int selectedWorld = 0;
    std::vector<LevelSummary*>* levelList = nullptr;
    WorldSelectionList* worldSelectionList = nullptr;
    std::string worldLang;
    std::string conversionLang;
    bool isDeleting = false;

    Button* deleteButton = nullptr;
    Button* selectButton = nullptr;
    Button* renameButton = nullptr;
    Button* createButton = nullptr;
    Button* cancelButton = nullptr;
    Button* tabWorldsButton = nullptr;
    Button* tabServersButton = nullptr;

    // Server tab controls
    ServerSelectionList* serverSelectionList = nullptr;
    std::vector<ServerData> serverList;
    int selectedServer = -1;
    Button* connectServerButton = nullptr;
    Button* addServerButton = nullptr;
    Button* deleteServerButton = nullptr;
    Button* editServerButton = nullptr;

public:
    SelectWorldScreen(Screen* lastScreen);
    virtual ~SelectWorldScreen();
    virtual void init() override;
    virtual void tick() override;
    virtual void removed() override;

private:
    void loadLevelList();
    void updateTabVisibility();

public:
    void loadServers();
    void saveServers();
    void refreshServerPings();
    void addServer(const std::string& name, const std::string& ip, int port);
    void updateServer(int index, const std::string& name, const std::string& ip, int port);
    void deleteServer(int index);
    void serverSelected(int id);

protected:
    std::string getWorldId(int id);
    std::string getWorldName(int id);

public:
    virtual void postInit();

protected:
    virtual void buttonClicked(Button* button) override;
    virtual void keyPressed(char ch, int eventKey) override;
    virtual void mouseClicked(int x, int y, int buttonNum) override;

public:
    void worldSelected(int id);
    void confirmResult(bool result, int id) override;
    virtual void render(int xm, int ym, float a) override;

    class WorldSelectionList : public ScrolledSelectionList {
    public:
        SelectWorldScreen* parent;
        WorldSelectionList(SelectWorldScreen* sws);

    protected:
        virtual int getNumberOfItems();
        virtual void selectItem(int item, bool doubleClick);
        virtual bool isSelectedItem(int item);
        virtual int getMaxPosition();
        virtual void renderBackground();
        virtual void renderItem(int i, int x, int y, int h, Tesselator* t);
    };

    class ServerSelectionList : public ScrolledSelectionList {
    public:
        SelectWorldScreen* parent;
        ServerSelectionList(SelectWorldScreen* sws);

    protected:
        virtual int getNumberOfItems();
        virtual void selectItem(int item, bool doubleClick);
        virtual bool isSelectedItem(int item);
        virtual int getMaxPosition();
        virtual void renderBackground();
        virtual void renderItem(int i, int x, int y, int h, Tesselator* t);
    };
};