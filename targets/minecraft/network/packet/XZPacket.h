#pragma once

// 4J ADDED THIS PACKET

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class XZPacket : public Packet, public std::enable_shared_from_this<XZPacket> {
public:
    static const int STRONGHOLD;

    char action;
    int x;
    int z;

    XZPacket();
    ~XZPacket();
    XZPacket(char action, int x, int z);

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<XZPacket>();
    }
    virtual int getId() { return 166; }
};