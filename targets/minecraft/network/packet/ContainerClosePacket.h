#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class ContainerClosePacket
    : public Packet,
      public std::enable_shared_from_this<ContainerClosePacket> {
public:
    int containerId;

    ContainerClosePacket();
    ContainerClosePacket(int containerId);

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<ContainerClosePacket>();
    }
    virtual int getId() { return 101; }
};
