#pragma once

// 4J ADDED THIS PACKET

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class CraftItemPacket : public Packet,
                        public std::enable_shared_from_this<CraftItemPacket> {
public:
    int recipe;
    short uid;

    CraftItemPacket();
    ~CraftItemPacket();
    CraftItemPacket(int recipe, short uid);

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<CraftItemPacket>();
    }
    virtual int getId() { return 150; }
};