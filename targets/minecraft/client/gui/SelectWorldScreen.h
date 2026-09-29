#pragma once
#include <format>
#include <string>
#include <vector>

#include "Screen.h"
#include "ScrolledSelectionList.h"

class LevelSummary;
class Button;
class Tesselator;

class SelectWorldScreen : public Screen {
public:
    class WorldSelectionList;

protected:
    static const int BUTTON_CANCEL_ID = 0;
    static const int BUTTON_SELECT_ID = 1;
    static const int BUTTON_DELETE_ID = 2;
    static const int BUTTON_CREATE_ID = 3;
    static const int BUTTON_UP_ID = 4;
    static const int BUTTON_DOWN_ID = 5;
    static const int BUTTON_RENAME_ID = 6;

private:
    //	final DateFormat DATE_FORMAT = new SimpleDateFormat();		// 4J -
    // removed

protected:
    Screen* lastScreen;
    std::string title;

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

public:
    SelectWorldScreen(Screen* lastScreen);
    virtual void init() override;

private:
    void loadLevelList();

protected:
    std::string getWorldId(int id);
    std::string getWorldName(int id);

public:
    virtual void postInit();

protected:
    virtual void buttonClicked(Button* button) override;

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