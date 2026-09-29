#pragma once

#include <format>
#include <memory>
#include <unordered_map>
#include <vector>

#include "minecraft/world/entity/Mob.h"
#include "minecraft/world/level/TilePos.h"

class Player;
class Level;
class Biome;
class ChunkPos;
class MobCategory;
class Random;
class ServerLevel;
struct ChunkPosKeyEq;
struct ChunkPosKeyHash;

class MobSpawner {
private:
    static const int MIN_SPAWN_DISTANCE;

protected:
    static TilePos getRandomPosWithin(Level* level, int cx, int cz);

private:
    static std::unordered_map<ChunkPos, bool, ChunkPosKeyHash, ChunkPosKeyEq>
        chunksToPoll;

public:
    static const int tick(ServerLevel* level, bool spawnEnemies,
                          bool spawnFriendlies, bool spawnPersistent);
    static bool isSpawnPositionOk(MobCategory* category, Level* level, int x,
                                  int y, int z);

public:
    static bool attackSleepingPlayers(
        Level* level, std::vector<std::shared_ptr<Player> >* players);

    static void postProcessSpawnMobs(Level* level, Biome* biome, int xo, int zo,
                                     int cellWidth, int cellHeight,
                                     Random* random);
};
