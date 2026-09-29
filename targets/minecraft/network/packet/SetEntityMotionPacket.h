#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Entity;

class SetEntityMotionPacket
    : public Packet,
      public std::enable_shared_from_this<SetEntityMotionPacket> {
public:
    int id;
    int xa, ya, za;
    bool useBytes;  // 4J added

private:
    void _init(int id, double xd, double yd, double zd);

public:
    SetEntityMotionPacket();
    SetEntityMotionPacket(std::shared_ptr<Entity> e);
    SetEntityMotionPacket(int id, double xd, double yd, double zd);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();
    virtual bool canBeInvalidated();
    virtual bool isInvalidatedBy(std::shared_ptr<Packet> packet);

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<SetEntityMotionPacket>();
    }
    virtual int getId() { return 28; }
};