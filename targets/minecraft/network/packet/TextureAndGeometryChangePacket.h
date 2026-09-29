#pragma once
#include <cstdint>
#include <memory>
#include <string>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Entity;

class TextureAndGeometryChangePacket
    : public Packet,
      public std::enable_shared_from_this<TextureAndGeometryChangePacket> {
public:
    int id;
    std::string path;
    std::uint32_t dwSkinID;

    TextureAndGeometryChangePacket();
    TextureAndGeometryChangePacket(std::shared_ptr<Entity> e,
                                   const std::string& path);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<TextureAndGeometryChangePacket>();
    }
    virtual int getId() { return 161; }
};
