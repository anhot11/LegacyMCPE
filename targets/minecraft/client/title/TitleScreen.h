#pragma once
#include <string>

#include "minecraft/client/gui/Screen.h"

class Random;
class Button;

class TitleScreen : public Screen {
private:
    static Random* random;

    float vo;

    std::string splash;
    Button* multiplayerButton;

    // 4jcraft: panorama
    void renderPanorama(float a);
    void renderSkybox(float a);
    void rotateAndBlur(float a);
    int viewportTexture;

    // 4jcraft: taken from UIScene_MainMenu
    // 4J Added
    enum eSplashIndexes {
        eSplashHappyBirthdayEx = 0,
        eSplashHappyBirthdayNotch,
        eSplashMerryXmas,
        eSplashHappyNewYear,

        // The start index in the splashes vector from which we can select a
        // random splash
        eSplashRandomStart,
    };

public:
    TitleScreen();
    virtual void tick() override;

protected:
    virtual void keyPressed(char eventCharacter, int eventKey) override;

public:
    virtual void init() override;

protected:
    virtual void buttonClicked(Button* button) override;

public:
    virtual void render(int xm, int ym, float a) override;
};