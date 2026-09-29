#include "UIScene_Timer.h"

#include "app/common/UI/Controls/UIControl.h"
#include "app/common/UI/UIScene.h"

class UILayer;

UIScene_Timer::UIScene_Timer(int iPad, void* initData, UILayer* parentLayer)
    : UIScene(iPad, parentLayer) {
    // Setup all the Iggy references we need for this scene
    initialiseMovie();

    // In normal usage, we want to hide the new background that's used during
    // texture pack reloading
    if (initData == 0) {
        m_controlBackground.setVisible(false);
    }
}

std::string UIScene_Timer::getMoviePath() { return "Timer"; }

void UIScene_Timer::reloadMovie(bool force) {
    // Never needs reloaded
}

bool UIScene_Timer::needsReloaded() {
    // Never needs reloaded
    return false;
}