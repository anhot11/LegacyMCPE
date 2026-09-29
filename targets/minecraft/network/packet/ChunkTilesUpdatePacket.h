#pragma once

#include <stdint.h>

#include <format>
#include <memory>
#include <vector>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Level;

class ChunkTilesUpdatePacket
    : public Packet,
      public std::enable_shared_from_this<ChunkTilesUpdatePacket> {
public:
    int xc, zc;
    std::vector<short> positions;
    std::vector<uint8_t> blocks;
    std::vector<uint8_t> data;
    uint8_t count;  // 4J Was int but never has a value higher than 10
    int levelIdx;

    ChunkTilesUpdatePacket();
    ~ChunkTilesUpdatePacket();
    ChunkTilesUpdatePacket(int xc, int zc, std::vector<short>& positions,
                           uint8_t count, Level* level);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<ChunkTilesUpdatePacket>();
    }
    virtual int getId() { return 52; }
};
