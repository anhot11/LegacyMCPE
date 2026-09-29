#pragma once

#include "FoodItem.h"

class SeedFoodItem : public FoodItem {
private:
    int resultId;
    int targetLand;

public:
    SeedFoodItem(int id, int nutrition, float saturationMod, int resultId,
                 int targetLand);

    bool useOn(std::shared_ptr<ItemInstance> instance,
               std::shared_ptr<Player> player, Level* level, int x, int y,
               int z, int face, float clickX, float clickY, float clickZ,
               bool bTestUseOnOnly);
};