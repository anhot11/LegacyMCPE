#pragma once

#include <string>

class GuiMessage {
public:
    std::string string;
    int ticks;
    GuiMessage(const std::string& string);
};