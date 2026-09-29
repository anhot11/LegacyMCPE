#include "AnimalChest.h"

#include "minecraft/world/SimpleContainer.h"
#include "strings.h"

AnimalChest::AnimalChest(const std::string& name, int size)
    : SimpleContainer(IDS_CONTAINER_ANIMAL, name, false, size) {}

AnimalChest::AnimalChest(int iTitle, const std::string& name,
                         bool hasCustomName, int size)
    : SimpleContainer(iTitle, name, hasCustomName, size) {}