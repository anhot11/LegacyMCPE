#pragma once

#include "Item.h"

class Player;
class Material;
class Level;
class Tile;

class DoorItem : public Item {
private:
    Material* material;

public:
    DoorItem(int id, Material* material);

    virtual bool useOn(std::shared_ptr<ItemInstance> instance,
                       std::shared_ptr<Player> player, Level* level, int x,
                       int y, int z, int face, float clickX, float clickY,
                       float clickZ, bool bTestUseOnOnly = false);
    static void place(Level* level, int x, int y, int z, int dir, Tile* tile);
};
