#include "HealthCriteria.h"

#include <memory>
#include <vector>

#include "minecraft/util/Mth.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/scores/criteria/DummyCriteria.h"

HealthCriteria::HealthCriteria(const std::string& id) : DummyCriteria(id) {}

int HealthCriteria::getScoreModifier(
    std::vector<std::shared_ptr<Player> >* players) {
    float health = 0;

    for (auto it = players->begin(); it != players->end(); ++it) {
        std::shared_ptr<Player> player = *it;
        health += player->getHealth() + player->getAbsorptionAmount();
    }

    if (players->size() > 0) health /= players->size();

    return Mth::ceil(health);
}

bool HealthCriteria::isReadOnly() { return true; }