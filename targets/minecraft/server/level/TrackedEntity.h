#pragma once
#include <format>
#include <memory>
#include <unordered_set>
#include <vector>

// class ServerPlayer;
#include "ServerPlayer.h"
#include "minecraft/world/entity/player/Player.h"

class Entity;
class Packet;
class EntityTracker;
class PlayerConnection;
class ServerPlayer;

#define TRACKED_ENTITY_MINIMUM_VIEW_DISTANCE 4

class TrackedEntity {
private:
    static const int TOLERANCE_LEVEL = 4;

public:
    std::shared_ptr<Entity> e;

    int range, updateInterval;
    int xp, yp, zp, yRotp, xRotp, yHeadRotp;
    double xap, yap, zap;
    int tickCount;

private:
    double xpu, ypu, zpu;
    bool updatedPlayerVisibility;
    bool trackDelta;
    int teleportDelay;
    std::shared_ptr<Entity> lastRidingEntity;
    bool wasRiding;

public:
    bool moved;

    std::unordered_set<std::shared_ptr<ServerPlayer>, PlayerKeyHash,
                       PlayerKeyEq>
        seenBy;

    TrackedEntity(std::shared_ptr<Entity> e, int range, int updateInterval,
                  bool trackDelta);

    void tick(EntityTracker* tracker,
              std::vector<std::shared_ptr<Player> >* players);

private:
    void sendDirtyEntityData();

public:
    void broadcast(std::shared_ptr<Packet> packet);
    void broadcastAndSend(std::shared_ptr<Packet> packet);
    void broadcastRemoved();
    void removePlayer(std::shared_ptr<ServerPlayer> sp);

private:
    bool canBySeenBy(std::shared_ptr<ServerPlayer> player);

    enum eVisibility {
        eVisibility_NotVisible = 0,
        eVisibility_IsVisible = 1,
        eVisibility_SeenAndVisible = 2,
    };

    eVisibility isVisible(EntityTracker* tracker,
                          std::shared_ptr<ServerPlayer> sp,
                          bool forRider = false);  // 4J Added forRider

public:
    void updatePlayer(EntityTracker* tracker, std::shared_ptr<ServerPlayer> sp);
    void updatePlayers(EntityTracker* tracker,
                       std::vector<std::shared_ptr<Player> >* players);

private:
    void sendEntityData(std::shared_ptr<PlayerConnection> conn);
    std::shared_ptr<Packet> getAddEntityPacket();

public:
    void clear(std::shared_ptr<ServerPlayer> sp);
};
