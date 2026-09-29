#pragma once

#include <memory>

#include "Packet.h"
#include "PacketListener.h"
#include "java/InputOutputStream/InputOutputStream.h"
#include "minecraft/network/packet/Packet.h"

class RotateHeadPacket : public Packet,
                         public std::enable_shared_from_this<RotateHeadPacket> {
public:
    int id;
    char yHeadRot;

    RotateHeadPacket();
    RotateHeadPacket(int id, char yHeadRot);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();
    virtual bool canBeInvalidated();
    virtual bool isInvalidatedBy(std::shared_ptr<Packet> packet);
    virtual bool isAync();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<RotateHeadPacket>();
    }
    virtual int getId() { return 35; }
};