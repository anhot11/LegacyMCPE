#pragma once
#include <cstdint>
#include <format>
#include <memory>
#include <string>
#include <vector>

#include "minecraft/SharedConstants.h"
#include "minecraft/client/player/LocalPlayer.h"
#include "minecraft/world/entity/item/ItemEntity.h"
#include "minecraft/world/entity/player/Player.h"

class ClientConnection;
class Minecraft;
class Level;
class Stat;
class User;

// #define STRESS_TEST_MOVE

class MultiplayerLocalPlayer : public LocalPlayer {
private:
    static const int POSITION_REMINDER_INTERVAL =
        SharedConstants::TICKS_PER_SECOND;

public:
    ClientConnection* connection;

private:
    bool flashOnSetHealth;

public:
    MultiplayerLocalPlayer(Minecraft* minecraft, Level* level, User* user,
                           ClientConnection* connection);

private:
    double xLast, yLast1, yLast2, zLast;
    float yRotLast, xRotLast;

public:
    virtual bool hurt(DamageSource* source, float dmg);
    virtual void heal(float heal);
    virtual void tick();

private:
    bool lastOnGround;
    bool lastSneaked;
    bool lastIdle;
    bool lastSprinting;
    int positionReminder;

public:
    void sendPosition();

    using Player::drop;
    virtual std::shared_ptr<ItemEntity> drop();

protected:
    virtual void reallyDrop(std::shared_ptr<ItemEntity> itemEntity);

public:
    virtual void chat(const std::string& message);
    virtual void swing();
    virtual void respawn();

protected:
    virtual void actuallyHurt(DamageSource* source, float dmg);

    // 4J Added override to capture event for tutorial messages
    virtual void completeUsingItem();

    // 4J Added overrides to capture events for tutorial
    virtual void onEffectAdded(MobEffectInstance* effect);
    virtual void onEffectUpdated(MobEffectInstance* effect,
                                 bool doRefreshAttributes);
    virtual void onEffectRemoved(MobEffectInstance* effect);

public:
    virtual void closeContainer();
    void clientSideCloseContainer();
    virtual void hurtTo(float newHealth, uint8_t damageSource);
    virtual void awardStat(Stat* stat, const std::vector<uint8_t>& param);
    void awardStatFromServer(Stat* stat, std::vector<uint8_t>& param);
    void onUpdateAbilities();
    bool isLocalPlayer();

protected:
    virtual void sendRidingJump();

public:
    virtual void sendOpenInventory();

    // 4J - send the custom skin texture data if there is one
    // void CustomSkin(uint8_t* pbData, uint32_t dwBytes);

    // 4J Overriding this so we can flag an event for the tutorial
    virtual void ride(std::shared_ptr<Entity> e);

    // 4J - added for the Stop Sleeping
    virtual void StopSleeping();

    // 4J Added
    virtual void setAndBroadcastCustomSkin(uint32_t skinId);
    virtual void setAndBroadcastCustomCape(uint32_t capeId);

    // 4J added for testing
#ifdef STRESS_TEST_MOVE
    void StressTestMove(double* tempX, double* tempY, double* tempZ);
#endif
};
