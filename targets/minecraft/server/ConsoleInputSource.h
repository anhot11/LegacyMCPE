#pragma once

#include <string>

class ConsoleInputSource {
public:
    virtual ~ConsoleInputSource() {}
    virtual void info(const std::string& string) = 0;
    virtual void warn(const std::string& string) = 0;
    virtual std::string getConsoleName() = 0;
};
