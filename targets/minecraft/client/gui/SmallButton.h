#pragma once
#include <string>

#include "Button.h"
#include "minecraft/client/Options.h"

class SmallButton : public Button {
private:
    const Options::Option* option;

public:
    SmallButton(int id, int x, int y, const std::string& msg);
    SmallButton(int id, int x, int y, int width, int height,
                const std::string& msg);
    SmallButton(int id, int x, int y, const Options::Option* item,
                const std::string& msg);
    const Options::Option* getOption();
};