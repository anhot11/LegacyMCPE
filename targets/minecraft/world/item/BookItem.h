#pragma once

#include "Item.h"

class BookItem : public Item {
public:
    BookItem(int id);

    bool isEnchantable(std::shared_ptr<ItemInstance> itemInstance);
    int getEnchantmentValue();
};