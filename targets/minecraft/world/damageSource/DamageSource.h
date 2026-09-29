#pragma once

#include <memory>

#include "minecraft/network/packet/ChatPacket.h"

class LivingEntity;
class Entity;
class Arrow;
class Fireball;
class Player;
class Explosion;

class DamageSource {
public:
    static DamageSource* inFire;
    static DamageSource* onFire;
    static DamageSource* lava;
    static DamageSource* inWall;
    static DamageSource* drown;
    static DamageSource* starve;
    static DamageSource* cactus;
    static DamageSource* fall;
    static DamageSource* outOfWorld;
    static DamageSource* genericSource;
    static DamageSource* magic;
    static DamageSource* dragonbreath;
    static DamageSource* wither;
    static DamageSource* anvil;
    static DamageSource* fallingBlock;

    static DamageSource* mobAttack(std::shared_ptr<LivingEntity> mob);
    static DamageSource* playerAttack(std::shared_ptr<Player> player);
    static DamageSource* arrow(std::shared_ptr<Arrow> arrow,
                               std::shared_ptr<Entity> owner);
    static DamageSource* fireball(std::shared_ptr<Fireball> fireball,
                                  std::shared_ptr<Entity> owner);
    static DamageSource* thrown(std::shared_ptr<Entity> entity,
                                std::shared_ptr<Entity> owner);
    static DamageSource* indirectMagic(std::shared_ptr<Entity> entity,
                                       std::shared_ptr<Entity> owner);
    static DamageSource* thorns(std::shared_ptr<Entity> source);
    static DamageSource* explosion(Explosion* explosion);

private:
    bool _bypassArmor;
    bool _bypassInvul;
    // food exhastion caused by being damaged by this source
    float exhaustion;
    bool isFireSource;
    bool _isProjectile;
    bool _scalesWithDifficulty;
    bool _isMagic;
    bool _isExplosion;

public:
    bool isProjectile();
    DamageSource* setProjectile();
    bool isExplosion();
    DamageSource* setExplosion();

    bool isBypassArmor();
    float getFoodExhaustion();
    bool isBypassInvul();

    // std::string msgId;
    ChatPacket::EChatPacketMessage m_msgId;  // 4J Made int so we can localise
    ChatPacket::EChatPacketMessage
        m_msgWithItemId;  // 4J: Renamed from m_msgWithSourceId (it was already
                          // renamed in places, just made consistent)

protected:
    // DamageSource(const std::string &msgId);
    DamageSource(ChatPacket::EChatPacketMessage msgId,
                 ChatPacket::EChatPacketMessage msgWithItemId =
                     ChatPacket::e_ChatCustom);

public:
    virtual ~DamageSource() {}

    virtual std::shared_ptr<Entity> getDirectEntity();
    virtual std::shared_ptr<Entity> getEntity();

protected:
    DamageSource* bypassArmor();
    DamageSource* bypassInvul();
    DamageSource* setIsFire();
    DamageSource* setScalesWithDifficulty();

public:
    virtual bool scalesWithDifficulty();

    bool isMagic();
    DamageSource* setMagic();

    // 4J Stu - Made return a packet
    // virtual std::string getLocalizedDeathMessage(std::shared_ptr<Player>
    // player);
    virtual std::shared_ptr<ChatPacket> getDeathMessagePacket(
        std::shared_ptr<LivingEntity> player);

    bool isFire();
    ChatPacket::EChatPacketMessage
    getMsgId();  // 4J Stu - Used to return String

    // 4J Added
    bool equals(DamageSource* source);
    virtual DamageSource* copy();
};