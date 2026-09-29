#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class TileEditorOpenPacket
    : public Packet,
      public std::enable_shared_from_this<TileEditorOpenPacket> {
public:
    static inline constexpr int SIGN = 0;
    static inline constexpr int COMMAND_BLOCK = 1;

    int editorType;
    int x, y, z;

    TileEditorOpenPacket();
    TileEditorOpenPacket(int editorType, int x, int y, int z);

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<TileEditorOpenPacket>();
    }
    virtual int getId() { return 133; }
};