#pragma once

#include <stdint.h>

#include <format>
#include <memory>
#include <vector>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class AwardStatPacket : public Packet,
                        public std::enable_shared_from_this<AwardStatPacket> {
public:
    int statId;

    // 4J-JEV: Changed to allow for Durango events.
protected:
    std::vector<uint8_t> m_paramData;

public:
    AwardStatPacket();
    AwardStatPacket(int statId, int count);
    AwardStatPacket(int statId, std::vector<uint8_t>& paramData);
    ~AwardStatPacket();

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();
    virtual bool isAync();

    static std::shared_ptr<Packet> create() {
        return std::make_shared<AwardStatPacket>();
    }
    virtual int getId() { return 200; }

public:
    // 4J-JEV: New getters to help prevent unsafe access
    int getCount();
    std::vector<uint8_t> getParamData();
};
