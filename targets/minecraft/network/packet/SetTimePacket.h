#pragma once

#include <stdint.h>

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class SetTimePacket : public Packet,
                      public std::enable_shared_from_this<SetTimePacket> {
public:
    int64_t gameTime;
    int64_t dayTime;

    SetTimePacket();
    SetTimePacket(int64_t gameTime, int64_t dayTime, bool tickDayTime);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();
    virtual bool canBeInvalidated();
    virtual bool isInvalidatedBy(std::shared_ptr<Packet> packet);
    virtual bool isAync();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<SetTimePacket>();
    }
    virtual int getId() { return 4; }
};