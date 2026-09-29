#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class PlayerActionPacket
    : public Packet,
      public std::enable_shared_from_this<PlayerActionPacket> {
public:
    static const int START_DESTROY_BLOCK;
    static const int ABORT_DESTROY_BLOCK;
    static const int STOP_DESTROY_BLOCK;
    static const int DROP_ALL_ITEMS;
    static const int DROP_ITEM;
    static const int RELEASE_USE_ITEM;

    int x, y, z, face, action;

    PlayerActionPacket();
    PlayerActionPacket(int action, int x, int y, int z, int face);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<PlayerActionPacket>();
    }
    virtual int getId() { return 14; }
};
