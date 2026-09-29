#include "LookAtTradingPlayerGoal.h"

#include <memory>

#include "minecraft/world/entity/ai/goal/LookAtPlayerGoal.h"
#include "minecraft/world/entity/npc/Villager.h"
#include "minecraft/world/entity/player/Player.h"

class Entity;
class Mob;

LookAtTradingPlayerGoal::LookAtTradingPlayerGoal(Villager* villager)
    : LookAtPlayerGoal((Mob*)villager, typeid(Player), 8) {
    this->villager = villager;
}

bool LookAtTradingPlayerGoal::canUse() {
    if (villager->isTrading()) {
        lookAt = std::weak_ptr<Entity>(
            std::dynamic_pointer_cast<Entity>(villager->getTradingPlayer()));
        return true;
    }
    return false;
}