#pragma once

#include <memory>

#include "Enchantment.h"

class Random;
class Entity;
class LivingEntity;

class ThornsEnchantment : public Enchantment {
private:
    static const float CHANCE_PER_LEVEl;

public:
    ThornsEnchantment(int id, int freq);

    virtual int getMinCost(int level);
    virtual int getMaxCost(int level);
    virtual int getMaxLevel();
    virtual bool canEnchant(std::shared_ptr<ItemInstance> item);
    static bool shouldHit(int level, Random* random);
    static int getDamage(int level, Random* random);
    static void doThornsAfterAttack(std::shared_ptr<Entity> source,
                                    std::shared_ptr<LivingEntity> target,
                                    Random* random);
};