#include "BookItem.h"

#include <memory>

#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"

BookItem::BookItem(int id) : Item(id) {}

bool BookItem::isEnchantable(std::shared_ptr<ItemInstance> itemInstance) {
    return itemInstance->count == 1;
}

int BookItem::getEnchantmentValue() { return 1; }