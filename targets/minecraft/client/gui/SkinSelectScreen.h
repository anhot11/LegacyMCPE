#pragma once

#include <memory>
#include <string>
#include <vector>

#include "minecraft/client/gui/Screen.h"

class Button;
class HumanoidModel;

struct SkinEntry {
    std::string id;
    std::string displayName;
    std::string subtitle;
    int textureEnum;       // TEXTURE_NAME enum if built-in
    std::string texturePath; // resource path if file
};

class SkinSelectScreen : public Screen {
private:
    Screen* lastScreen;
    HumanoidModel* previewModel;
    std::vector<SkinEntry> skins;
    int selectedIndex;

    Button* btnDone;
    Button* btnCustomSkin;
    Button* btnScrollUp;
    Button* btnScrollDown;

    void scanStorageSkins();

    float scrollY;
    float maxScroll;
    bool isDragging;
    bool isDraggingScrollbar;
    int dragStartY;
    float dragStartScroll;
    bool hasMoved;

    // 3D Preview interaction
    float previewYaw;
    float previewPitch;
    bool isDraggingPreview;
    int previewDragStartX;
    int previewDragStartY;
    float previewDragStartYaw;
    float previewDragStartPitch;
    float vo;

public:
    SkinSelectScreen(Screen* lastScreen);
    virtual ~SkinSelectScreen();

    virtual void init() override;
    virtual void tick() override;
    virtual void render(int xm, int ym, float a) override;
    virtual void buttonClicked(Button* button) override;
    virtual void mouseClicked(int xm, int ym, int buttonNum) override;
    virtual void mouseReleased(int xm, int ym, int buttonNum) override;

    void selectSkin(int index);
    int bindSkinTexture(const SkinEntry& entry);
    static int getActiveSkinTexture(Minecraft* mc);
};
