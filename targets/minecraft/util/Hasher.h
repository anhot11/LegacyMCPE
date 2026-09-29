#pragma once

#include <string>

class Hasher {
private:
    std::string salt;

public:
    Hasher(std::string& salt);
    std::string getHash(std::string& name);
};