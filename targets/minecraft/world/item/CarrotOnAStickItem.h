#pragma once

#include "Item.h"

class CarrotOnAStickItem : public Item {
public:
    CarrotOnAStickItem(int id);

    bool isHandEquipped();
    bool isMirroredArt();
    std::shared_ptr<ItemInstance> use(
        std::shared_ptr<ItemInstance> itemInstance, Level* level,
        std::shared_ptr<Player> player);
};