#pragma once

#include <stdarg.h>

#include <string>
#include <unordered_map>

class Language {
private:
    static Language* singleton;
    std::unordered_map<std::string, std::string> translateTable;

public:
    Language();
    static Language* getInstance();
    std::string getElement(std::string elementId, ...);
    std::string getElement(const std::string& elementId, va_list args);
    std::string getElementName(const std::string& elementId);
    std::string getElementDescription(const std::string& elementId);
};