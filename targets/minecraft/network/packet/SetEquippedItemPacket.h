#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"
#include "minecraft/world/item/ItemInstance.h"

class SetEquippedItemPacket
    : public Packet,
      public std::enable_shared_from_this<SetEquippedItemPacket> {
public:
    int entity;
    int slot;

private:
    // 4J Stu - Brought forward from 1.3 to fix #64688 - Customer Encountered:
    // TU7: Content: Art: Aura of enchanted item is not displayed for other
    // players in online game
    std::shared_ptr<ItemInstance> item;

public:
    SetEquippedItemPacket();
    SetEquippedItemPacket(int entity, int slot,
                          std::shared_ptr<ItemInstance> item);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();
    virtual bool canBeInvalidated();
    virtual bool isInvalidatedBy(std::shared_ptr<Packet> packet);

    // 4J Stu - Brought forward from 1.3 to fix #64688 - Customer Encountered:
    // TU7: Content: Art: Aura of enchanted item is not displayed for other
    // players in online game
    std::shared_ptr<ItemInstance> getItem();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<SetEquippedItemPacket>();
    }
    virtual int getId() { return 5; }
};