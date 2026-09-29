#pragma once

#include <cstdint>
#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class SetHealthPacket : public Packet,
                        public std::enable_shared_from_this<SetHealthPacket> {
public:
    float health;
    int food;
    float saturation;

    uint8_t damageSource;  // 4J Added

    SetHealthPacket();
    SetHealthPacket(float health, int food, float saturation,
                    uint8_t damageSource);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();
    virtual bool canBeInvalidated();
    virtual bool isInvalidatedBy(std::shared_ptr<Packet> packet);

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<SetHealthPacket>();
    }
    virtual int getId() { return 8; }
};
