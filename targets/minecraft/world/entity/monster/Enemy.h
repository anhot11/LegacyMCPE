#pragma once
#include "minecraft/world/entity/Creature.h"
#include "minecraft/world/entity/EntitySelector.h"

class Enemy : public Creature {
public:
    class EnemyEntitySelector : public EntitySelector {
        bool matches(std::shared_ptr<Entity> entity) const;
    };

    static const int XP_REWARD_NONE = 0;
    static const int XP_REWARD_SMALL = 3;
    static const int XP_REWARD_MEDIUM = 5;
    static const int XP_REWARD_LARGE = 10;
    static const int XP_REWARD_HUGE = 20;
    static const int XP_REWARD_BOSS = 50;

    static EntitySelector* ENEMY_SELECTOR;
};
