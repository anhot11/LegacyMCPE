#pragma once

#include <string>

#include "app/common/UI/All Platforms/UIEnums.h"
#include "app/common/UI/UIScene.h"

class UILayer;

class UIComponent_Logo : public UIScene {
public:
    UIComponent_Logo(int iPad, void* initData, UILayer* parentLayer);

protected:
    // TODO: This should be pure virtual in this class
    virtual std::string getMoviePath();

public:
    virtual EUIScene getSceneType() { return eUIComponent_Logo; }

    // Returns true if this scene handles input
    virtual bool stealsFocus() { return false; }

    // Returns true if this scene has focus for the pad passed in
    virtual bool hasFocus(int iPad) { return false; }

    // Returns true if lower scenes in this scenes layer, or in any layer below
    // this scenes layers should be hidden
    virtual bool hidesLowerScenes() { return false; }
};