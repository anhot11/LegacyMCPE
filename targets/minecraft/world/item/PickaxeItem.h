#pragma once

#include <format>
#include <vector>

#include "DiggerItem.h"

class Tile;

#define PICKAXE_DIGGABLES 23

class PickaxeItem : public DiggerItem {
private:
    static std::vector<Tile*> diggables;

public:  //
    static void staticCtor();

    PickaxeItem(int id, const Tier* tier);

public:
    virtual bool canDestroySpecial(Tile* tile);
    virtual float getDestroySpeed(
        std::shared_ptr<ItemInstance> itemInstance,
        Tile* tile);  // 4J - brought forward from 1.2.3
};
