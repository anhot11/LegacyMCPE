#pragma once

// 4J ADDED PACKET

#include <iostream>
#include <memory>

#include "Packet.h"
#include "PacketListener.h"
#include "minecraft/network/packet/Packet.h"

class TradeItemPacket : public Packet,
                        public std::enable_shared_from_this<TradeItemPacket> {
public:
    int containerId;
    int offer;

    TradeItemPacket();
    TradeItemPacket(int containerId, int offer);

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<TradeItemPacket>();
    }
    virtual int getId() { return 151; }
};
