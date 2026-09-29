#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class SetExperiencePacket
    : public Packet,
      public std::enable_shared_from_this<SetExperiencePacket> {
public:
    float experienceProgress;
    int totalExperience;
    int experienceLevel;

    SetExperiencePacket();
    SetExperiencePacket(float experienceProgress, int totalExperience,
                        int experienceLevel);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();
    virtual bool canBeInvalidated();
    virtual bool isInvalidatedBy(std::shared_ptr<Packet> packet);

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<SetExperiencePacket>();
    }
    virtual int getId() { return 43; }
};