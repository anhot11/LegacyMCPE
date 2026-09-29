#include "HtmlString.h"

#include <iomanip>
#include <sstream>
#include <vector>

#include "minecraft/IGameServices.h"
#include "util/StringHelpers.h"

HtmlString::HtmlString(std::string text, eMinecraftColour hexColor,
                       bool italics, bool indent) {
    this->text = escapeXML(text);
    this->color = hexColor;
    this->italics = italics;
    this->indent = indent;
}

std::string HtmlString::ToString() {
    std::stringstream ss;

    if (indent) {
        ss << "&nbsp;&nbsp;";
    }

    if (italics) {
        ss << "<i>";
    }

    eMinecraftColour color =
        this->color == eMinecraftColour_NOT_SET ? eHTMLColor_7 : this->color;

    ss << "<font color=\"#" << std::setfill('0') << std::setw(6) << std::hex
       << gameServices().getHTMLColour(color) << "\">" << text << "</font>";

    if (italics) {
        ss << "</i>";
    }

    return ss.str();
}

std::string HtmlString::Compose(std::vector<HtmlString>* strings) {
    if (strings == nullptr) return "";

    std::stringstream ss;

    for (int i = 0; i < strings->size(); i++) {
        ss << strings->at(i).ToString();

        // Add a break if there's another line
        if (i + 1 < strings->size()) {
            ss << "<br>";
        }
    }

    return ss.str();
}