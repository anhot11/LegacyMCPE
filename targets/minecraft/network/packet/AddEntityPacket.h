#pragma once

#include <stdint.h>

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Entity;

class AddEntityPacket : public Packet,
                        public std::enable_shared_from_this<AddEntityPacket> {
public:
    static inline constexpr int BOAT = 1;
    static inline constexpr int ITEM = 2;
    static inline constexpr int MINECART = 10;
    static inline constexpr int PRIMED_TNT = 50;
    static inline constexpr int ENDER_CRYSTAL = 51;
    static inline constexpr int ARROW = 60;
    static inline constexpr int SNOWBALL = 61;
    static inline constexpr int EGG = 62;
    static inline constexpr int FIREBALL = 63;
    static inline constexpr int SMALL_FIREBALL = 64;
    static inline constexpr int THROWN_ENDERPEARL = 65;
    static inline constexpr int WITHER_SKULL = 66;
    static inline constexpr int FALLING = 70;
    static inline constexpr int ITEM_FRAME = 71;
    static inline constexpr int EYEOFENDERSIGNAL = 72;
    static inline constexpr int THROWN_POTION = 73;
    static inline constexpr int FALLING_EGG = 74;
    static inline constexpr int THROWN_EXPBOTTLE = 75;
    static inline constexpr int FIREWORKS = 76;
    static inline constexpr int LEASH_KNOT = 77;
    static inline constexpr int FISH_HOOK = 90;

    // 4J Added TU9
    static inline constexpr int DRAGON_FIRE_BALL = 200;

    int id;
    int x, y, z;
    int xa, ya, za;
    int type;
    int data;
    uint8_t yRot, xRot;  // 4J added

private:
    void _init(std::shared_ptr<Entity> e, int type, int data, int xp, int yp,
               int zp, int yRotp, int xRotp);

public:
    AddEntityPacket();
    AddEntityPacket(std::shared_ptr<Entity> e, int type, int yRotp, int xRotp,
                    int xp, int yp, int zp);
    AddEntityPacket(std::shared_ptr<Entity> e, int type, int data, int yRotp,
                    int xRotp, int xp, int yp, int zp);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<AddEntityPacket>();
    }
    virtual int getId() { return 23; }
};