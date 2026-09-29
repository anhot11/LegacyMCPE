#pragma once

#include <unordered_map>

#include "DispenseItemBehavior.h"

class DispenseItemBehavior;
class Item;

class BehaviorRegistry {
private:
    std::unordered_map<Item*, DispenseItemBehavior*> storage;
    DispenseItemBehavior* defaultBehavior;

public:
    BehaviorRegistry(DispenseItemBehavior* defaultValue);
    ~BehaviorRegistry();

    DispenseItemBehavior* get(Item* key);
    void add(Item* key, DispenseItemBehavior* value);
};