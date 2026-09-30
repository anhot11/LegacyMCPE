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

class SelectWorldScreen : public Screen {
public:
    class WorldSelectionList;

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

protected:
    Screen* lastScreen;
    std::string title;
    Tab currentTab;

private:
    bool done;
    int selectedWorld;
    std::vector<LevelSummary*>* levelList;
    WorldSelectionList* worldSelectionList;
    std::string worldLang;
    std::string conversionLang;
    bool isDeleting;

    Button* deleteButton;
    Button* selectButton;
    Button* renameButton;
    Button* createButton;
    Button* cancelButton;
    Button* tabWorldsButton;
    Button* tabServersButton;

    EditBox* serverIpEdit;
    Button* connectServerButton;

public:
    SelectWorldScreen(Screen* lastScreen);
    virtual void init() override;
    virtual void tick() override;
    virtual void removed() override;

private:
    void loadLevelList();
    void updateTabVisibility();

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
        // 4J - had to add input parameters to ctor, original is a java subclass
        // of the screen and can access its members
        WorldSelectionList(SelectWorldScreen* sws);

    protected:
        virtual int getNumberOfItems();
        virtual void selectItem(int item, bool doubleClick);
        virtual bool isSelectedItem(int item);
        virtual int getMaxPosition();
        virtual void renderBackground();
        virtual void renderItem(int i, int x, int y, int h, Tesselator* t);
    };
};