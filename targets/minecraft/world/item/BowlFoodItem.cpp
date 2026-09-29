#include "BowlFoodItem.h"

#include <memory>

#include "minecraft/world/item/FoodItem.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"

BowlFoodItem::BowlFoodItem(int id, int nutrition)
    : FoodItem(id, nutrition, false) {
    setMaxStackSize(1);
}

std::shared_ptr<ItemInstance> BowlFoodItem::useTimeDepleted(
    std::shared_ptr<ItemInstance> instance, Level* level,
    std::shared_ptr<Player> player) {
    FoodItem::useTimeDepleted(instance, level, player);

    return std::make_shared<ItemInstance>(Item::bowl);
}