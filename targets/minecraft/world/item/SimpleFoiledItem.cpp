#include "SimpleFoiledItem.h"

#include <memory>

#include "minecraft/world/item/Item.h"

SimpleFoiledItem::SimpleFoiledItem(int id) : Item(id) {}

bool SimpleFoiledItem::isFoil(std::shared_ptr<ItemInstance> itemInstance) {
    return true;
}
