#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class SetSpawnPositionPacket
    : public Packet,
      public std::enable_shared_from_this<SetSpawnPositionPacket> {
public:
    int x, y, z;

    SetSpawnPositionPacket();
    SetSpawnPositionPacket(int x, int y, int z);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();
    virtual bool canBeInvalidated();
    virtual bool isInvalidatedBy(std::shared_ptr<Packet> packet);
    virtual bool isAync();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<SetSpawnPositionPacket>();
    }
    virtual int getId() { return 6; }
};