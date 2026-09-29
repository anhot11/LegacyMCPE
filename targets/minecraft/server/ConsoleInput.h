#pragma once
#include <string>

#include "ConsoleInputSource.h"

class ConsoleInputSource;

class ConsoleInput {
public:
    std::string msg;
    ConsoleInputSource* source;

    ConsoleInput(const std::string& msg, ConsoleInputSource* source);
};