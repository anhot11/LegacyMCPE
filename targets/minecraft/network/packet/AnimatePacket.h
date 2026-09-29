#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Entity;

class AnimatePacket : public Packet,
                      public std::enable_shared_from_this<AnimatePacket> {
public:
    static inline constexpr int SWING = 1;
    static inline constexpr int HURT = 2;
    static inline constexpr int WAKE_UP = 3;
    static inline constexpr int RESPAWN = 4;
    static inline constexpr int EAT = 5;  // 1.8.2
    static inline constexpr int CRITICAL_HIT = 6;
    static inline constexpr int MAGIC_CRITICAL_HIT = 7;

    int id;
    int action;

    AnimatePacket();
    AnimatePacket(std::shared_ptr<Entity> e, int action);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<AnimatePacket>();
    }
    virtual int getId() { return 18; }
};