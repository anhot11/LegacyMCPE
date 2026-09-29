#include "SmallButton.h"

#include "minecraft/client/Options.h"
#include "minecraft/client/gui/Button.h"

SmallButton::SmallButton(int id, int x, int y, const std::string& msg)
    : Button(id, x, y, 150, 20, msg) {
    this->option = nullptr;
}

SmallButton::SmallButton(int id, int x, int y, int width, int height,
                         const std::string& msg)
    : Button(id, x, y, width, height, msg) {
    this->option = nullptr;
}

SmallButton::SmallButton(int id, int x, int y, const Options::Option* item,
                         const std::string& msg)
    : Button(id, x, y, 150, 20, msg) {
    this->option = item;
}

const Options::Option* SmallButton::getOption() { return option; }