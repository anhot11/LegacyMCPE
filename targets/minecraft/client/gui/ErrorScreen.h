#pragma once
#include <string>

#include "Screen.h"

class ErrorScreen : public Screen {
private:
    std::string title, message;

public:
    ErrorScreen(const std::string& title, const std::string& message);
    virtual void init() override;
    virtual void render(int xm, int ym, float a) override;

protected:
    virtual void keyPressed(char eventCharacter, int eventKey) override;
};