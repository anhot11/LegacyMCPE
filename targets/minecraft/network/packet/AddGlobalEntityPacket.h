#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Entity;

class AddGlobalEntityPacket
    : public Packet,
      public std::enable_shared_from_this<AddGlobalEntityPacket> {
public:
    static const int LIGHTNING;

    int id;
    int x, y, z;
    int type;

    AddGlobalEntityPacket();
    AddGlobalEntityPacket(std::shared_ptr<Entity> e);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<AddGlobalEntityPacket>();
    }
    virtual int getId() { return 71; }
};