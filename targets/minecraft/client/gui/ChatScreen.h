#pragma once
#include <string>

#include "Screen.h"

class ChatScreen : public Screen {
protected:
    std::string message;

private:
    int frame;

    Button* btnSend;
    Button* btnClose;

public:
    ChatScreen();  // 4J added
    virtual void init() override;
    virtual void removed() override;
    virtual void tick() override;

private:
    static const std::string allowedChars;
    void sendMessage();

protected:
    void keyPressed(char ch, int eventKey) override;
    void buttonClicked(Button* button) override;

public:
    void render(int xm, int ym, float a) override;

protected:
    void mouseClicked(int x, int y, int buttonNum) override;
};