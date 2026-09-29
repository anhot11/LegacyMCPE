#pragma once

#include "FoodItem.h"
#include "minecraft/world/item/Item.h"

class GoldenAppleItem : public FoodItem {
public:
    using Item::getUseDescriptionId;

    GoldenAppleItem(int id, int nutrition, float saturationMod, bool isMeat);

    virtual bool isFoil(std::shared_ptr<ItemInstance> itemInstance);
    virtual const Rarity* getRarity(std::shared_ptr<ItemInstance> itemInstance);

    // 4J-JEV: Enchanted goldenapples and goldenapples each require their own
    // tooltips.
    virtual unsigned int getUseDescriptionId(int iData /*= -1*/);
    virtual unsigned int getUseDescriptionId(
        std::shared_ptr<ItemInstance> instance);

protected:
    void addEatEffect(std::shared_ptr<ItemInstance> instance, Level* level,
                      std::shared_ptr<Player> player);
};
