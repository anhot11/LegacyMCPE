#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class TileDestructionPacket
    : public Packet,
      public std::enable_shared_from_this<TileDestructionPacket> {
private:
    int id;
    int x;
    int y;
    int z;
    int state;

public:
    TileDestructionPacket();
    TileDestructionPacket(int id, int x, int y, int z, int state);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

    int getEntityId();
    int getX();
    int getY();
    int getZ();
    int getState();

    virtual bool canBeInvalidated();
    virtual bool isInvalidatedBy(std::shared_ptr<Packet> packet);

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<TileDestructionPacket>();
    }
    virtual int getId() { return 55; }
};