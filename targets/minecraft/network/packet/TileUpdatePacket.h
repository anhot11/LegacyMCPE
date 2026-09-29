#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Level;

class TileUpdatePacket : public Packet,
                         public std::enable_shared_from_this<TileUpdatePacket> {
public:
    int x, y, z;
    int block, data;
    int levelIdx;

    TileUpdatePacket();
    TileUpdatePacket(int x, int y, int z, Level* level);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<TileUpdatePacket>();
    }
    virtual int getId() { return 53; }
};