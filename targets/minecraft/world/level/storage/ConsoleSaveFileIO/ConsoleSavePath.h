#pragma once

#include <string>

class ConsoleSavePath {
private:
    std::string path;

public:
    ConsoleSavePath(const std::string& newPath) { path = newPath; }

    std::string getName() const { return path; }

    std::string operator+(std::string& b) { return path + b; }
};