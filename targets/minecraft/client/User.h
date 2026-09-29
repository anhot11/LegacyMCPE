#pragma once

#include <format>
#include <string>
#include <vector>

class Tile;

class User {
public:
    static std::vector<Tile*> allowedTiles;
    static void staticCtor();
    std::string name;
    std::string sessionId;
    std::string mpPassword;

    User(const std::string& name, const std::string& sessionId);
};