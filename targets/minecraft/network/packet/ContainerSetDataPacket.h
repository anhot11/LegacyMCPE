#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class ContainerSetDataPacket
    : public Packet,
      public std::enable_shared_from_this<ContainerSetDataPacket> {
public:
    int containerId;
    int id;
    int value;

    ContainerSetDataPacket();
    ContainerSetDataPacket(int containerId, int id, int value);

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<ContainerSetDataPacket>();
    }
    virtual int getId() { return 105; }
};