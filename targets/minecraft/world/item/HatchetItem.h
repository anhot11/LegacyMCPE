#pragma once

#include <format>
#include <vector>

#include "DiggerItem.h"

class Tile;

#define HATCHET_DIGGABLES 8
class HatchetItem : public DiggerItem {
private:
    static std::vector<Tile*>* diggables;

public:
    static void staticCtor();
    HatchetItem(int id, const Tier* tier);
    virtual float getDestroySpeed(
        std::shared_ptr<ItemInstance> itemInstance,
        Tile* tile);  // 4J - brought forward from 1.2.3
};
