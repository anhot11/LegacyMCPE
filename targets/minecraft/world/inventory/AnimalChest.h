#pragma once

#include <string>

#include "minecraft/world/SimpleContainer.h"

class AnimalChest : public SimpleContainer {
public:
    AnimalChest(const std::string& name, int size);
    AnimalChest(int iTitle, const std::string& name, bool hasCustomName,
                int size);  // 4J Added iTitle param
};