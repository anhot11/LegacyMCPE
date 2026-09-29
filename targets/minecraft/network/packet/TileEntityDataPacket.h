#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class CompoundTag;

class TileEntityDataPacket
    : public Packet,
      public std::enable_shared_from_this<TileEntityDataPacket> {
public:
    static inline constexpr int TYPE_MOB_SPAWNER = 1;
    static inline constexpr int TYPE_ADV_COMMAND = 2;
    static inline constexpr int TYPE_BEACON = 3;
    static inline constexpr int TYPE_SKULL = 4;

    int x, y, z;
    int type;
    CompoundTag* tag;

private:
    void _init();

public:
    TileEntityDataPacket();
    ~TileEntityDataPacket();
    TileEntityDataPacket(int x, int y, int z, int type, CompoundTag* tag);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<TileEntityDataPacket>();
    }
    virtual int getId() { return 132; }
};