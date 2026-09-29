#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class ItemInstance;

class ContainerClickPacket
    : public Packet,
      public std::enable_shared_from_this<ContainerClickPacket> {
public:
    int containerId;
    int slotNum;
    int buttonNum;
    short uid;
    std::shared_ptr<ItemInstance> item;
    int clickType;

    ContainerClickPacket();
    ~ContainerClickPacket();
    ContainerClickPacket(int containerId, int slotNum, int buttonNum,
                         int clickType, std::shared_ptr<ItemInstance> item,
                         short uid);

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<ContainerClickPacket>();
    }
    virtual int getId() { return 102; }
};
