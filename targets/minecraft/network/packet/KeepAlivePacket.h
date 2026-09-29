#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class KeepAlivePacket : public Packet,
                        public std::enable_shared_from_this<KeepAlivePacket> {
public:
    int id;

    KeepAlivePacket();
    KeepAlivePacket(int id);

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();
    virtual bool canBeInvalidated();
    virtual bool isInvalidatedBy(std::shared_ptr<Packet> packet);
    virtual bool isAync();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<KeepAlivePacket>();
    }
    virtual int getId() { return 0; }
};