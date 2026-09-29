#include "GuiMessage.h"

GuiMessage::GuiMessage(const std::string& string) {
    this->string = string;
    ticks = 0;
}