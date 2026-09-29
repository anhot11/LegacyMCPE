#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class ItemInstance;

class ContainerSetSlotPacket
    : public Packet,
      public std::enable_shared_from_this<ContainerSetSlotPacket> {
public:
    static const int CONTAINER;
    static const int WORKBENCH;
    static const int FURNACE;

    int containerId;
    int slot;
    std::shared_ptr<ItemInstance> item;

    ContainerSetSlotPacket();
    ContainerSetSlotPacket(int containerId, int slot,
                           std::shared_ptr<ItemInstance> item);

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<ContainerSetSlotPacket>();
    }
    virtual int getId() { return 103; }
};
