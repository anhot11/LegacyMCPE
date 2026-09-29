#include "MobSpawner.h"

#include <utility>
#include <vector>

#include "Level.h"
#include "java/Class.h"
#include "java/Random.h"
#include "minecraft/Pos.h"
#include "minecraft/server/level/ServerLevel.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/Difficulty.h"
#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/entity/EntityIO.h"
#include "minecraft/world/entity/Mob.h"
#include "minecraft/world/entity/MobCategory.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/level/ChunkPos.h"
#include "minecraft/world/level/TilePos.h"
#include "minecraft/world/level/biome/Biome.h"
#include "minecraft/world/level/dimension/Dimension.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/Tile.h"

class MobGroupData;

const int MobSpawner::MIN_SPAWN_DISTANCE = 24;

TilePos MobSpawner::getRandomPosWithin(Level* level, int cx, int cz) {
    // 4J Stu - Added 1.2.3 but we don't need it as it was only used to access
    // sections Leaving here though to help explain why chunk coords are not
    // passed in rather than full coords
    // LevelChunk *chunk = level->getChunk(cx, cz);
    int x = cx * 16 + level->random->nextInt(16);
    int y = level->random->nextInt(level->getHeight());
    int z = cz * 16 + level->random->nextInt(16);

    return TilePos(x, y, z);
}

std::unordered_map<ChunkPos, bool, ChunkPosKeyHash, ChunkPosKeyEq>
    MobSpawner::chunksToPoll;

const int MobSpawner::tick(ServerLevel* level, bool spawnEnemies,
                           bool spawnFriendlies, bool spawnPersistent) {
    if (!spawnEnemies && !spawnFriendlies && !spawnPersistent) {
        return 0;
    }
    chunksToPoll.clear();

    // 4J - rewritten to add chunks interleaved by player, and to add them from
    // the centre outwards. We're going to be potentially adding less creatures
    // than the original so that our count stays consistent with number of
    // players added, so we want to make sure as best we can that the ones we do
    // add are near the active players
    int playerCount = (int)level->players.size();
    int* xx = new int[playerCount];
    int* zz = new int[playerCount];
    for (int i = 0; i < playerCount; i++) {
        std::shared_ptr<Player> player = level->players[i];
        xx[i] = Mth::floor(player->x / 16);
        zz[i] = Mth::floor(player->z / 16);
        chunksToPoll.insert(
            std::pair<ChunkPos, bool>(ChunkPos(xx[i], zz[i]), false));
    }

    for (int r = 1; r <= 8; r++) {
        for (int l = 0; l < (r * 2); l++) {
            for (int i = 0; i < playerCount; i++) {
                bool edgeChunk = (r == 8);

                // If this chunk isn't at the edge of the region for this
                // player, then always store with a flag of false so that if it
                // was at the edge of another player, then this will remove that
                if (!edgeChunk) {
                    chunksToPoll.insert(std::pair<ChunkPos, bool>(
                        ChunkPos((xx[i] - r) + l, (zz[i] - r)), false));
                    chunksToPoll.insert(std::pair<ChunkPos, bool>(
                        ChunkPos((xx[i] + r), (zz[i] - r) + l), false));
                    chunksToPoll.insert(std::pair<ChunkPos, bool>(
                        ChunkPos((xx[i] + r) - l, (zz[i] + r)), false));
                    chunksToPoll.insert(std::pair<ChunkPos, bool>(
                        ChunkPos((xx[i] - r), (zz[i] + r) - l), false));
                } else {
                    ChunkPos cp = ChunkPos((xx[i] - r) + l, (zz[i] - r));
                    if (chunksToPoll.find(cp) == chunksToPoll.end())
                        chunksToPoll.insert(
                            std::pair<ChunkPos, bool>(cp, true));
                    cp = ChunkPos((xx[i] + r), (zz[i] - r) + l);
                    if (chunksToPoll.find(cp) == chunksToPoll.end())
                        chunksToPoll.insert(
                            std::pair<ChunkPos, bool>(cp, true));
                    cp = ChunkPos((xx[i] + r) - l, (zz[i] + r));
                    if (chunksToPoll.find(cp) == chunksToPoll.end())
                        chunksToPoll.insert(
                            std::pair<ChunkPos, bool>(cp, true));
                    cp = ChunkPos((xx[i] - r), (zz[i] + r) - l);
                    if (chunksToPoll.find(cp) == chunksToPoll.end())
                        chunksToPoll.insert(
                            std::pair<ChunkPos, bool>(cp, true));
                }
            }
        }
    }
    delete[] xx;
    delete[] zz;
    int count = 0;
    Pos* spawnPos = level->getSharedSpawnPos();

    for (unsigned int i = 0; i < MobCategory::values.size(); i++) {
        MobCategory* mobCategory = MobCategory::values[i];
        if ((mobCategory->isFriendly() && !spawnFriendlies) ||
            (!mobCategory->isFriendly() && !spawnEnemies) ||
            (mobCategory->isPersistent() && !spawnPersistent)) {
            continue;
        }

        // 4J - early out for non-main dimensions, if spawning anything friendly
        if (mobCategory->isFriendly()) {
            if (level->dimension->id != 0) {
                continue;
            }
        }

        // 4J - this is now quite different to the java version. We just have
        // global max counts for the level whereas the original has a max per
        // chunk that scales with the number of chunks to be polled.
        int categoryCount = level->countInstanceOf(
            mobCategory->getEnumBaseClass(), mobCategory->isSingleType());
        if (categoryCount >= mobCategory->getMaxInstancesPerLevel()) {
            continue;
        }

        auto itEndCTP = chunksToPoll.end();
        for (auto it = chunksToPoll.begin(); it != itEndCTP; it++) {
            if (it->second) {
                // don't add mobs to edge chunks, to prevent adding mobs
                // "outside" of the active playground
                continue;
            }
            ChunkPos* cp = (ChunkPos*)(&it->first);

            // 4J - don't let this actually create/load a chunk that isn't here
            // already - we'll let the normal updateDirtyChunks etc. processes
            // do that, so it can happen on another thread
            if (!level->hasChunk(cp->x, cp->z)) continue;

            TilePos start = getRandomPosWithin(level, cp->x, cp->z);
            int xStart = start.x;
            int yStart = start.y;
            int zStart = start.z;

            if (level->isSolidBlockingTile(xStart, yStart, zStart)) continue;
            if (level->getMaterial(xStart, yStart, zStart) !=
                mobCategory->getSpawnPositionMaterial())
                continue;
            int clusterSize = 0;

            for (int dd = 0; dd < 3; dd++) {
                int x = xStart;
                int y = yStart;
                int z = zStart;
                int ss = 6;

                Biome::MobSpawnerData* currentMobType = nullptr;
                MobGroupData* groupData = nullptr;

                for (int ll = 0; ll < 4; ll++) {
                    x +=
                        level->random->nextInt(ss) - level->random->nextInt(ss);
                    y += level->random->nextInt(1) - level->random->nextInt(1);
                    z +=
                        level->random->nextInt(ss) - level->random->nextInt(ss);
                    // int y = heightMap[x + z * w] + 1;

                    // 4J - don't let this actually create/load a chunk that
                    // isn't here already - we'll let the normal
                    // updateDirtyChunks etc. processes do that, so it can
                    // happen on another thread
                    if (!level->hasChunkAt(x, y, z)) continue;

                    if (isSpawnPositionOk(mobCategory, level, x, y, z)) {
                        float xx = x + 0.5f;
                        float yy = (float)y;
                        float zz = z + 0.5f;
                        if (level->getNearestPlayer(
                                xx, yy, zz, MIN_SPAWN_DISTANCE) != nullptr) {
                            continue;
                        } else {
                            float xd = xx - spawnPos->x;
                            float yd = yy - spawnPos->y;
                            float zd = zz - spawnPos->z;
                            float sd = xd * xd + yd * yd + zd * zd;
                            if (sd < MIN_SPAWN_DISTANCE * MIN_SPAWN_DISTANCE) {
                                continue;
                            }
                        }

                        if (currentMobType == nullptr) {
                            currentMobType = level->getRandomMobSpawnAt(
                                mobCategory, x, y, z);
                            if (currentMobType == nullptr) {
                                break;
                            }
                        }

                        std::shared_ptr<Mob> mob;
                        // 4J - removed try/catch
                        //						   try
                        //						   {
                        // mob =
                        // type.mobClass.getConstructor(Level.class).newInstance(level);
                        mob = std::dynamic_pointer_cast<Mob>(
                            EntityIO::newByEnumType(currentMobType->mobClass,
                                                    level));
                        //						   }
                        //						   catch
                        //(exception e)
                        //						   {
                        //							   //
                        // TODO 4J We can't print a stack trace, and the
                        // newInstance function doesn't throw an exception just
                        // now anyway
                        //							   //e.printStackTrace();
                        //							   return
                        // count;
                        //						   }

                        // 4J - If it is an animal or a monster, don't let any
                        // one type of mob represent more than 50% of the total
                        // amount of these things. This was added initially to
                        // stop flat lands being totally populated with slimes
                        // but seems like a generally good rule.
                        eINSTANCEOF mobType = mob->GetType();

                        if ((mobType & eTYPE_ANIMALS_SPAWN_LIMIT_CHECK) ||
                            (mobType & eTYPE_MONSTER)) {
                            // even more special rule for ghasts, because
                            // filling up the nether with 25 of them is a bit
                            // unpleasant. In the java version they are only
                            // limited by the fact that the world fills up with
                            // pig zombies (the only other type of enemy mob in
                            // the nether) before them - they aren't actually
                            // even counted properly themselves
                            if (mobType == eTYPE_GHAST) {
                                if (level->countInstanceOf(mobType, true) >= 4)
                                    continue;
                            } else if (mobType == eTYPE_ENDERMAN &&
                                       level->dimension->id == 1) {
                                // Special rule for the end, as we only have
                                // Endermen (plus the dragon). Increase the
                                // spawnable counts based on level difficulty
                                int maxEndermen =
                                    mobCategory->getMaxInstancesPerLevel();

                                if (level->difficulty == Difficulty::NORMAL) {
                                    maxEndermen -=
                                        mobCategory->getMaxInstancesPerLevel() /
                                        4;
                                } else if (level->difficulty <=
                                           Difficulty::EASY) {
                                    maxEndermen -=
                                        mobCategory->getMaxInstancesPerLevel() /
                                        2;
                                }

                                if (level->countInstanceOf(mobType, true) >=
                                    maxEndermen)
                                    continue;
                            } else if (level->countInstanceOf(mobType, true) >=
                                       (mobCategory->getMaxInstancesPerLevel() /
                                        2))
                                continue;
                        }

                        mob->moveTo(xx, yy, zz,
                                    level->random->nextFloat() * 360, 0);

                        if (mob->canSpawn()) {
                            // 4J - check if we are going to despawn straight
                            // away too, and don't add if we will - otherwise
                            // we'll be sending network packets for adding &
                            // removal that we don't need
                            mob->checkDespawn();
                            if (!mob->removed) {
                                clusterSize++;
                                categoryCount++;
                                mob->setDespawnProtected();  // 4J added -
                                                             // default to
                                                             // protected
                                                             // against
                                                             // despawning
                                level->addEntity(mob);
                                groupData = mob->finalizeMobSpawn(groupData);
                                // 4J - change here so that we can't ever make
                                // more than the desired amount of entities in
                                // each priority. In the original java version
                                // depending on the random spawn positions being
                                // considered the only limit as to the number of
                                // entities created per category is the number
                                // of chunks to poll.
                                if (categoryCount >=
                                    mobCategory->getMaxInstancesPerLevel())
                                    goto categoryLoop;
                                if (clusterSize >=
                                    mob->getMaxSpawnClusterSize())
                                    goto chunkLoop;
                            }
                        }
                        count += clusterSize;
                    }
                }
            }
        chunkLoop:
            continue;
        }
    categoryLoop:
        continue;
    }
    delete spawnPos;

    return count;
}

bool MobSpawner::isSpawnPositionOk(MobCategory* category, Level* level, int x,
                                   int y, int z) {
    // 4J - don't let this actually create/load a chunk that isn't here already
    // - we'll let the normal updateDirtyChunks etc. processes do that, so it
    // can happen on another thread
    if (!level->hasChunkAt(x, y, z)) return false;

    if (category->getSpawnPositionMaterial() == Material::water) {
        // 4J - changed to spawn water things only in deep water
        int yo = 0;
        int liquidCount = 0;

        while ((y - yo) >= 0 && (yo < 5)) {
            if (level->getMaterial(x, y - yo, z)->isLiquid()) liquidCount++;
            yo++;
        }

        // 4J - Sometimes deep water could be just a waterfall, so check that
        // it's wide as well
        bool inEnoughWater = false;
        if (liquidCount == 5) {
            if (level->getMaterial(x + 5, y, z)->isLiquid() &&
                level->getMaterial(x - 5, y, z)->isLiquid() &&
                level->getMaterial(x, y, z + 5)->isLiquid() &&
                level->getMaterial(x, y, z - 5)->isLiquid()) {
                inEnoughWater = true;
            }
        }

        return inEnoughWater && !level->isSolidBlockingTile(x, y + 1, z);
    } else {
        if (!level->isTopSolidBlocking(x, y - 1, z)) return false;
        int tt = level->getTile(x, y - 1, z);
        return tt != Tile::unbreakable_Id &&
               !level->isSolidBlockingTile(x, y, z) &&
               !level->getMaterial(x, y, z)->isLiquid() &&
               !level->isSolidBlockingTile(x, y + 1, z);
    }
}

void MobSpawner::postProcessSpawnMobs(Level* level, Biome* biome, int xo,
                                      int zo, int cellWidth, int cellHeight,
                                      Random* random) {
    // 4J - not for our version. Creates a few too many mobs.
}
