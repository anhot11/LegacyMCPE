#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class MobEffectInstance;

class RemoveMobEffectPacket
    : public Packet,
      public std::enable_shared_from_this<RemoveMobEffectPacket> {
public:
    int entityId;
    char effectId;

    RemoveMobEffectPacket();
    RemoveMobEffectPacket(int entityId, MobEffectInstance* effect);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<RemoveMobEffectPacket>();
    }
    virtual int getId() { return 42; }
};