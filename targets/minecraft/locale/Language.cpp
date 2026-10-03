#include "Language.h"

#include <stdint.h>
#include <wchar.h>

#include <sstream>
#include <utility>
#include <vector>

#include "java/File.h"
#include "java/InputOutputStream/FileInputStream.h"
#include "java/InputOutputStream/InputStream.h"

// 4J - TODO - properly implement
// 4jcraft: done!

Language* Language::singleton = nullptr;

void Language::parseLangFile(const std::string& path) {
    File langFile(path);
    if (!langFile.exists()) return;
    InputStream* stream = new FileInputStream(langFile);
    if (!stream) return;
    int64_t fileSize = langFile.length();
    if (fileSize > 0) {
        std::vector<uint8_t> buffer((unsigned int)fileSize);
        int bytesRead = stream->read(buffer, 0, (unsigned int)fileSize);
        if (bytesRead > 0) {
            std::string content(reinterpret_cast<char*>(buffer.data()), bytesRead);
            std::istringstream iss(content);
            std::string line;
            while (std::getline(iss, line)) {
                size_t start = line.find_first_not_of(" \t\r\n");
                if (start == std::string::npos) continue;
                size_t end = line.find_last_not_of(" \t\r\n");
                std::string trimmed = line.substr(start, end - start + 1);
                if (trimmed.empty() || trimmed[0] == '#') continue;
                size_t equalsPos = trimmed.find('=');
                if (equalsPos != std::string::npos) {
                    std::string key = trimmed.substr(0, equalsPos);
                    std::string value = trimmed.substr(equalsPos + 1);

                    size_t tabHash = value.find("\t#");
                    if (tabHash != std::string::npos) {
                        value = value.substr(0, tabHash);
                    }

                    size_t kstart = key.find_first_not_of(" \t\r\n");
                    size_t kend = key.find_last_not_of(" \t\r\n");
                    if (kstart != std::string::npos) key = key.substr(kstart, kend - kstart + 1);

                    size_t vstart = value.find_first_not_of(" \t\r\n");
                    size_t vend = value.find_last_not_of(" \t\r\n");
                    if (vstart != std::string::npos) value = value.substr(vstart, vend - vstart + 1);
                    else value = "";

                    translateTable[key] = value;
                }
            }
        }
    }
    delete stream;
}

void Language::loadLanguage(const std::string& langCode) {
    translateTable.clear();
    parseLangFile("Common/res/lang/en_US.lang");
    if (!langCode.empty() && langCode != "en_US") {
        parseLangFile("Common/res/lang/" + langCode + ".lang");
    }
    currentLanguage = langCode.empty() ? "en_US" : langCode;
}

Language::Language() {
    loadLanguage("en_US");
}

Language* Language::getInstance() {
    // 4jcraft, fixes static init fiassco in I18n.cpp
    if (singleton == nullptr) {
        singleton = new Language();
    }

    return singleton;
}

/* 4J Jev, creates 2 identical functions.
std::string Language::getElement(const std::string& elementId)
{
        return elementId;
} */

// 4jcraft changed, again const reference into va_start, std forbids
std::string Language::getElement(std::string elementId, ...) {
    va_list args;
    va_start(args, elementId);
    std::string result = getElement(elementId, args);
    va_end(args);
    return result;
}

std::string Language::getElement(const std::string& elementId, va_list args) {
    auto it = translateTable.find(elementId);
    std::string formatString =
        (it != translateTable.end()) ? it->second : elementId;

    if (formatString.find('%') != std::string::npos) {
        int bufferSize = formatString.length() + 256;
        std::vector<char> buffer(bufferSize);

        int written =
            vsnprintf(buffer.data(), bufferSize, formatString.c_str(), args);
        if (written >= 0) {
            return std::string(buffer.data(), written);
        }
    }

    return formatString;
}

std::string Language::getElementName(const std::string& elementId) {
    std::string nameKey = elementId + ".name";
    auto it = translateTable.find(nameKey);
    return (it != translateTable.end()) ? it->second : elementId;
}

std::string Language::getElementDescription(const std::string& elementId) {
    std::string descKey = elementId + ".description";
    auto it = translateTable.find(descKey);
    return (it != translateTable.end()) ? it->second : elementId;
}
