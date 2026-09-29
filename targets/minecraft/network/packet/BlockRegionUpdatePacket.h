#pragma once

#include <stdint.h>

#include <memory>
#include <vector>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Level;

class BlockRegionUpdatePacket
    : public Packet,
      public std::enable_shared_from_this<BlockRegionUpdatePacket> {
public:
    int x, y, z;
    int xs, ys, zs;
    std::vector<uint8_t> buffer;
    int levelIdx;
    bool bIsFullChunk;  // 4J Added

private:
    int size;

public:
    BlockRegionUpdatePacket();
    ~BlockRegionUpdatePacket();
    BlockRegionUpdatePacket(int x, int y, int z, int xs, int ys, int zs,
                            Level* level);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<BlockRegionUpdatePacket>();
    }
    virtual int getId() { return 51; }
};
