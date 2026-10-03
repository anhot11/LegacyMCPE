#pragma once

#include <string>
#include <vector>

#include "Screen.h"

class Options;
class Button;

struct LanguageEntry {
    std::string code;
    std::string name;
    std::string region;
    int flagU;
    int flagV;
};

class LanguageSelectScreen : public Screen {
private:
    static const int DONE_BUTTON_ID = 200;

    Screen* lastScreen;
    Options* options;
    std::string title;
    int selectedIndex;
    std::vector<LanguageEntry> languages;

    // Scrolling and touch state
    float scrollY;
    float maxScroll;
    bool isDragging;
    int dragStartY;
    float dragStartScroll;
    bool hasMoved;

    Button* btnDone;

public:
    LanguageSelectScreen(Screen* lastScreen, Options* options);
    virtual ~LanguageSelectScreen() = default;

    virtual void init() override;
    virtual void buttonClicked(Button* button) override;
    virtual void render(int xm, int ym, float a) override;

    void selectLanguage(int index);
};
