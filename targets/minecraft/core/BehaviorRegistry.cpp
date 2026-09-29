#include "BehaviorRegistry.h"

#include <utility>

#include "minecraft/core/DispenseItemBehavior.h"

class Item;

BehaviorRegistry::BehaviorRegistry(DispenseItemBehavior* defaultValue) {
    defaultBehavior = defaultValue;
}

BehaviorRegistry::~BehaviorRegistry() {
    for (auto it = storage.begin(); it != storage.end(); ++it) {
        delete it->second;
    }

    delete defaultBehavior;
}

DispenseItemBehavior* BehaviorRegistry::get(Item* key) {
    auto it = storage.find(key);

    return (it == storage.end()) ? defaultBehavior : it->second;
}

void BehaviorRegistry::add(Item* key, DispenseItemBehavior* value) {
    storage.insert(std::make_pair(key, value));
}