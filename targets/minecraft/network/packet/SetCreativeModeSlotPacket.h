#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class ItemInstance;

class SetCreativeModeSlotPacket
    : public Packet,
      public std::enable_shared_from_this<SetCreativeModeSlotPacket> {
public:
    int slotNum;
    std::shared_ptr<ItemInstance> item;

    SetCreativeModeSlotPacket();
    SetCreativeModeSlotPacket(int slotNum, std::shared_ptr<ItemInstance> item);

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<SetCreativeModeSlotPacket>();
    }
    virtual int getId() { return 107; }
};