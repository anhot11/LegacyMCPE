#pragma once

#include <stdint.h>

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class EntityEventPacket
    : public Packet,
      public std::enable_shared_from_this<EntityEventPacket> {
public:
    int entityId;
    uint8_t eventId;

    EntityEventPacket();
    EntityEventPacket(int entityId, uint8_t eventId);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<EntityEventPacket>();
    }
    virtual int getId() { return 38; }
};
