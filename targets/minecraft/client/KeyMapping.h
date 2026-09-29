#pragma once

#include <string>

// 4J Stu - Not updated to 1.8.2 as we don't use this
class KeyMapping {
public:
    std::string name;
    int key;
    KeyMapping(const std::string& name, int key);
};