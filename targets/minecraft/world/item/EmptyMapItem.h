#pragma once

#include "ComplexItem.h"

class EmptyMapItem : public ComplexItem {
public:
    EmptyMapItem(int id);

    std::shared_ptr<ItemInstance> use(
        std::shared_ptr<ItemInstance> itemInstance, Level* level,
        std::shared_ptr<Player> player);
};