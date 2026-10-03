#pragma once
#include <string>
#include <vector>

#include "Screen.h"
#include "ScrolledSelectionList.h"

class EditBox;
class Button;
class Tesselator;

struct ServerEntry {
    std::string name;
    std::string ip;
    int port;
};

class JoinMultiplayerScreen : public Screen {
public:
    enum ScreenMode {
        MODE_LIST = 0,
        MODE_ADD_SERVER = 1,
        MODE_DIRECT_CONNECT = 2
    };

    class ServerSelectionList : public ScrolledSelectionList {
    public:
        JoinMultiplayerScreen* parent;
        ServerSelectionList(JoinMultiplayerScreen* parent);
        virtual int getNumberOfItems() override;
        virtual void selectItem(int item, bool doubleClick) override;
        virtual bool isSelectedItem(int item) override;
        virtual int getMaxPosition() override;
        virtual void renderBackground() override;
        virtual void renderItem(int item, int x, int y, int h, Tesselator* t) override;
    };

private:
    Screen* lastScreen;
    ScreenMode currentMode;
    std::vector<ServerEntry> serverList;
    int selectedServer;

    ServerSelectionList* selectionList;

    // List mode buttons
    Button* btnJoin;
    Button* btnDirect;
    Button* btnAdd;
    Button* btnDelete;
    Button* btnCancel;

    // Add server mode controls
    EditBox* addServerNameEdit;
    EditBox* addServerIpEdit;
    Button* btnAddSave;
    Button* btnAddCancel;

    // Direct connect mode controls
    EditBox* directIpEdit;
    Button* btnDirectJoin;
    Button* btnDirectCancel;

public:
    JoinMultiplayerScreen(Screen* lastScreen);
    virtual ~JoinMultiplayerScreen();
    virtual void tick() override;
    virtual void init() override;
    virtual void removed() override;

    void connectToServer(const std::string& ip, int port);
    void serverSelected(int index);

protected:
    virtual void buttonClicked(Button* button) override;
    virtual void keyPressed(char ch, int eventKey) override;
    virtual void mouseClicked(int x, int y, int buttonNum) override;

public:
    virtual void render(int xm, int ym, float a) override;

private:
    void initModeList();
    void initModeAddServer();
    void initModeDirectConnect();
    void loadServerList();
    void saveServerList();
    void parseIpPort(const std::string& input, std::string& outIp, int& outPort);
    void updateButtonState();
};