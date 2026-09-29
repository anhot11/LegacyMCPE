#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Entity;

class SetEntityLinkPacket
    : public Packet,
      public std::enable_shared_from_this<SetEntityLinkPacket> {
public:
    static inline constexpr int RIDING = 0;
    static inline constexpr int LEASH = 1;

    int type;
    int sourceId, destId;

    SetEntityLinkPacket();
    SetEntityLinkPacket(int linkType, std::shared_ptr<Entity> sourceEntity,
                        std::shared_ptr<Entity> destEntity);

    virtual int getEstimatedSize();
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual bool canBeInvalidated();
    virtual bool isInvalidatedBy(std::shared_ptr<Packet> packet);

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<SetEntityLinkPacket>();
    }
    virtual int getId() { return 39; }
};