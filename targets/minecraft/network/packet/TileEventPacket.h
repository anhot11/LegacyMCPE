#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class TileEventPacket : public Packet,
                        public std::enable_shared_from_this<TileEventPacket> {
public:
    int x, y, z, b0, b1, tile;

    TileEventPacket();
    TileEventPacket(int x, int y, int z, int tile, int b0, int b1);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<TileEventPacket>();
    }
    virtual int getId() { return 54; }
};