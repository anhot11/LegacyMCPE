#pragma once

#include <string>

#include "app/common/UI/All Platforms/UIEnums.h"
#include "app/common/UI/UIScene.h"

class UILayer;

class UIScene_TrialExitUpsell : public UIScene {
private:
    UI_BEGIN_MAP_ELEMENTS_AND_NAMES(UIScene)
    UI_END_MAP_ELEMENTS_AND_NAMES()

public:
    UIScene_TrialExitUpsell(int iPad, void* initData, UILayer* parentLayer);

    virtual EUIScene getSceneType() { return eUIScene_TrialExitUpsell; }

    // Returns true if this scene has focus for the pad passed in
    virtual bool hasFocus(int iPad) { return bHasFocus; }
    virtual void updateTooltips();

protected:
    virtual std::string getMoviePath();

public:
    // INPUT
    virtual void handleInput(int iPad, int key, bool repeat, bool pressed,
                             bool released, bool& handled);

    virtual void handleAnimationEnd();
};