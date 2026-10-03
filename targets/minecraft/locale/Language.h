#pragma once

#include <stdarg.h>

#include <string>
#include <unordered_map>

class Language {
private:
    static Language* singleton;
    std::unordered_map<std::string, std::string> translateTable;
    std::string currentLanguage;

    void parseLangFile(const std::string& path);

public:
    Language();
    static Language* getInstance();
    void loadLanguage(const std::string& langCode);
    std::string getCurrentLanguage() const { return currentLanguage; }
    std::string getElement(std::string elementId, ...);
    std::string getElement(const std::string& elementId, va_list args);
    std::string getElementName(const std::string& elementId);
    std::string getElementDescription(const std::string& elementId);
};