#pragma once
#include <string>

#include "Screen.h"

class ConfirmScreen : public Screen {
private:
    Screen* parent;
    std::string title1;
    std::string title2;
    std::string yesButton;
    std::string noButton;
    int id;

public:
    ConfirmScreen(Screen* parent, const std::string& title1,
                  const std::string& title2, int id);
    ConfirmScreen(Screen* parent, const std::string& title1,
                  const std::string& title2, const std::string& yesButton,
                  const std::string& noButton, int id);
    virtual void init() override;

protected:
    virtual void buttonClicked(Button* button) override;

public:
    virtual void render(int xm, int ym, float a) override;
};