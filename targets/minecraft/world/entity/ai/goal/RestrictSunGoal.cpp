#include "RestrictSunGoal.h"

#include "minecraft/world/entity/PathfinderMob.h"
#include "minecraft/world/entity/ai/navigation/PathNavigation.h"
#include "minecraft/world/level/Level.h"

RestrictSunGoal::RestrictSunGoal(PathfinderMob* mob) { this->mob = mob; }

bool RestrictSunGoal::canUse() { return mob->level->isDay(); }

void RestrictSunGoal::start() { mob->getNavigation()->setAvoidSun(true); }

void RestrictSunGoal::stop() { mob->getNavigation()->setAvoidSun(false); }