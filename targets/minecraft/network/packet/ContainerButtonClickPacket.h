#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class ContainerButtonClickPacket
    : public Packet,
      public std::enable_shared_from_this<ContainerButtonClickPacket> {
public:
    int containerId;
    int buttonId;

    ContainerButtonClickPacket();
    ContainerButtonClickPacket(int containerId, int buttonId);

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<ContainerButtonClickPacket>();
    }
    virtual int getId() { return 108; }
};