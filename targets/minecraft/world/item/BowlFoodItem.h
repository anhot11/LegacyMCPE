#pragma once

#include "FoodItem.h"

class Player;
class Level;

class BowlFoodItem : public FoodItem {
public:
    BowlFoodItem(int id, int nutrition);

    std::shared_ptr<ItemInstance> useTimeDepleted(
        std::shared_ptr<ItemInstance> instance, Level* level,
        std::shared_ptr<Player> player);
};