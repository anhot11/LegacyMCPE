#pragma once

#include "ChatScreen.h"

class InBedChatScreen : public ChatScreen {
private:
    static const int WAKE_UP_BUTTON = 1;

public:
    virtual void init() override;
    virtual void removed() override;

protected:
    virtual void keyPressed(char ch, int eventKey) override;

public:
    virtual void render(int xm, int ym, float a) override;

protected:
    virtual void buttonClicked(Button* button) override;

private:
    void sendWakeUp();
};