#pragma once
#include <format>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "TilePos.h"
#include "minecraft/world/entity/LivingEntity.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/level/TilePos.h"
#include "minecraft/world/phys/Vec3.h"

class Random;
class Level;
class Entity;

class Explosion {
public:
    bool fire;
    bool destroyBlocks;

private:
    int size;

    Random* random;
    Level* level;

public:
    double x, y, z;
    std::shared_ptr<Entity> source;
    float r;

    std::unordered_set<TilePos, TilePosKeyHash, TilePosKeyEq> toBlow;

private:
    typedef std::unordered_map<std::shared_ptr<Player>, Vec3, PlayerKeyHash,
                               PlayerKeyEq>
        playerVec3Map;
    playerVec3Map hitPlayers;

public:
    Explosion(Level* level, std::shared_ptr<Entity> source, double x, double y,
              double z, float r);
    ~Explosion();

public:
    void explode();

public:
    void finalizeExplosion(bool generateParticles,
                           std::vector<TilePos>* toBlowDirect =
                               nullptr);  // 4J - added toBlow parameter
    playerVec3Map* getHitPlayers();
    Vec3 getHitPlayerKnockback(std::shared_ptr<Player> player);
    std::shared_ptr<LivingEntity> getSourceMob();
};
