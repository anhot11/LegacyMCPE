#pragma once

#include <deque>
#include <string>

#include "app/common/UI/All Platforms/UIEnums.h"
#include "app/common/UI/Controls/UIControl_Label.h"
#include "app/common/UI/UIScene.h"

class UILayer;

class UIComponent_DebugUIConsole : public UIScene {
private:
    UIControl_Label m_labels[10];
    UI_BEGIN_MAP_ELEMENTS_AND_NAMES(UIScene)
    UI_MAP_ELEMENT(m_labels[0], "consoleLine1")
    UI_MAP_ELEMENT(m_labels[1], "consoleLine2")
    UI_MAP_ELEMENT(m_labels[2], "consoleLine3")
    UI_MAP_ELEMENT(m_labels[3], "consoleLine4")
    UI_MAP_ELEMENT(m_labels[4], "consoleLine5")
    UI_MAP_ELEMENT(m_labels[5], "consoleLine6")
    UI_MAP_ELEMENT(m_labels[6], "consoleLine7")
    UI_MAP_ELEMENT(m_labels[7], "consoleLine8")
    UI_MAP_ELEMENT(m_labels[8], "consoleLine9")
    UI_MAP_ELEMENT(m_labels[9], "consoleLine10")
    UI_END_MAP_ELEMENTS_AND_NAMES()

    std::deque<std::string> m_textList;

    bool m_bTextChanged;

public:
    UIComponent_DebugUIConsole(int iPad, void* initData, UILayer* parentLayer);

    virtual void tick();

protected:
    // TODO: This should be pure virtual in this class
    virtual std::string getMoviePath();

public:
    virtual EUIScene getSceneType() { return eUIComponent_DebugUIConsole; }

    // Returns true if this scene handles input
    virtual bool stealsFocus() { return false; }

    // Returns true if this scene has focus for the pad passed in
    virtual bool hasFocus(int iPad) { return false; }

    // Returns true if lower scenes in this scenes layer, or in any layer below
    // this scenes layers should be hidden
    virtual bool hidesLowerScenes() { return false; }

    void addText(const std::string& text);
};