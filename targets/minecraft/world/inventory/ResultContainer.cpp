#include "ResultContainer.h"

#include "minecraft/world/Container.h"

ResultContainer::ResultContainer() : Container() {}

unsigned int ResultContainer::getContainerSize() { return 1; }

std::shared_ptr<ItemInstance> ResultContainer::getItem(unsigned int slot) {
    return items[0];
}

std::string ResultContainer::getName() { return ""; }

std::string ResultContainer::getCustomName() { return ""; }

bool ResultContainer::hasCustomName() { return false; }

std::shared_ptr<ItemInstance> ResultContainer::removeItem(unsigned int slot,
                                                          int count) {
    if (items[0] != nullptr) {
        std::shared_ptr<ItemInstance> item = items[0];
        items[0] = nullptr;
        return item;
    }
    return nullptr;
}

std::shared_ptr<ItemInstance> ResultContainer::removeItemNoUpdate(int slot) {
    if (items[0] != nullptr) {
        std::shared_ptr<ItemInstance> item = items[0];
        items[0] = nullptr;
        return item;
    }
    return nullptr;
}

void ResultContainer::setItem(unsigned int slot,
                              std::shared_ptr<ItemInstance> item) {
    items[0] = item;
}

int ResultContainer::getMaxStackSize() {
    return Container::LARGE_MAX_STACK_SIZE;
}

void ResultContainer::setChanged() {}

bool ResultContainer::stillValid(std::shared_ptr<Player> player) {
    return true;
}

bool ResultContainer::canPlaceItem(int slot,
                                   std::shared_ptr<ItemInstance> item) {
    return true;
}