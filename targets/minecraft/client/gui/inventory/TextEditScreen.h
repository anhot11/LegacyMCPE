#pragma once
#include <memory>
#include <string>

#include "minecraft/client/gui/Screen.h"

class SignTileEntity;

class TextEditScreen : public Screen {
protected:
    std::string title;

private:
    std::shared_ptr<SignTileEntity> sign;
    int frame;
    int line;

public:
    TextEditScreen(std::shared_ptr<SignTileEntity> sign);
    virtual void init() override;
    virtual void removed() override;
    virtual void tick() override;

protected:
    virtual void buttonClicked(Button* button) override;

private:
    static const std::string allowedChars;

protected:
    virtual void keyPressed(char ch, int eventKey) override;

public:
    virtual void render(int xm, int ym, float a) override;
};