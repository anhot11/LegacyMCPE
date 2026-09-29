#pragma once

#include <memory>

#include "Item.h"
#include "minecraft/world/item/UseAnim.h"

class Player;
class Level;
class ItemInstance;

class FoodItem : public Item {
public:
    static const int EAT_DURATION = (int)(20 * 1.6);

private:
    const int nutrition;
    const float saturationModifier;
    const bool m_isMeat;
    bool canAlwaysEat;

    int effectId;
    int effectDurationSeconds;
    int effectAmplifier;
    float effectProbability;

    void _init();

public:
    FoodItem(int id, int nutrition, float saturationMod, bool isMeat);
    FoodItem(int id, int nutrition, bool isMeat);

    virtual std::shared_ptr<ItemInstance> useTimeDepleted(
        std::shared_ptr<ItemInstance> instance, Level* level,
        std::shared_ptr<Player> player);

protected:
    virtual void addEatEffect(std::shared_ptr<ItemInstance> instance,
                              Level* level, std::shared_ptr<Player> player);

public:
    virtual int getUseDuration(std::shared_ptr<ItemInstance> itemInstance);
    virtual UseAnim getUseAnimation(std::shared_ptr<ItemInstance> itemInstance);

    virtual std::shared_ptr<ItemInstance> use(
        std::shared_ptr<ItemInstance> instance, Level* level,
        std::shared_ptr<Player> player);

    int getNutrition();
    float getSaturationModifier();
    bool isMeat();

    FoodItem* setEatEffect(int id, int durationInSecods, int amplifier,
                           float effectProbability);
    FoodItem* setCanAlwaysEat();

    // 4J Added
    bool canEat(std::shared_ptr<Player> player);
};