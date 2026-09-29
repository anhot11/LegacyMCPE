#include "ObjectiveCriteria.h"

#include "minecraft/world/scores/criteria/DummyCriteria.h"
#include "minecraft/world/scores/criteria/HealthCriteria.h"

std::unordered_map<std::string, ObjectiveCriteria*>
    ObjectiveCriteria::CRITERIA_BY_NAME;

ObjectiveCriteria* ObjectiveCriteria::DUMMY = new DummyCriteria("dummy");
ObjectiveCriteria* ObjectiveCriteria::DEATH_COUNT =
    new DummyCriteria("deathCount");
ObjectiveCriteria* ObjectiveCriteria::KILL_COUNT_PLAYERS =
    new DummyCriteria("playerKillCount");
ObjectiveCriteria* ObjectiveCriteria::KILL_COUNT_ALL =
    new DummyCriteria("totalKillCount");
ObjectiveCriteria* ObjectiveCriteria::HEALTH = new HealthCriteria("health");