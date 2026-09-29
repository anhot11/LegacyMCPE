#pragma once

#include <stdarg.h>

#include <string>

#include "Language.h"

class Language;

class I18n {
private:
    static Language* lang;

public:
    static std::string get(std::string id, ...);
    static std::string get(const std::string& id, va_list args);
};
