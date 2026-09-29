#include "VillageSiege.h"

#include <math.h>

#include <numbers>
#include <optional>
#include <vector>

#include "java/Random.h"
#include "minecraft/Pos.h"
#include "minecraft/world/entity/MobCategory.h"
#include "minecraft/world/entity/ai/village/Village.h"
#include "minecraft/world/entity/ai/village/Villages.h"
#include "minecraft/world/entity/monster/Zombie.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/MobSpawner.h"
#include "minecraft/world/phys/Vec3.h"

VillageSiege::VillageSiege(Level* level) {
    hasSetupSiege = false;
    siegeState = SIEGE_NOT_INITED;
    siegeCount = 0;
    nextSpawnTime = 0;
    village = std::weak_ptr<Village>();
    spawnX = spawnY = spawnZ = 0;

    this->level = level;
}

void VillageSiege::tick() {
    bool debug = false;
    if (debug) {
        if (siegeState == SIEGE_DONE) {
            siegeCount = 100;
            return;
        }
        // return;
    } else {
        if (level->isDay()) {
            siegeState = SIEGE_CAN_ACTIVATE;
            return;
        }

        if (siegeState == SIEGE_DONE) return;

        if (siegeState == SIEGE_CAN_ACTIVATE) {
            float timeOfDay = level->getTimeOfDay(0);
            if (timeOfDay < 0.50 || timeOfDay > 0.501) return;
            siegeState =
                level->random->nextInt(10) == 0 ? SIEGE_TONIGHT : SIEGE_DONE;
            hasSetupSiege = false;
            if (siegeState == SIEGE_DONE) return;
        }
    }

    if (!hasSetupSiege) {
        if (tryToSetupSiege())
            hasSetupSiege = true;
        else
            return;
    }

    // Siege!
    if (nextSpawnTime > 0) {
        --nextSpawnTime;
        return;
    }

    nextSpawnTime = 2;  // 50 + level.random.nextInt(100);
    if (siegeCount > 0) {
        trySpawn();
        --siegeCount;
    } else {
        siegeState = SIEGE_DONE;
    }
}

bool VillageSiege::tryToSetupSiege() {
    std::vector<std::shared_ptr<Player> >* players = &level->players;
    // for (Player player : players)
    for (auto it = players->begin(); it != players->end(); ++it) {
        std::shared_ptr<Player> player = *it;
        std::shared_ptr<Village> _village = level->villages->getClosestVillage(
            (int)player->x, (int)player->y, (int)player->z, 1);
        village = _village;

        if (_village == nullptr) continue;
        if (_village->getDoorCount() < 10) continue;
        if (_village->getStableAge() < 20) continue;
        if (_village->getPopulationSize() < 20) continue;

        // setup siege origin
        Pos* center = _village->getCenter();
        float radius = _village->getRadius();

        bool overlaps = false;
        for (int i = 0; i < 10; ++i) {
            spawnX = center->x + (int)(cosf(level->random->nextFloat() *
                                            std::numbers::pi * 2.f) *
                                       radius * 0.9);
            spawnY = center->y;
            spawnZ = center->z + (int)(sinf(level->random->nextFloat() *
                                            std::numbers::pi * 2.f) *
                                       radius * 0.9);
            overlaps = false;
            std::vector<std::shared_ptr<Village> >* villages =
                level->villages->getVillages();
            // for (Village v : level.villages.getVillages())
            for (auto itV = villages->begin(); itV != villages->end(); ++itV) {
                std::shared_ptr<Village> v = *itV;
                if (v == _village) continue;
                if (v->isInside(spawnX, spawnY, spawnZ)) {
                    overlaps = true;
                    break;
                }
            }
            if (!overlaps) break;
        }
        if (overlaps) return false;

        auto spawnPos = findRandomSpawnPos(spawnX, spawnY, spawnZ);
        if (!spawnPos.has_value()) continue;

        nextSpawnTime = 0;
        siegeCount = 20;
        return true;
    }
    return false;
}

bool VillageSiege::trySpawn() {
    auto spawnPos = findRandomSpawnPos(spawnX, spawnY, spawnZ);
    if (!spawnPos.has_value()) return false;
    std::shared_ptr<Zombie> mob;
    // try
    {
        mob = std::make_shared<Zombie>(level);
        mob->finalizeMobSpawn(nullptr);
        mob->setVillager(false);
    }
    // catch (Exception e) {
    //	e.printStackTrace();
    //	return false;
    // }
    mob->moveTo(spawnPos->x, spawnPos->y, spawnPos->z,
                level->random->nextFloat() * 360, 0);
    level->addEntity(mob);
    std::shared_ptr<Village> _village = village.lock();
    if (_village == nullptr) return false;

    Pos* center = _village->getCenter();
    mob->restrictTo(center->x, center->y, center->z, _village->getRadius());
    return true;
}

std::optional<Vec3> VillageSiege::findRandomSpawnPos(int x, int y, int z) {
    std::shared_ptr<Village> _village = village.lock();
    if (_village == nullptr) return std::nullopt;

    for (int i = 0; i < 10; ++i) {
        int xx = x + level->random->nextInt(16) - 8;
        int yy = y + level->random->nextInt(6) - 3;
        int zz = z + level->random->nextInt(16) - 8;
        if (!_village->isInside(xx, yy, zz)) continue;
        if (MobSpawner::isSpawnPositionOk(MobCategory::monster, level, xx, yy,
                                          zz))
            return Vec3(xx, yy, zz);
    }

    return std::nullopt;
}
