#pragma once

// 4J ADDED THIS PACKET

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class DebugOptionsPacket
    : public Packet,
      public std::enable_shared_from_this<DebugOptionsPacket> {
public:
    unsigned int m_uiVal;

    DebugOptionsPacket();
    ~DebugOptionsPacket();
    DebugOptionsPacket(unsigned int uiVal);

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<DebugOptionsPacket>();
    }
    virtual int getId() { return 152; }
};
