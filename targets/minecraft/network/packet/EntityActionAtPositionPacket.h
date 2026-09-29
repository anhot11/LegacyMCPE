#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Entity;

class EntityActionAtPositionPacket
    : public Packet,
      public std::enable_shared_from_this<EntityActionAtPositionPacket> {
public:
    static const int START_SLEEP;
    int id, x, y, z, action;

    EntityActionAtPositionPacket();
    EntityActionAtPositionPacket(std::shared_ptr<Entity> e, int action, int x,
                                 int y, int z);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<EntityActionAtPositionPacket>();
    }
    virtual int getId() { return 17; }
};