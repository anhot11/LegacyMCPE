#pragma once

#include <format>
#include <memory>
#include <vector>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class ComplexItemDataPacket
    : public Packet,
      public std::enable_shared_from_this<ComplexItemDataPacket> {
public:
    short itemType;
    short itemId;
    std::vector<char> data;

    ComplexItemDataPacket();
    ~ComplexItemDataPacket();
    ComplexItemDataPacket(short itemType, short itemId,
                          std::vector<char>& data);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<ComplexItemDataPacket>();
    }
    virtual int getId() { return 131; }
};
