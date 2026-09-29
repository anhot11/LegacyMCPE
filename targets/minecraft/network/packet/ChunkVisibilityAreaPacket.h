#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Level;

// 4J Added this packet so that when a player initially joins the game we just
// need to send them the area of chunks around them rather than one packet for
// each chunk
class ChunkVisibilityAreaPacket
    : public Packet,
      public std::enable_shared_from_this<ChunkVisibilityAreaPacket> {
public:
    int m_minX, m_maxX, m_minZ, m_maxZ;

private:
    // int size;

public:
    ChunkVisibilityAreaPacket();
    ChunkVisibilityAreaPacket(int minX, int maxX, int minZ, int maxZ);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<ChunkVisibilityAreaPacket>();
    }
    virtual int getId() { return 155; }
};
