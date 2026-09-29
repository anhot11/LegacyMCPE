#pragma once

#include <memory>

#include "Item.h"

class Level;
class Player;

class LeashItem : public Item {
public:
    LeashItem(int id);

    bool useOn(std::shared_ptr<ItemInstance> itemInstance,
               std::shared_ptr<Player> player, Level* level, int x, int y,
               int z, int face, float clickX, float clickY, float clickZ,
               bool bTestUseOnOnly = false);
    static bool bindPlayerMobs(std::shared_ptr<Player> player, Level* level,
                               int x, int y, int z);
    static bool bindPlayerMobsTest(std::shared_ptr<Player> player, Level* level,
                                   int x, int y, int z);
};