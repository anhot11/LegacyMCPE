#pragma once
#include <format>
#include <vector>

#include "DiggerItem.h"

class Tile;

#define SHOVEL_DIGGABLES 10
class ShovelItem : public DiggerItem {
private:
    static std::vector<Tile*>* diggables;

public:
    static void staticCtor();
    ShovelItem(int id, const Tier* tier);

    bool canDestroySpecial(Tile* tile);
    virtual bool useOn(std::shared_ptr<ItemInstance> instance,
                       std::shared_ptr<Player> player, Level* level, int x,
                       int y, int z, int face, float clickX, float clickY,
                       float clickZ, bool bTestUseOnOnly = false) override;
};