#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "java/JavaIntHash.h"

class Entity;
class ServerLevel;
class ServerPlayer;
class LevelChunk;
class TrackedEntity;
class MinecraftServer;
class Packet;

class EntityTracker {
private:
    ServerLevel* level;
    std::unordered_set<std::shared_ptr<TrackedEntity> > entities;
    std::unordered_map<int, std::shared_ptr<TrackedEntity>, IntKeyHash2,
                       IntKeyEq>
        entityMap;  // was IntHashMap
    int maxRange;

public:
    EntityTracker(ServerLevel* level);
    void addEntity(std::shared_ptr<Entity> e);
    void addEntity(std::shared_ptr<Entity> e, int range, int updateInterval);
    void addEntity(std::shared_ptr<Entity> e, int range, int updateInterval,
                   bool trackDeltas);
    void removeEntity(std::shared_ptr<Entity> e);
    void removePlayer(std::shared_ptr<Entity> e);  // 4J added
    void tick();
    void broadcast(std::shared_ptr<Entity> e, std::shared_ptr<Packet> packet);
    void broadcastAndSend(std::shared_ptr<Entity> e,
                          std::shared_ptr<Packet> packet);
    void clear(std::shared_ptr<ServerPlayer> serverPlayer);
    void playerLoadedChunk(std::shared_ptr<ServerPlayer> player,
                           LevelChunk* chunk);
    void updateMaxRange();  // AP added for Vita

    // 4J-JEV: Added, needed access to tracked entity of a riders mount.
    std::shared_ptr<TrackedEntity> getTracker(std::shared_ptr<Entity> entity);
};
