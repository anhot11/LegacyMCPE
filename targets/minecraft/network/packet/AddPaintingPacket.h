#pragma once

#include <memory>
#include <string>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Painting;

class AddPaintingPacket
    : public Packet,
      public std::enable_shared_from_this<AddPaintingPacket> {
public:
    int id;
    int x, y, z;
    int dir;
    std::string motive;

public:
    AddPaintingPacket();
    AddPaintingPacket(std::shared_ptr<Painting> e);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<AddPaintingPacket>();
    }
    virtual int getId() { return 25; }
};
