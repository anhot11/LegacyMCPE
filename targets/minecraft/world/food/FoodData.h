#pragma once

#include <memory>

class FoodItem;
class Player;
class CompoundTag;

class FoodData {
private:
    int foodLevel;
    float saturationLevel;
    float exhaustionLevel;

    int tickTimer;
    int lastFoodLevel;

public:
    FoodData();

    void eat(int food, float saturationModifier);
    void eat(FoodItem* item);
    void tick(std::shared_ptr<Player> player);
    void readAdditionalSaveData(CompoundTag* entityTag);
    void addAdditonalSaveData(CompoundTag* entityTag);
    int getFoodLevel();
    int getLastFoodLevel();
    bool needsFood();
    void addExhaustion(float amount);
    float getExhaustionLevel();
    float getSaturationLevel();
    void setFoodLevel(int food);
    void setSaturation(float saturation);
    void setExhaustion(float exhaustion);
};