#pragma once

#include "Item.h"

class Player;
class Level;

class EggItem : public Item {
public:
    EggItem(int id);

    virtual std::shared_ptr<ItemInstance> use(
        std::shared_ptr<ItemInstance> instance, Level* level,
        std::shared_ptr<Player> player);
};
